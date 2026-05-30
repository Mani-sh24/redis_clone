#ifndef STRING_HELPERS_HPP
#define STRING_HELPERS_HPP

#include <string>
#include <string_view>
#include <optional>

std::string to_upper(std::string_view s);
std::optional<int> parse_int(std::string_view str);

#endif // STRING_HELPERS_HPP
