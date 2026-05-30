#include "resp/handler.hpp"
#include "resp/serialiser.hpp"
#include "commands/db_commands.hpp"
#include "cache/Cache.hpp"
#include <cctype>
#include <string>

using namespace std;

Cache<string, string> storage(true);
string handle_value(const RespValue &value, ClientState &client)
{
  if (value.type != RespType::ARRAY || value.array.empty())
    return "";

  string command = value.array[0].str;
  
  for (auto &c : command)
    c = toupper(c);
  
  if (command == "MULTI")
  {
    if (client.in_multi)
    {
      RespValue err;
      err.type = RespType::ERROR;
      err.str = "ERR MULTI calls can not be nested";
      return serialise(err);
    }
    client.in_multi = true;
    client.tx_queue.clear();
    return "+OK\r\n";
  }
  else if (command == "DISCARD")
  {
    if (!client.in_multi)
    {
      RespValue err;
      err.type = RespType::ERROR;
      err.str = "ERR DISCARD without MULTI";
      return serialise(err);
    }
    client.tx_queue.clear();
    client.in_multi = false;
    return "+OK\r\n";
  }
  else if (command == "EXEC")
  {
    if (!client.in_multi)
    {
      RespValue err;
      err.type = RespType::ERROR;
      err.str = "ERR EXEC without MULTI";
      return serialise(err);
    }
    for (auto c:client.watchlist)
    {
      if (c.version != storage.getVersion(c.key))
      {
          client.tx_queue.clear();
            client.watchlist.clear();
            client.in_multi = false;
            RespValue res;
            res.type = RespType::NIL;
            return serialise(res);
      }
    }
    
    client.in_multi = false;
    string res = "*" + to_string(client.tx_queue.size()) + "\r\n";
    for (const auto &cmd_val : client.tx_queue)
    {
      res += execute_cmd(cmd_val);
    }
    client.tx_queue.clear();
    return res;
  }

  if (client.in_multi)
  {
    client.tx_queue.push_back(value);
    return "+QUEUED\r\n";
  }

  return execute_cmd(value);
}

string execute_cmd(const RespValue &value , ClientState &client){
  string response;

  if (value.type != RespType::ARRAY || value.array.empty())
    return response;

  string command = value.array[0].str;
  
  for (auto &c : command)
    c = toupper(c);

  if (command == "PING")
  {
    response = "+PONG\r\n";
  }
  else if (command == "ECHO" && value.array.size() >= 2)
  {
    response = serialise(value.array[1]);
  }
  else if (command == "SET" && value.array.size() >= 3)
  {
    return setKeys(value, storage, response) ? response : response;
  }
  else if (command == "GET" && value.array.size() >= 2)
  {
    return getKeys(value, storage, response) ? response : response;
  }
  else if (command == "INCR" && value.array.size() >= 1)
  {
    return incr(value, storage, response) ? response : response;
  }
  else if (command == "DECR" && value.array.size() >= 1)
  {
    return decr(value, storage, response) ? response : response;
  }
  else if (command == "INCRBY" && value.array.size() >= 1)
  {
    return incr_by(value, storage, response) ? response : response;
  }
  else if(command == "WATCH" && value.array.size() >=1){
    return watch(value , storage , response , client)? response :response;
  }
  else
  {
    RespValue err;
    err.type = RespType::ERROR;
    err.str = "ERR unknown command ";
    err.str += command;
    err.str += " or wrong number of arguments";
    response = serialise(err);
  }

  return response;
}