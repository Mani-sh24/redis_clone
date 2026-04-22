#include "helpers.h"
unordered_map<string, Entry> storage;
std::string handle_value(const RespValue &value)
{
  std::string response;

  if (value.type != RespType::ARRAY || value.array.empty())
    return response;

  std::string command = value.array[0].str;
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
  else if (command == "SET" && value.array.size() >=3)
  {
    Entry entry;
    entry.value = value.array[2].str;
    auto now = chrono::steady_clock::now();
    auto parsed = parse_set_options(value.array);
    if (parsed.has_expiry)
    {
      entry.expires_at = now + parsed.ttl;
    }
    else
    {
      entry.expires_at = now + chrono::steady_clock::duration::max();
    }
    storage[value.array[1].str] = entry;

    RespValue res;
    res.type = RespType::STRING;
    res.str = "OK";
    response = serialise(res);
  }
  else if (command == "GET" && value.array.size() >= 2)
  {
    auto it = storage.find(value.array[1].str);

    RespValue res;

    if (it != storage.end())
    {
      res.type = RespType::BULK_NULL;
    }
    else
    {
      auto now = std::chrono::steady_clock::now();

      if (now >= it->second.expires_at)
      {
        // expired → delete + return null
        storage.erase(it);
        res.type = RespType::BULK_NULL;
      }
      else
      {
        // valid → return value
        res.type = RespType::BULK;
        res.str = it->second.value;
      }
    }
    response = serialise(res);
  }
  else
  {
    RespValue err;
    err.type = RespType::ERROR;
    err.str = "ERR unknown command or wrong number of arguments";
    response = serialise(err);
  }

  return response;
}