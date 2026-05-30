#include "utils/aof.hpp"
#include <fstream>

void record(std::string_view command) {
    std::ofstream file("logs.txt", std::ios::app);
    if (file.is_open()) {
        std::string command_str(command);
        size_t pos = 0;
        while ((pos = command_str.find('\r', pos)) != std::string::npos) {
            command_str.replace(pos, 1, "\\r");
            pos += 2; // Move past the newly inserted "\r"
        }

        pos = 0;
        while ((pos = command_str.find('\n', pos)) != std::string::npos) {
            command_str.replace(pos, 1, "\\n");
            pos += 2; // Move past the newly inserted "\n"
        }

        file << command_str << std::endl;
    }
}
