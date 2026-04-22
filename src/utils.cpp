#include "helpers.h"
string to_upper(string s)
{
    for (auto &c : s)
        c = toupper(c);
    return s;
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
                result.ttl = chrono::milliseconds(ttl * 1000);
            }
            else if (token == "PX")
            { // PX
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