#include <gtest/gtest.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "filter.h"
#include "ip_v4.h"
#include "parser.h"
#include "utils.h"

// utils
TEST(IpFilterUtils, Split) {
  std::string str = "67.232.81.208	1	0";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "67.232.81.208");
  EXPECT_EQ(result[1], "1");
  EXPECT_EQ(result[2], "0");
}

TEST(IpFilterUtils, SplitStartsWithDelimeter) {
  std::string str = "	1	0";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "");
  EXPECT_EQ(result[1], "1");
  EXPECT_EQ(result[2], "0");
}

TEST(IpFilterUtils, SplitDoubleDelimeterMiddle) {
  std::string str = "1		0";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "1");
  EXPECT_EQ(result[1], "");
  EXPECT_EQ(result[2], "0");
}

TEST(IpFilterUtils, SplitDelimeterEnd) {
  std::string str = "11.232.81.208	";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "11.232.81.208");
  EXPECT_EQ(result[1], "");
}

TEST(IpFilterUtils, SplitDoubleDelimeterEnd) {
  std::string str = "11.232.81.208		";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "11.232.81.208");
  EXPECT_EQ(result[1], "");
  EXPECT_EQ(result[2], "");
}

TEST(IpFilterUtils, SplitOnlyDelimeters) {
  std::string str = "		";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[1], "");
  EXPECT_EQ(result[2], "");
  EXPECT_EQ(result[3], "");
}

TEST(IpFilterUtils, SplitNonExistedDelimeter) {
  std::string str = "67.232.81.208";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "67.232.81.208");
}

TEST(IpFilterUtils, SplitEmpty) {
  std::string str = "";
  const auto result = ip_filter::split(str, '\t');
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "");
}

