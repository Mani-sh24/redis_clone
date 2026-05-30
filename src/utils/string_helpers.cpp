#include "utils/string_helpers.hpp"
#include <cctype>
#include <charconv>

std::string to_upper(std::string_view s)
{
    std::string result(s);
    for (auto &c : result)
        c = toupper(c);
    return result;
}

std::optional<int> parse_int(std::string_view str)
{
    int val;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), val);
    if (ec == std::errc() && ptr == str.data() + str.size())
        return val;
    return std::nullopt;
}
