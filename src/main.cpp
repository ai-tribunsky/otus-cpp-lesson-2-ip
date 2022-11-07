#include <algorithm>
#include <functional>
#include <iostream>

#include "filter.h"
#include "parser.h"

int main() {
  auto ips = ip_filter::parse_ips(std::cin, 0, '\t');

  // print sorted IPs
  std::sort(ips.begin(), ips.end(), std::greater<>());
  print(ips);

  // print IPs, where ip[0] = 1
  auto filtered = ip_filter::filter_all(ips, 1);
  print(filtered);

  // print IPs, where ip[0] = 46 and ip[1] = 70
  filtered = ip_filter::filter_all(ips, 46, 70);
  print(filtered);

  // print IPs, where ip[0] = 46 or ip[1] = 46 or ip[2] = 46 or ip[3] = 46
  filtered = ip_filter::filter_any(ips, 46);
  print(filtered);

  return 0;
}