// ip_v4
TEST(IpFilterIPv4, ConstructCorrect) {
  try {
    ip_filter::IPv4 ip("67.232.81.208");
    EXPECT_EQ(ip[0], 67);
    EXPECT_EQ(ip[1], 232);
    EXPECT_EQ(ip[2], 81);
    EXPECT_EQ(ip[3], 208);
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterIPv4, ConstructMalformed) {
  try {
    ip_filter::IPv4 ip("67.232.81");
    FAIL() << "Expected invalid_argument exception";
  } catch (const std::invalid_argument& e) {
    ASSERT_EQ(std::string(e.what()),
              std::string("Malformed IP address: 67.232.81"));
  }

  try {
    ip_filter::IPv4 ip("67.232.81.");
    FAIL() << "Expected invalid_argument exception";
  } catch (const std::invalid_argument& e) {
    ASSERT_EQ(std::string(e.what()), std::string("stoi"));
  }

  try {
    ip_filter::IPv4 ip("67.232.81..");
    FAIL() << "Expected invalid_argument exception";
  } catch (const std::invalid_argument& e) {
    ASSERT_EQ(std::string(e.what()),
              std::string("Malformed IP address: 67.232.81.."));
  }

  try {
    ip_filter::IPv4 ip("some string");
    FAIL() << "Expected invalid_argument exception";
  } catch (const std::invalid_argument& e) {
    ASSERT_EQ(std::string(e.what()),
              std::string("Malformed IP address: some string"));
  }

  try {
    ip_filter::IPv4 ip("");
    FAIL() << "Expected invalid_argument exception";
  } catch (const std::invalid_argument& e) {
    ASSERT_EQ(std::string(e.what()), std::string("Malformed IP address: "));
  }
}

TEST(IpFilterIPv4, IntRepresentation) {
  try {
    ip_filter::IPv4 ip("92.11.213.208");
    ASSERT_EQ(ip.to_int(), 1544279504);
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterIPv4, StrRepresentation) {
  try {
    ip_filter::IPv4 ip("92.11.213.208");
    ASSERT_EQ(ip.to_string(), "92.11.213.208");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterIPv4, Sorting) {
  try {
    std::vector<ip_filter::IPv4> ips{
        ip_filter::IPv4("1.76.12.11"), ip_filter::IPv4("1.77.12.11"),
        ip_filter::IPv4("1.0.12.11"),  ip_filter::IPv4("10.0.12.11"),
        ip_filter::IPv4("2.11.12.11"), ip_filter::IPv4("35.11.9.11"),
        ip_filter::IPv4("35.11.9.1"),
    };
    std::sort(ips.begin(), ips.end());
    EXPECT_EQ(ips[0].to_string(), "1.0.12.11");
    EXPECT_EQ(ips[1].to_string(), "1.76.12.11");
    EXPECT_EQ(ips[2].to_string(), "1.77.12.11");
    EXPECT_EQ(ips[3].to_string(), "2.11.12.11");
    EXPECT_EQ(ips[4].to_string(), "10.0.12.11");
    EXPECT_EQ(ips[5].to_string(), "35.11.9.1");
    EXPECT_EQ(ips[6].to_string(), "35.11.9.11");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterIPv4, StreamOutput) {
  try {
    ip_filter::IPv4 ip("1.76.12.11");
    std::stringstream ss;
    ss << ip;
    ASSERT_EQ(ss.str(), ip.to_string());
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

// parser
TEST(IpFilterParser, CorrectTsv) {
  std::stringstream ss;
  ss << "113.162.145.156	111	0" << '\n'
     << "157.39.22.224	5	6" << '\n'
     << "79.180.73.190	2	1" << '\n'
     << "179.210.145.4	22	0" << '\n'
     << "219.102.120.135	486	0";
  try {
    auto result = ip_filter::parse_ips(ss, 0, '\t');
    ASSERT_EQ(result.size(), 5);
    EXPECT_EQ(result[0].to_string(), "113.162.145.156");
    EXPECT_EQ(result[1].to_string(), "157.39.22.224");
    EXPECT_EQ(result[2].to_string(), "79.180.73.190");
    EXPECT_EQ(result[3].to_string(), "179.210.145.4");
    EXPECT_EQ(result[4].to_string(), "219.102.120.135");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterParser, MalformedTsv) {
  std::stringstream ss;
  ss << "113.162.145.	111	0" << '\n'
     << "157.39.22.224" << '\n'
     << "79.180.73.190	2" << '\n'
     << "179.210.145.4	22	0" << '\n'
     << "	486	0";
  try {
    auto result = ip_filter::parse_ips(ss, 0, '\t');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].to_string(), "157.39.22.224");
    EXPECT_EQ(result[1].to_string(), "79.180.73.190");
    EXPECT_EQ(result[2].to_string(), "179.210.145.4");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterParser, EmptyStream) {
  std::stringstream ss;
  try {
    auto result = ip_filter::parse_ips(ss, 0, '\t');
    ASSERT_EQ(result.size(), 0);
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

// filter
TEST(IpFilterFilter, FilterAllExisted) {
  try {
    std::vector<ip_filter::IPv4> ips{
        ip_filter::IPv4("1.76.12.11"), ip_filter::IPv4("1.76.12.13"),
        ip_filter::IPv4("1.0.12.11"),  ip_filter::IPv4("10.0.12.11"),
        ip_filter::IPv4("2.11.12.11"), ip_filter::IPv4("35.11.9.11"),
        ip_filter::IPv4("35.11.9.1"),  ip_filter::IPv4("35.11.8.1")};
    auto result = ip_filter::filter_all(ips, 2);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].to_string(), "2.11.12.11");

    result = ip_filter::filter_all(ips, 1, 76);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].to_string(), "1.76.12.11");
    EXPECT_EQ(result[1].to_string(), "1.76.12.13");

    result = ip_filter::filter_all(ips, 35, 11, 9);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].to_string(), "35.11.9.11");
    EXPECT_EQ(result[1].to_string(), "35.11.9.1");

    result = ip_filter::filter_all(ips, 35, 11, 8, 1);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].to_string(), "35.11.8.1");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterFilter, FilterAllNonExisted) {
  try {
    std::vector<ip_filter::IPv4> ips{
        ip_filter::IPv4("1.76.12.11"), ip_filter::IPv4("1.76.12.13"),
        ip_filter::IPv4("1.0.12.11"),  ip_filter::IPv4("10.0.12.11"),
        ip_filter::IPv4("2.11.12.11"), ip_filter::IPv4("35.11.9.11"),
        ip_filter::IPv4("35.11.9.1"),
    };
    auto result = ip_filter::filter_all(ips, 22);
    ASSERT_EQ(result.size(), 0);

    result = ip_filter::filter_all(ips, 2, 12);
    ASSERT_EQ(result.size(), 0);

    result = ip_filter::filter_all(ips, 1, 0, 13);
    ASSERT_EQ(result.size(), 0);

    result = ip_filter::filter_all(ips, 1, 0, 12, 211);
    ASSERT_EQ(result.size(), 0);
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterFilter, FilterAllEmpty) {
  std::vector<ip_filter::IPv4> ips{};
  auto result = ip_filter::filter_all(ips, 2, 45, 11);
  ASSERT_EQ(result.size(), 0);
}

TEST(IpFilterFilter, FilterAnyExisted) {
  try {
    std::vector<ip_filter::IPv4> ips{
        ip_filter::IPv4("1.76.12.11"), ip_filter::IPv4("1.76.12.13"),
        ip_filter::IPv4("1.0.12.11"),  ip_filter::IPv4("10.0.12.11"),
        ip_filter::IPv4("2.11.12.11"), ip_filter::IPv4("35.11.9.11"),
        ip_filter::IPv4("35.11.9.1"),  ip_filter::IPv4("35.11.8.1")};
    auto result = ip_filter::filter_any(ips, 11);
    ASSERT_EQ(result.size(), 7);
    EXPECT_EQ(result[0].to_string(), "1.76.12.11");
    EXPECT_EQ(result[1].to_string(), "1.0.12.11");
    EXPECT_EQ(result[2].to_string(), "10.0.12.11");
    EXPECT_EQ(result[3].to_string(), "2.11.12.11");
    EXPECT_EQ(result[4].to_string(), "35.11.9.11");
    EXPECT_EQ(result[5].to_string(), "35.11.9.1");
    EXPECT_EQ(result[6].to_string(), "35.11.8.1");
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterFilter, FilterAnyNonExisted) {
  try {
    std::vector<ip_filter::IPv4> ips{
        ip_filter::IPv4("1.76.12.11"), ip_filter::IPv4("1.76.12.13"),
        ip_filter::IPv4("1.0.12.11"),  ip_filter::IPv4("10.0.12.11"),
        ip_filter::IPv4("2.11.12.11"), ip_filter::IPv4("35.11.9.11"),
        ip_filter::IPv4("35.11.9.1"),
    };
    auto result = ip_filter::filter_any(ips, 221);
    ASSERT_EQ(result.size(), 0);
  } catch (const std::invalid_argument& e) {
    FAIL() << "Not expected exception: " << e.what();
  }
}

TEST(IpFilterFilter, FilterAnyEmpty) {
  std::vector<ip_filter::IPv4> ips{};
  auto result = ip_filter::filter_all(ips, 2, 45, 11);
  ASSERT_EQ(result.size(), 0);
}
