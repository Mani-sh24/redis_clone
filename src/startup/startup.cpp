#include<unistd.h>
#include<iostream>

void startup_info(__uint16_t &port){
    pid_t pid = getpid();
    std::cout << "===========================================================" << std::endl;
    std::cout << "Pid = " << pid << std::endl << "Port " << port << std::endl;
    std::cout << "===========================================================" << std::endl;
}