#define WIN32_LEAN_AND_MEAN
#include <objbase.h>
#include <windows.h>

#include <iostream>

#include "inca/Session.hpp"

int main(void)
{
  CoInitialize(NULL);
  try
  {
    auto  session = inca::Session::connect();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }

  CoUninitialize();

  return 0;
}