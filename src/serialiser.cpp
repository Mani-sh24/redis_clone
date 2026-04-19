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
        return "$"+obj.str.length()+'\r\n'+obj.str+"\r\n";
        default:
        return "error";
    }
}