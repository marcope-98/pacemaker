#include "pacemaker/io/CSV.hpp"

#include "bison-csv.hpp"
#include "pacemaker/io/CSVLexer.hpp"

auto pacemaker::io::CSV::parse(const std::filesystem::path &filename) -> std::vector<std::vector<std::string>>
{
  if (!std::filesystem::exists(filename) || !std::filesystem::is_regular_file(filename))
    throw std::invalid_argument("provided filename does not exist or is not a regular file");

  std::ifstream file{filename, std::ios::binary};
  if (!file.good()) throw std::invalid_argument("Could not open file");

  return parse_helper(file);
}

auto pacemaker::io::CSV::parse_helper(std::istream &stream) -> std::vector<std::vector<std::string>>
{
  pacemaker::io::CSVLexer  lexer(&stream);
  pacemaker::io::CSVParser parser(lexer);
  parser.parse();
  return lexer.data();
}