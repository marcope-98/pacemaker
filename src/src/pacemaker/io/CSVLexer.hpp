#ifndef PACEMAKER_IO_CSVLEXER_HPP_
#define PACEMAKER_IO_CSVLEXER_HPP_

#include <istream>
#include <string>
#include <vector>

#ifndef yyFlexLexerOnce
#include "FlexLexer.h"
#endif

namespace pacemaker::io
{
  /**
   * @brief Tokenizer and record accumulator for CSV parsing.
   *
   * `CSVLexer` extends the Flex-generated `yyFlexLexer` base with:
   * - a typed `yylex(std::string*)` overload that returns the token kind and writes
   *   the matched text (for `TEXTDATA` tokens) into the caller-supplied string, matching
   *   the `%define api.value.type` and `YY_DECL` signature configured in `csv.l`;
   * - record/field accumulation methods (`newline()`, `append`) that the Bison grammar actions in
   *   `csv.y` call as each field and record boundary is recognised;
   * - accessors (`data()`, `reset()`) for retrieving and clearing the accumulated result.
   *
   * The lexer recognises five token kinds, defined in `csv.l`:
   * `CR` (`\r`), `LF` (`\n`), `COMMA` (`,`), `DQUOTE` (`"`), and `TEXTDATA` (one or more printable
   * ASCII characters excluding comma and double-quote). Token classification is the lexer's only
   * responsibility; quote-doubling unescape logic and record/field structure are handled by the
   * grammar actions in `csv.y`, which call back into this class via `append()` and `newline()`.
   *
   * Instances are short-lived : `CSV::parse_helper()` constructs one `CSVLexer` per parse, binds it to
   * `CSVParser`, and discards it after extracting the result via `data()`.
   *
   * @see pacemaker::io::CSV
   * @note The generated `CSVParser` class (declared in the Bison-generated `bison-csv.hpp`, built from `csv.y`)
   *       is the sole caller of `yylex()`, `append()`, and `newline()`.
   */
  class CSVLexer : public yyFlexLexer
  {
  public:
    /**
     * @brief Constructs a lexer bound to the given input stream.
     *
     * Forwards @p in to the `yyFlexLexer` base constructor, which uses it as the source of characters
     * for tokenization.
     *
     * @param in Pointer to the input stream to read from. Must outlive the lexer; ownership is not transferred.
     */
    CSVLexer(std::istream *in) : yyFlexLexer(in) {}

    /// @brief Virtual destructor; no additional cleanup beyond the base class.
    virtual ~CSVLexer() = default;

    /// @brief Brings the base class's no-argument `yylex()` overload into scope alongside the typed
    ///        overload declared below.
    using FlexLexer::yylex;

    /**
     * @brief Scans the next token from the input stream.
     *
     * Implemented in the Flex-generated source produced from `csv.l`. For a matched `TEXTDATA` token,
     * the matched text is copied into `*yyval`; for all other token kinds (`CR`, `LF`, `COMMA`, `DQUOTE`)
     * `*yyval` is left unmodified, since the token kind alone is sufficient information for the grammar.
     *
     * This signature matches the `YY_DECL` macro defined in `csv.l` and is invoked by the generated `CSVParser`
     * as its `yylex` callback (see `%code { #define yylex lexer.yylex }` in `csv.y`).
     *
     * @param yyval Output parameter receiving the matched text for `TEXTDATA` tokens. Must not be null.
     *
     * @return The recognised token kind, as a `pacemaker::io::CSVParser::token` enumerator value, or `0` at end of input.
     */
    auto yylex(std::string *const yyval) -> int;

    /**
     * @brief Begins a new record by appending an empty row to the result.
     *
     * Called by the grammar action for the `records` rule in `csv.y` each time a complete record has been recognised,
     * including after the first record. As a side effect of `append()`'s lazy-initialisation check, the very first
     * record does not required an explicit `newline()` call before its first field is appended.
     */
    auto newline() -> void { this->m_content.emplace_back(std::vector<std::string>{}); }

    /**
     * @brief Appends a field value to the current (last) record.
     *
     * Called by the grammar action for the `record` rule in `csv.y` each time a `field` non-terminal is reduced.
     *
     * @param field The unescaped field value, as computed by the grammar's `escaped` or `nonescaped` rule.
     */
    auto append(const std::string &field) -> void { this->m_content.back().emplace_back(field); }

    /**
     * @brief Returns a copy of all records accumulated so far.
     *
     * @return A vector of records, each a vector of field strings, in the order they were parsed.
     */
    auto data() -> std::vector<std::vector<std::string>> { return this->m_content; }

    /**
     * @brief Clears all accumulated records.
     *
     * After calling `reset()`, `data()` returns an empty vector until new records are accumulated via
     * `append()` / `newline()`. Allows a single `CSVLexer` instance to be reused for multiple parses, although
     * the current `CSV::parse_helper()` implementation constructs a fresh lexer per call instead of reusing one.
     */
    auto reset() -> void { this->m_content.clear(); }

  private:
    /**
     * @brief Accumulated parse result: one entry per record, each a vector of field strings in column order.
     *
     * Populated exclusively through `newline()` and `append()`, both
     * invoked by the Bison grammar actions during `CSVParser::parse()`
     */
    std::vector<std::vector<std::string>> m_content;
  };
} // namespace pacemaker::io

#endif // PACEMAKER_IO_CSVLEXER_HPP_