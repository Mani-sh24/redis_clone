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
    default:
        break;
    }
}