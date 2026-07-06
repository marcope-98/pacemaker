#include <algorithm>
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

struct CSVContent
{
  std::vector<std::string>         header;
  std::vector<std::vector<double>> values;
};

CSVContent get_CSVContent(const std::filesystem::path &filename)
{
  auto csv_content = pacemaker::io::CSV::parse(filename);

  // header
  auto header = std::move(csv_content.front());
  csv_content.erase(csv_content.begin());

  auto expected_row_size = header.size();
  // clang-format off
  auto size_mismatch = [expected_row_size](const std::vector<std::string> &element)
                       { return element.size() != expected_row_size; };
  // clang-format on
  auto row_mismatch = std::any_of(csv_content.cbegin(), csv_content.cend(), size_mismatch);
  if (row_mismatch)
    throw std::invalid_argument("CSV has inconsistent number of columns");

  // values
  std::vector<std::vector<double>> values;
  values.reserve(csv_content.size());
  for (const auto &csv_row : csv_content)
  {
    std::vector<double> tmp;
    tmp.reserve(csv_row.size());
    for (const auto &st : csv_row)
    {
      double x{};
      auto [ptr, ec] = std::from_chars(st.data(), st.data() + st.size(), x);
      tmp.push_back((ec != std::errc() || ptr != st.data() + st.size())
                        ? std::numeric_limits<double>::quiet_NaN()
                        : x);
    }
    values.emplace_back(std::move(tmp));
  }

  return CSVContent{std::move(header), std::move(values)};
}

struct COMGuard
{
  COMGuard()
  {
    if (FAILED(CoInitialize(NULL))) 
      throw std::runtime_error("CoInitialize failed");
  }
  ~COMGuard() { CoUninitialize(); }
  COMGuard(const COMGuard &)            = delete;
  COMGuard &operator=(const COMGuard &) = delete;
  COMGuard(COMGuard &&)                 = delete;
  COMGuard &operator=(COMGuard &&)      = delete;
};

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
  if (argc != 3 && argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <period> <csv_file>\n"
              << "  [required] <period>    Sampling period, e.g. 100ms\n"
              << "  [required] <csv_file>  Path to the CSV input file\n"
              << "  [optional] <mf4_file>  Path to the recording output file or folder\n";
    return EXIT_FAILURE;
  }

  try
  {
    // Convert first argument to std::chrono::milliseconds (e.g. valid inputs are in the form: 10ms, invalid inputs are returned as 0ms)
    auto period = cvt_str_to_ms(std::string{argv[1]});

    // CSV parsing
    auto [header, values] = get_CSVContent(argv[2]);

    // MF4 save options
    auto mf4 = std::filesystem::path{""};
    if (argc == 4) mf4 = argv[3];

    // Actual execution of the inca automation
    [[maybe_unused]] COMGuard com;
    auto                      session = pacemaker::inca::Session::connect();
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
    timer.stop([&session, mf4] { session.stop_recording(mf4); });
    // clang-format on
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}