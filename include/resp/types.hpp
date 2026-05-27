#ifndef RESP_TYPES_HPP
#define RESP_TYPES_HPP

#include <string>
#include <vector>
#include <chrono>

enum class RespType
{
    STRING,
    ERROR,
    INTEGER,
    BULK,
    ARRAY,
    NIL,
    BULK_NULL
};

struct RespValue
{
    RespType type;
    std::string str;
    long long integer = 0;
    std::vector<RespValue> array;
    bool is_null = false;
};

struct ParseResults {
    bool has_expiry = false;  
    std::chrono::steady_clock::duration ttl{0};
};
struct ClientState {
    int fd;
    std::string accumulator;
    bool in_multi = false;
    std::vector<RespValue> tx_queue;
};
#endif // RESP_TYPES_HPP
