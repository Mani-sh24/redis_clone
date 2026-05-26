#include "utils/aof.hpp"
#include <fstream>

void record(std::string command) {
    std::ofstream file("logs.txt", std::ios::app);
    if (file.is_open()) {
        size_t pos = 0;
        while ((pos = command.find('\r', pos)) != std::string::npos) {
            command.replace(pos, 1, "\\r");
            pos += 2; // Move past the newly inserted "\r"
        }

        pos = 0;
        while ((pos = command.find('\n', pos)) != std::string::npos) {
            command.replace(pos, 1, "\\n");
            pos += 2; // Move past the newly inserted "\n"
        }

        file << command << std::endl;
    }
}
