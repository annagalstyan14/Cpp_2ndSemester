#include <iostream>
#include <fstream>
#include <bitset>
#include <string_view>
#include <sstream>
#include <stdexcept>

constexpr uint32_t IPV4_LIMIT = 4294967296ULL; // 2^32

uint32_t convertIpToInteger(std::string_view ip_str) {
    uint32_t ip_int = 0;
    size_t pos = 0;
    int segment_count = 0;

    for (int shift = 24; shift >= 0; shift -= 8) {
        size_t next_dot_pos = ip_str.find('.', pos);
        if (next_dot_pos == std::string_view::npos) {
            next_dot_pos = ip_str.length();
        }

        if (pos >= next_dot_pos) {
            throw std::invalid_argument("Invalid IP address format.");
        }

        std::string segment(ip_str.substr(pos, next_dot_pos - pos));
        size_t processed_chars;
        int byte = std::stoi(segment, &processed_chars);
        if (processed_chars != segment.length() || byte < 0 || byte > 255) {
            throw std::out_of_range("Invalid octet in IP address.");
        }

        ip_int |= (static_cast<uint32_t>(byte) << shift);
        pos = next_dot_pos + 1;
        segment_count++;
    }

    if (segment_count != 4 || pos < ip_str.length()) {
        throw std::invalid_argument("IP address must consist of exactly four octets.");
    }

    return ip_int;
}

uint32_t processIpFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return UINT32_MAX; // Error marker
    }

    uint32_t ip_count;
    file >> ip_count;
    if (!file) {
        std::cerr << "Error reading IP count from file." << std::endl;
        return UINT32_MAX;
    }

    std::bitset<IPV4_LIMIT> ip_bits;
    std::string ip;
    for (uint32_t i = 0; i < ip_count && file >> ip; ++i) {
        try {
            uint32_t ip_as_int = convertIpToInteger(ip);
            ip_bits.set(ip_as_int);
        } 
        catch (const std::exception& err) {
            std::cerr << "Invalid IP address '" << ip << "': " << err.what() << std::endl;
            return UINT32_MAX;
        }
    }

    if (file.fail() && !file.eof()) {
        std::cerr << "Error reading IP addresses from file." << std::endl;
        return UINT32_MAX;
    }

    return static_cast<uint32_t>(ip_bits.count());
}
