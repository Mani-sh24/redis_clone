#ifndef RESP_DESERIALISER_HPP
#define RESP_DESERIALISER_HPP

#include "resp/types.hpp"
#include <string>
#include <utility>

std::pair<RespValue, int> parse_bulk_strings(std::string text, int pos);
std::pair<RespValue, int> parse_string(std::string text, int pos);
std::pair<RespValue, int> parse_simple_errors(std::string text, int pos);
std::pair<RespValue, int> parse_integers(std::string text, int pos);
std::pair<RespValue, int> parse_array(std::string text, int pos);
std::pair<RespValue, int> process_parser(std::string buffer, int offset);
void print_value(const RespValue &val, int depth = 0);

#endif // RESP_DESERIALISER_HPP
