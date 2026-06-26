%require "3.2"
%language "c++"

%code requires {
#include <iostream>
#include <string>
#include "pacemaker/io/CSVLexer.hpp"
}

%define api.namespace { pacemaker::io }
%define api.value.type { std::string }
%define api.parser.class { CSVParser }
%parse-param { CSVLexer &lexer }

%header

%code {
  #define yylex lexer.yylex
}

%token TEXTDATA CR LF CRLF COMMA DQUOTE

%%

file:
  records opt_crlf
;

crlf:
  CRLF | LF | CR
;

opt_crlf:
    %empty
  | crlf
;

records:
    record
  | records crlf record
;

record:
    field              { lexer.newline(); lexer.append($field); }
  | record COMMA field { lexer.append($field); }
;

field:
  escaped | nonescaped 
;

escaped:
  DQUOTE escaped_content DQUOTE { $$ = $escaped_content; }
;

escaped_content:
    %empty                        { $$  = ""; }
  | escaped_content TEXTDATA      { $$ += $2; }
  | escaped_content CRLF          { $$ += "\r\n"; }
  | escaped_content CR            { $$ += '\r'; }
  | escaped_content LF            { $$ += '\n'; }
  | escaped_content COMMA         { $$ += ','; }
  | escaped_content DQUOTE DQUOTE { $$ += '"'; }
;

nonescaped:
    %empty   { $$ = ""; }
  | TEXTDATA { $$ = $1; }
;

%%

void pacemaker::io::CSVParser::error(const std::string& message)
{
  throw std::runtime_error(message);
}