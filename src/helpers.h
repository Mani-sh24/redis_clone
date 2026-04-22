#ifndef HELPERS_H
#define HELPERS_H
#include<string>
#include<vector>
#include <unordered_map>
#include<chrono>
using namespace std;
using TimePoint = std::chrono::steady_clock::time_point;

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
struct Entry{
    string value;
    TimePoint expires_at;
};
struct RespValue
{
    RespType type;
    // keep it simple
    string str;
    long long integer;
    vector<RespValue> array;

    bool is_null = false;
};
struct ParseResults{
    bool has_expiry = false;  
    std::chrono::steady_clock::duration ttl{0};
};
string to_upper(string s);
void print_value(const RespValue &val, int depth = 0);
pair<RespValue, int>  parse_bulk_strings(string text, int pos);
pair<RespValue, int>parse_string(string text, int pos);
pair<RespValue, int>  parse_simple_errors(string text, int pos);
pair<RespValue, int>  parse_integers(string text, int pos);
pair<RespValue , int> prcoess_parser(string buffer , int offset);
string serialise(const RespValue& obj);

std::string handle_value(const RespValue &value);
ParseResults parse_set_options(const vector<RespValue>&args);

#endif