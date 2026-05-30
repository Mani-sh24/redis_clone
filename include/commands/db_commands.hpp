#ifndef DB_COMMANDS_HPP
#define DB_COMMANDS_HPP

#include "resp/types.hpp"
#include "cache/Cache.hpp"
#include <string>
#include <vector>

bool setKeys(const RespValue &value, Cache<std::string, std::string> &storage, std::string &response);
bool getKeys(const RespValue &value, Cache<std::string, std::string> &storage, std::string &response);
bool incr(const RespValue &value, Cache<std::string, std::string> &storage, std::string &response);
bool decr(const RespValue &value, Cache<std::string, std::string> &storage, std::string &response);
bool incr_by(const RespValue &value, Cache<std::string, std::string> &storage, std::string &response);
bool watch(const RespValue &value, Cache<string, string> &storage, string &response, ClientState &client);
ParseResults parse_set_options(const std::vector<RespValue> &args);

#endif // DB_COMMANDS_HPP
