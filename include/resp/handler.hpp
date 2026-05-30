#ifndef RESP_HANDLER_HPP
#define RESP_HANDLER_HPP

#include "resp/types.hpp"
#include <string>

std::string handle_value(const RespValue &value, ClientState &client);
std::string execute_cmd(const RespValue &value , ClientState &Client);

#endif // RESP_HANDLER_HPP
