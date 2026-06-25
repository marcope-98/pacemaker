#ifndef PACEMAKER_IO_CSV_HPP_
#define PACEMAKER_IO_CSV_HPP_

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace pacemaker::io
{
  /**
   * @brief Stateless utility for parsing CSV files into in-memory records.
   *
   * `CSV exposes a single static entry point, `parse()`, which reads a file
   * from disk and returns its contents as
   * `std::vector<std::vector<std::string>>`: one inner vector per record
   * (row), each containing the unescaped field values of that record in order.
   *
   * The underlying grammar (see `csv.l` / `csv.y`) follows the RFC 4180 conventions:
   * - Records are separated by `CRLF`, `CR` or `LF`.
   * - Fields within a record are separated by a comma.
   * - A field may be **unescaped** (printable ASCII excluding comma and double-quote)
   *   or **escaped** (enclosed in double quotes, in which case it may contain commas,
   *   `CR`, `LF`, and double-quote characters represented as a double `""`).
   * - Escaped fields are unescaped during parsing: a doubled quote `""` becomes
   *   a single `"` in the returned string, and embedded `CR`/`LF` tokens become
   *   literal `\r`/`\n` characters in the field.
   *
   * `CSV` has no instance state; both `parse()` and the private `parse_helper()` are static.
   * The class exists purely to namespace the parsing entry point and is never instantiated.
   *
   * ### Typical usage
   * @code
   * auto rows = pacemaker::io::CSV::parse("measurements.csv");
   * for(const auto &row : rows)
   * {
   *   for (const auto &field : row)
   *     std::cout << field << '\t';
   *   std::cout << '\n';
   * }
   * @endcode
   *
   * @see pacemaker::io::CSVLexer
   */
  class CSV
  {
  public:
    /**
     * @brief Parses a CSV file from disk into a vector of records.
     *
     * Validates that @p filename exists and refers to a regular file, opens
     * it for reading, and delegates to `parse_helper()` to run the
     * lexer/parser over the resulting stream.
     *
     * @param filename Path to the CSV file to parse.
     *
     * @return A vector of records, each a vector of field strings in
     *         document order. An empty file yields an empty (or
     *         grammar-dependent minimal) result; no record is syntesised
     *         for input that contains no data.
     *
     * @throws std::invalid_argument if @p filename does not exist or does not refer to a regular file
     * @throws std::invalid_argument if the file cannot be opened for reading (e.g.\ permissions)
     * @throws (parse-defined) if the file content does not conform to the CSV grammar;
     *         parse errors are currently reported to `std::cerr` by `CSVParser::error()` rather that
     *         thrown.
     */
    static auto parse(const std::filesystem::path &filename) -> std::vector<std::vector<std::string>>;

  private:
    /**
     * @brief Runs the lexer/parser pair over an already-open input stream.
     *
     * Constructs a `CSVLexer` bound to @p stream and a generated
     * `CSVParser` bound to that lexer, invokes `CSVParser::parse()` to
     * consume the entire srteam, and returns the accumulated records via `CSVLexer::data()`.
     *
     * Factored out from `parse()` so that the file-existence and
     * file-opening logic is independently testable from the parsing logic
     * (e.g.\ by passing an `std::istringstream` directly in unit tests).
     *
     * @param stream Input stream positioned at the start of CSV content.
     *               Must remain valid for the duration of the call.
     *
     * @return A vector of records, each a vector of field strings,
     *         as accumulated by the lexer during parsing.
     */
    static auto parse_helper(std::istream &stream) -> std::vector<std::vector<std::string>>;
  };
} // namespace pacemaker::io

#endif // PACEMAKER_IO_CSV_HPP_