#include<unistd.h>
#include<iostream>

void startup_info(__uint16_t &port){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    
    std::cout << "                _._" << std::endl;
    std::cout << "           _.-``__ ''-._" << std::endl;
    std::cout << "      _.-``    `.  `_.  ''-._          Redis C++ Clone" << std::endl;
    std::cout << "  .-`` .-```.  ```\\/    _.,_ ''-._     Port: " << port << std::endl;
    std::cout << " (    ' (＃｀д´)     .-`  | `,    )    PID:  " << pid << std::endl;
    std::cout << " |`-._`-...-` __...-.``-._|'` _.-'|    PPID: " << ppid << std::endl;
    std::cout << " |    `-._   `._    /     _.-'    |    UID:  " << uid << std::endl;
    std::cout << "  `-._    `-._  `-./___.-'    _.-'" << std::endl;
    std::cout << "      `-._    `-._   _.-'  _.-'" << std::endl;
    std::cout << "          `-._    `-._ _.-'" << std::endl;
    std::cout << "              `-._  _.-'" << std::endl;
    std::cout << "                  `-'" << std::endl << std::endl;
}