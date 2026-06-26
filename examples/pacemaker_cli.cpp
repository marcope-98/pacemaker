#include <charconv>
#include <chrono>
#include <iostream>
#include <limits>
#include <string>
#include <vector>


#include <comdef.h>

#include "pacemaker/inca/Session.hpp"
#include "pacemaker/io/CSV.hpp"
#include "pacemaker/timer/Timer.hpp"

using namespace std::chrono_literals;

std::vector<std::string>         get_header(std::vector<std::vector<std::string>> &csv_content) { return std::move(csv_content.front()); }
std::vector<std::vector<double>> get_values(std::vector<std::vector<std::string>> &csv_content)
{
  std::vector<std::vector<double>> values;
  values.reserve(csv_content.size() - 1);
  for (std::size_t row{1}; row < csv_content.size(); ++row)
  {
    const auto &csv_row = csv_content[row];

    std::vector<double> tmp;
    tmp.reserve(csv_row.size());
    for (const auto &st : csv_row)
    {
      double      x{};
      const auto *begin = st.data();
      const auto *end   = begin + st.size();
      auto [ptr, ec]    = std::from_chars(begin, end, x);
      if (ec != std::errc() || ptr != end)
        x = std::numeric_limits<double>::quiet_NaN();
      tmp.push_back(x);
    }
    values.emplace_back(std::move(tmp));
  }
  return values;
}

std::chrono::milliseconds cvt_str_to_ms(const std::string &s)
{
  std::size_t num_end{};
  auto        value = std::stoll(s, &num_end);
  std::string unit  = s.substr(num_end);
  if (unit == "ms") return std::chrono::milliseconds{value};
  return 0ms;
}

int main(int argc, char *argv[])
{
  // Initial parameter check
  if (argc != 3)
  {
    std::cerr << "Incorrect number of arguments provided. Expected 2, got " + std::to_string(argc - 1) << "\n";
    return EXIT_FAILURE;
  }

  // Convert first argument to std::chrono::milliseconds (e.g. valid inputs are in the form: 10ms, invalid inputs are returned as 0ms)
  auto period = cvt_str_to_ms(std::string{argv[1]});

  // CSV parsing
  std::vector<std::string>         header;
  std::vector<std::vector<double>> values;
  try
  {
    auto csv_content = pacemaker::io::CSV::parse(argv[2]);
    header           = std::move(get_header(csv_content));
    values           = std::move(get_values(csv_content));
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  // Actual execution of the inca automation
  CoInitialize(NULL);
  try
  {
    auto session = pacemaker::inca::Session::connect();
    for (const auto &name : header)
      session.add_param(name);

    auto timer = pacemaker::timer::Timer(period, values.size());
    // clang-format off
    timer.start([&session] { session.start_recording(); });
    timer.wait([&session, &header, &values](std::size_t i)
               {
                for (std::size_t j{}; j < values[i].size(); ++j)
                  if (!std::isnan(values[i][j]))
                    session.set_param(header[j], values[i][j]); 
                });
    timer.stop([&session] { session.stop_recording(""); });
    // clang-format on
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
  CoUninitialize();

  return 0;
}