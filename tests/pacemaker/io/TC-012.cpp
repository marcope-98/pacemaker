#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <type_traits>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/io/CSV.hpp"

PACEMAKER_FIXTURE_INIT(TC012)

namespace
{
  struct FilesystemHelper
  {
    FilesystemHelper(const FilesystemHelper &)            = delete;
    FilesystemHelper(FilesystemHelper &&)                 = delete;
    FilesystemHelper &operator=(const FilesystemHelper &) = delete;
    FilesystemHelper &operator=(FilesystemHelper &&)      = delete;

    FilesystemHelper() : file{std::filesystem::temp_directory_path() / "file.csv"} {}

    void write(const std::string &content)
    {
      std::ofstream ofs(this->file, std::ios::binary | std::ios::trunc);
      ofs << content;
      ofs.close();
    }

    ~FilesystemHelper()
    {
      std::error_code ec{};
      std::filesystem::remove(this->file, ec);
    }

    std::filesystem::path file;
  };
} // namespace

TEST_F(TC012, A)
{
  EXPECT_THROW(pacemaker::io::CSV::parse(""), std::invalid_argument);
}

TEST_F(TC012, B)
{
  EXPECT_TRUE(std::filesystem::exists(std::filesystem::temp_directory_path()));
  EXPECT_THROW(pacemaker::io::CSV::parse(std::filesystem::temp_directory_path()), std::invalid_argument);
}

TEST_F(TC012, C)
{
  FilesystemHelper fh;
  fh.write("a\rb\nc\r\nd");
  auto csv      = pacemaker::io::CSV::parse(fh.file);
  auto expected = std::vector<std::vector<std::string>>{{"a"}, {"b"}, {"c"}, {"d"}};
  EXPECT_EQ(csv, expected);
}

TEST_F(TC012, D)
{
  FilesystemHelper fh;
  fh.write("a,b");
  auto csv      = pacemaker::io::CSV::parse(fh.file);
  auto expected = std::vector<std::vector<std::string>>{{"a", "b"}};
  EXPECT_EQ(csv, expected);
}

TEST_F(TC012, E)
{
  FilesystemHelper fh;
  fh.write(",,,");
  auto csv      = pacemaker::io::CSV::parse(fh.file);
  auto expected = std::vector<std::vector<std::string>>{{"", "", "", ""}};
  EXPECT_EQ(csv, expected);
}

TEST_F(TC012, F)
{
  FilesystemHelper fh;
  fh.write("0,1,2\n3,4,5");
  auto csv      = pacemaker::io::CSV::parse(fh.file);
  auto expected = std::vector<std::vector<std::string>>{{"0", "1", "2"}, {"3", "4", "5"}};
  EXPECT_EQ(csv, expected);
}

