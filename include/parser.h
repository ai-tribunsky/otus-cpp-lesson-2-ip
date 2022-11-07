#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ip_v4.h"
#include "utils.h"

namespace ip_filter {

std::vector<IPv4> parse_ips(std::istream& stream, std::size_t column_idx,
                            char delimeter) {
  std::vector<IPv4> ips;

  std::string row;
  while (stream.good() && !stream.eof() && std::getline(stream, row) &&
         !row.empty()) {
    const auto columns = split(row, delimeter);
    if (column_idx <= columns.size() - 1) {
      try {
        ips.emplace_back(columns[column_idx]);
      } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
      }
    }
  }

  return ips;
}

}  // namespace ip_filter
