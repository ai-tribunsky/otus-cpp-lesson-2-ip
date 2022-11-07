#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "ip_v4.h"

namespace ip_filter {

/**
 * @brief Filter IPs list
 *
 * @param ips - input IPs list
 * @param n1 - 0 byte mask, 0 means all
 * @param n2 - 1 byte mask, 0 means all
 * @param n3 - 2 byte mask, 0 means all
 * @param n4 - 3 byte mask, 0 means all
 *
 * @return std::vector<IPv4> - filtered list
 */
std::vector<IPv4> filter_all(const std::vector<IPv4>& ips, uint8_t n1 = 0,
                             uint8_t n2 = 0, uint8_t n3 = 0, uint8_t n4 = 0) {
  std::vector<IPv4> filtered;
  filtered.reserve(ips.size());
  std::copy_if(ips.cbegin(), ips.cend(), std::back_inserter(filtered),
               [n1, n2, n3, n4](const IPv4& ip) -> bool {
                 return (n1 == 0 || n1 == ip[0]) && (n2 == 0 || n2 == ip[1]) &&
                        (n3 == 0 || n3 == ip[2]) && (n4 == 0 || n4 == ip[3]);
               });
  return filtered;
}

std::vector<IPv4> filter_any(const std::vector<IPv4>& ips, uint8_t n = 0) {
  std::vector<IPv4> filtered;
  filtered.reserve(ips.size());
  std::copy_if(ips.cbegin(), ips.cend(), std::back_inserter(filtered),
               [n](const IPv4& ip) -> bool {
                 return n == 0 || ip[0] == n || ip[1] == n || ip[2] == n ||
                        ip[3] == n;
               });
  return filtered;
}

}  // namespace ip_filter