TEST_F(TC012, G)
{
  FilesystemHelper fh;
  {
    fh.write("");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\r\n\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}, {""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, H)
{
  FilesystemHelper fh;
  {
    fh.write("hello");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("hello\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"hello\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"hello\"\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, I)
{
  FilesystemHelper fh;
  {
    fh.write("a,b,c");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,c\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write(",b,c");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "b", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", ""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,,c");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write(",,");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "", ""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write(",");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", ""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,c,d,e,f,g,h,i,j");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, J)
{
  FilesystemHelper fh;
  {
    fh.write("a,b\r\nc,d");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b"}, {"c", "d"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a,b\r\nc,d\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b"}, {"c", "d"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a\r\nb\r\nc");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a"}, {"b"}, {"c"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a\r\nb\r\nc\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a"}, {"b"}, {"c"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, K)
{
  FilesystemHelper fh;
  {
    fh.write("a\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a,b\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a\r\nb\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a"}, {"b"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("a\r\nb\r\nc\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a"}, {"b"}, {"c"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\"x\"\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"x"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, L)
{
  FilesystemHelper fh;
  {
    fh.write("\x20");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x20"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\x21");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x21"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\x23");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x23"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\x2B");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x2B"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\x2D");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x2D"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\x7E");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\x7E"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("#$%&'()*+");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"#$%&'()*+"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{}");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{}"}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("   string with leading spaces");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"   string with leading spaces"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("hello world");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello world"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, M)
{
  FilesystemHelper fh;
  {
    fh.write("\"hello\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\" \"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{" "}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\" leading\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{" leading"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"trailing \"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"trailing "}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a\",\"b\",\"c\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, N)
{
  FilesystemHelper fh;
  {
    fh.write("\"a,b\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a,b"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a,b,c,d\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a,b,c,d"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\",value\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{",value"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"value,\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"value,"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\",\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{","}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("x,\"a,b\",z");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"x", "a,b", "z"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a,b\",\"c,d\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a,b", "c,d"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, O)
{
  FilesystemHelper fh;
  {
    fh.write("\"line1\nline2\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"line1\nline2"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"line1\rline2\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"line1\rline2"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"line1\r\nline2\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"line1\r\nline2"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a\r\nb\r\nc\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a\r\nb\r\nc"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("before,\"has\r\nnewline\",after");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"before", "has\r\nnewline", "after"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("r1\r\n\"r2\r\nstill_r2\"\r\nr3");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"r1"}, {"r2\r\nstill_r2"}, {"r3"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, P)
{
  FilesystemHelper fh;
  {
    fh.write("\"\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"\"\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\"\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"\"\"\"\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\"\"\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"\"hello\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"\"hello"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"hello\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"hello\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"say \"\"hi\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"say \"hi\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"he said, \"\"hello\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"he said, \"hello\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a\"\"b\"\"c\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a\"b\"c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\"\"");
    EXPECT_THROW(auto csv = pacemaker::io::CSV::parse(fh.file), std::runtime_error);
  }
  {
    fh.write("\"line1\r\n\"\"quoted\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"line1\r\n\"quoted\""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, Q)
{
  FilesystemHelper fh;
  {
    fh.write("\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\",b,c");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "b", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,\"\",c");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "", "c"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", ""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\",\"\",\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "", ""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, R)
{
  FilesystemHelper fh;
  {
    fh.write("\"a,\"\"b\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a,\"b\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"a\r\n\"\"b\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a\r\n\"b\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\",\r\n\"\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{",\r\n\""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\",\"a,b\",\"c\"\"d\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "a,b", "c\"d"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, S)
{
  FilesystemHelper fh;
  fh.write(
      "Year,Make,Model,Description,Price\r\n"
      "1997,Ford,E350,\"ac, abs, moon\",3000.00\r\n"
      "1999,Chevy,\"Venture \"\"Extended Edition\"\"\",\"\",4900.00\r\n"
      "2000,Ford,E350,\"super, luxurious truck\",5000.00");
  auto csv      = pacemaker::io::CSV::parse(fh.file);
  auto expected = std::vector<std::vector<std::string>>{
      {"Year", "Make", "Model", "Description", "Price"},
      {"1997", "Ford", "E350", "ac, abs, moon", "3000.00"},
      {"1999", "Chevy", "Venture \"Extended Edition\"", "", "4900.00"},
      {"2000", "Ford", "E350", "super, luxurious truck", "5000.00"}};
  EXPECT_EQ(csv, expected);
}

TEST_F(TC012, T)
{
  FilesystemHelper fh;
  {
    fh.write("plain1,\"quoted\",plain2");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"plain1", "quoted", "plain2"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,\"b\"\r\n\"c\",d");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b"}, {"c", "d"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write(",\"x\",");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "x", ""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\"\",plain,\"\"");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"", "plain", ""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, U)
{
  FilesystemHelper fh;
  {
    fh.write("a,b\r\nc,d,e,f");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b"}, {"c", "d", "e", "f"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,c,d\r\ne,f");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c", "d"}, {"e", "f"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("a,b,c\r\nx\r\nd,e,f");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"a", "b", "c"}, {"x"}, {"d", "e", "f"}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, V)
{
  FilesystemHelper fh;
  {
    fh.write("alpha\r\nbeta\r\ngamma");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"alpha"}, {"beta"}, {"gamma"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("alpha\r\nbeta\r\ngamma\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{"alpha"}, {"beta"}, {"gamma"}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\r\n");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
}

TEST_F(TC012, W)
{
  FilesystemHelper fh;
  {
    fh.write("\"unterminated");
    EXPECT_THROW(auto csv = pacemaker::io::CSV::parse(fh.file), std::runtime_error);
  }
  {
    fh.write("hel\"lo");
    EXPECT_THROW(auto csv = pacemaker::io::CSV::parse(fh.file), std::runtime_error);
  }
  {
    fh.write("\x01");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
  {
    fh.write("\x7F");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }

  {
    fh.write("\x80");
    auto csv      = pacemaker::io::CSV::parse(fh.file);
    auto expected = std::vector<std::vector<std::string>>{{""}};
    EXPECT_EQ(csv, expected);
  }
}