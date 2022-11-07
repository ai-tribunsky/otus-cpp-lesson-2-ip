#pragma once

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

#include "utils.h"

namespace ip_filter {

constexpr std::size_t IP_V4_PARTS_COUNT = 4;
constexpr char IP_V4_DELIMETER = '.';

class IPv4 {
 public:
  explicit IPv4(const std::string& ip) {
    const auto ip_parts = split(ip, IP_V4_DELIMETER);
    if (ip_parts.size() != 4) {
      throw std::invalid_argument("Malformed IP address: " + ip);
    }

    ip_ = {static_cast<uint8_t>(std::stoi(ip_parts[0])),
           static_cast<uint8_t>(std::stoi(ip_parts[1])),
           static_cast<uint8_t>(std::stoi(ip_parts[2])),
           static_cast<uint8_t>(std::stoi(ip_parts[3]))};

    ip_int_ =
        ip_[0] * (1 << 24) + ip_[1] * (1 << 16) + ip_[2] * (1 << 8) + ip_[3];
    ip_str_ = ip;
  }

  uint8_t operator[](std::size_t idx) const { return ip_[idx]; };

  uint64_t to_int() const { return ip_int_; }

  std::string to_string() const { return ip_str_; }

  friend bool operator<(const IPv4& l, const IPv4& r) {
    return l.ip_int_ < r.ip_int_;
  }

  friend bool operator>(const IPv4& l, const IPv4& r) {
    return l.ip_int_ > r.ip_int_;
  }

  friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
    os << ip.ip_str_;
    return os;
  }

 private:
  std::array<uint8_t, IP_V4_PARTS_COUNT> ip_;
  uint64_t ip_int_;
  std::string ip_str_;
};

}  // namespace ip_filter
