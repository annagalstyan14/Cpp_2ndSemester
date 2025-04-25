#include "Utilities.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace Utils {

    double toDouble(const std::string& s){
        std::stringstream ss(s);
        double result;
        ss << result;
        if (ss.fail()) throw std::runtime_error("Invalid number: "+ s);
        return result;
    }

    std::string trim(const std::string& s){
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end = s.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    bool isNumber(const std::string& s){
        std::string str = trim(s);
        if (str.empty()) return false;
        char* end;
        std::strtod(str.c_str(), &end);
        return (*end == '\0');
    }
}