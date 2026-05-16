#include "helpers.h"
#include "Cache.hpp"

string to_upper(string s)
{
    for (auto &c : s)
        c = toupper(c);
    return s;
}

bool setKeys(const RespValue &value , Cache<string , string> &storage , string &response)
{
    RespValue res;

    if (value.type != RespType::ARRAY || value.array.size() < 3)
    {
        res.type = RespType::ERROR;
        res.str = "Error invalid arguments!";
        response = serialise(res);
        return false;
    }

    if (value.array[1].type != RespType::BULK ||
        value.array[2].type != RespType::BULK)
    {
        res.type = RespType::ERROR;
        res.str = "Error invalid arguments!";
        response = serialise(res);
        return false;
    }

    try
    {
        auto parsed = parse_set_options(value.array);

        if (parsed.has_expiry)
        {
            int ttl_seconds = chrono::duration_cast<chrono::seconds>(parsed.ttl).count();
            storage.put(value.array[1].str, value.array[2].str, ttl_seconds);
        }
        else
        {
            storage.put(value.array[1].str, value.array[2].str);
        }

        res.type = RespType::STRING;
        res.str = "OK";
    }
    catch (const exception &e)
    {
        res.type = RespType::ERROR;
        res.str = e.what();
        response = serialise(res);
        return false;
    }

    response = serialise(res);
    return true;
}

bool getKeys(const RespValue &value , Cache<string , string> &storage , string &response)
{
    RespValue res;

    if (value.type != RespType::ARRAY || value.array.size() < 2)
    {
        res.type = RespType::ERROR;
        res.str = "Error invalid arguments!";
        response = serialise(res);
        return false;
    }

    if (value.array[1].type != RespType::BULK)
    {
        res.type = RespType::ERROR;
        res.str = "Error invalid arguments!";
        response = serialise(res);
        return false;
    }

    if (!storage.exists(value.array[1].str))
    {
        res.type = RespType::BULK_NULL;
    }
    else
    {
        res.type = RespType::BULK;
        res.str = storage.get(value.array[1].str);
    }

    response = serialise(res);
    return true;
}

ParseResults parse_set_options(const vector<RespValue> &args)
{
    ParseResults result;

    int i = 3;

    while (i < args.size())
    {
        string token = to_upper(args[i].str);

        if (token == "EX" || token == "PX")
        {
            if (i + 1 >= args.size())
            {
                throw runtime_error("NO TIME PROVIDED");
            }

            int ttl = stoi(args[i + 1].str);

            if (ttl < 0)
            {
                throw runtime_error("TTL must be non-negative");
            }

            if (result.has_expiry)
            {
                throw runtime_error("Multiple expiry options not allowed");
            }

            if (token == "EX")
            {
                result.ttl = chrono::seconds(ttl);
            }
            else
            {
                result.ttl = chrono::milliseconds(ttl);
            }

            result.has_expiry = true;
            i += 2;
        }
        else
        {
            throw runtime_error("Unknown option " + token);
        }
    }

    return result;
}