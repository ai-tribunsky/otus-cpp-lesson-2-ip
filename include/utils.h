#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace ip_filter {

/**
 * @brief Split string inti chunks by specified delimeter
 *
 * @param input - inpit string
 * @param delimeter - delimeter character
 * @return std::vector<std::string> - chunks list
 */
std::vector<std::string> split(const std::string& input, char delimeter) {
  std::vector<std::string> chunks;

  auto begin = input.cbegin();
  std::size_t start{0};
  std::size_t delimeter_pos = input.find_first_of(delimeter, start);
  while (delimeter_pos != std::string::npos) {
    if (start != delimeter_pos) {
      chunks.emplace_back(begin + start, begin + delimeter_pos);
    } else {
      chunks.emplace_back();
    }

    start = delimeter_pos + 1;
    delimeter_pos = input.find_first_of(delimeter, start);
  }

  if (start < input.size()) {
    chunks.emplace_back(begin + start, begin + input.size());
  } else if (start == input.size()) {
    chunks.emplace_back();
  }

  return chunks;
}

template <typename T>
void print(const std::vector<T>& vec) {
  std::for_each(vec.cbegin(), vec.cend(),
                [](const auto& element) { std::cout << element << '\n'; });
}

}  // namespace ip_filter
