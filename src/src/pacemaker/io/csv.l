%top{
  #include <cstdint>
}


%{
#include "pacemaker/io/CSVLexer.hpp"
#include "bison-csv.hpp"

#undef YY_DECL
#define YY_DECL int pacemaker::io::CSVLexer::yylex(std::string* const yylval)
%}

%option c++ noyywrap
%option yyclass="pacemaker::io::CSVLexer"

CR       \r
LF       \n
CRLF     \r\n
COMMA    ,
DQUOTE   \"
TEXTDATA ([\x20-\x21\x23-\x2B\x2D-\x7E])+

%%

{CR}       { return pacemaker::io::CSVParser::token::CR;       }
{LF}       { return pacemaker::io::CSVParser::token::LF;       }
{CRLF}     { return pacemaker::io::CSVParser::token::CRLF;     }
{COMMA}    { return pacemaker::io::CSVParser::token::COMMA;    }
{DQUOTE}   { return pacemaker::io::CSVParser::token::DQUOTE;   }
{TEXTDATA} { *yylval = std::string(yytext, yytext + yyleng); return pacemaker::io::CSVParser::token::TEXTDATA; }

%%