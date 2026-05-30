#ifndef RESP_TYPES_HPP
#define RESP_TYPES_HPP

#include <string>
#include <vector>
#include <chrono>
#include "cache/Cache.hpp"
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
struct WatchedItem {
    string_view key;
    uint64_t version = 0;
};
struct ClientState {
    int fd;
    std::string accumulator;
    bool in_multi = false;
    std::vector<RespValue> tx_queue;
    std::vector<WatchedItem> watchlist;
};
#endif // RESP_TYPES_HPP
