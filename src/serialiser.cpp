#include"helpers.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

string serialise(const RespValue& obj){
    switch (obj.type)
    {
    case RespType::STRING:
        return "+"+obj.str +"\r\n";
    case RespType::BULK:
        return "$" + to_string(obj.str.length()) + "\r\n" + obj.str + "\r\n";
    case RespType::ERROR:
        return "-"+obj.str+ "\r\n";
    case RespType::NIL:
            return "$-1\r\n";
    default:
        return "error";
    }
}