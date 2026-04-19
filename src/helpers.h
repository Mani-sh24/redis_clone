#ifndef HELPERS_H
#define HELPERS_H
#include<string>
#include<vector>
using namespace std;
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
    // keep it simple
    string str;
    long long integer;
    vector<RespValue> array;

    bool is_null = false;
};
void print_value(const RespValue &val, int depth = 0);
pair<RespValue, int>  parse_bulk_strings(string text, int pos);
pair<RespValue, int>parse_string(string text, int pos);
pair<RespValue, int>  parse_simple_errors(string text, int pos);
pair<RespValue, int>  parse_integers(string text, int pos);
pair<RespValue , int> prcoess_parser(string buffer , int offset);
string serialise(const RespValue& obj);
#endif