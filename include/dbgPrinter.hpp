/**
 * @file debugPrinter.cpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

#include "color.hpp"

namespace loxo {

template <typename args>
auto debugPrint(const char *fn, int32_t ln, const char *argStr, args al) {
  std::cout << fn << "(" << ln << "): {" << argStr << "}-->[" << (al) << "]\r\n";
  return (al);
}

#define NL std::cout << '\n';
#define MARKER std::cout << "\n\n" << __FUNCTION__ << "(" << __LINE__ << ") "<< "@@@@@@!!!MARKER!!!--------!!!MARKER!!!@@@@@@\n\n";
#define DBP(...) debugPrint(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);

#define DEFAULT_ERR_MSG std::cerr << "Defaulted in \"" << __FUNCTION__ << '(' <<__LINE__ << ")\"!! \n";

// https://en.wikipedia.org/wiki/Variadic_macro
// #define dbgprintf(...) realdbgprintf (__FILE__, __LINE__, __VA_ARGS__)

template <typename T, typename... iteratorList>
void ppMap(int32_t lines, const T &printebleVector,
              iteratorList... iterators) {

  int32_t colWidth = 15;
  int32_t dbgColSel = 0;
  int32_t vSize = static_cast<int32_t>(printebleVector.size());
  static const std::size_t itCount = sizeof...(iterators);
  std::string (*dbgColors[])(const std::string &str) = {
    Color::green, Color::blue, Color::magenta, Color::cyan};
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (vSize == 0) {
    std::cout << Color::brown("Map is empty...\n");
    return;
  }

  if (itCount > 1) {
    int32_t maxCol = (w.ws_col / itCount);
    if (colWidth > maxCol) {
      colWidth = maxCol - 1;
    }
  }

  if (lines <= 0) {
    lines = w.ws_row;
  }

  if (vSize < lines) {
    lines = vSize;
  }

  std::vector<std::string> col(lines, "");

  for (const auto it : {iterators...}) {
    std::stringstream ssout;
    int32_t itPos = it - printebleVector.begin();
    int32_t start = itPos - (lines / 2);
    int32_t end = itPos + (lines / 2);

    if (itPos < 0 || itPos > vSize) {
      ssout << std::left << std::setw(colWidth) << "Inv pos!";
      col[0] += Color::red(ssout.str());
      ssout.str("");
      ssout.clear();
      for (int32_t i = 1; i < lines; i++) {
        ssout << std::left << std::setw(colWidth) << itPos;
        col[i] += Color::red(ssout.str().substr(0, colWidth) + ' ');
      }
      continue;
    }

    if (start < 0) {
      end += start * (-1);
      start = 0;
    }

    if (end >= vSize) {
      start += vSize - end;
      end = vSize - 1;
    }

    int32_t digitLength = std::to_string(end).length();

    for (int32_t i = 0; i < lines; i++) {
      ssout.str("");
      ssout.clear();
      ssout << std::right << "[" << std::setw(digitLength) << start + i << "] ";

      if ((start + i) == itPos) {
        ssout << std::left << std::setw(colWidth - 3)
              << printebleVector[start + i];
        col[i] += Color::bold((*dbgColors[(dbgColSel++) % 4])(
            "‣" + ssout.str().substr(0, colWidth - 1) + ' '));
      } else {
        ssout << std::left << std::setw(colWidth) << printebleVector[start + i];
        col[i] += ssout.str().substr(0, colWidth) + ' ';
      }
    }
  }

  for (auto s : col) {
    std::cout << s << '\n';
  }
}

template <typename T, typename... iteratorList>
void ppVector(int32_t lines, const T &printebleVector,
              iteratorList... iterators) {

  int32_t colWidth = 15;
  int32_t dbgColSel = 0;
  int32_t vSize = static_cast<int32_t>(printebleVector.size());
  static const std::size_t itCount = sizeof...(iterators);
  std::string (*dbgColors[])(const std::string &str) = {
      Color::green, Color::blue, Color::magenta, Color::cyan};
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (vSize == 0) {
    std::cout << Color::brown("Vector is empty...\n");
    return;
  }

  if (itCount > 1) {
    int32_t maxCol = (w.ws_col / itCount);
    if (colWidth > maxCol) {
      colWidth = maxCol - 1;
    }
  }

  if (lines <= 0) {
    lines = w.ws_row;
  }

  if (vSize < lines) {
    lines = vSize;
  }

  std::vector<std::string> col(lines, "");

  for (const auto it : {iterators...}) {
    std::stringstream ssout;
    int32_t itPos = it - printebleVector.begin();
    int32_t start = itPos - (lines / 2);
    int32_t end = itPos + (lines / 2);

    if (itPos < 0 || itPos > vSize) {
      ssout << std::left << std::setw(colWidth) << "Inv pos!";
      col[0] += Color::red(ssout.str());
      ssout.str("");
      ssout.clear();
      for (int32_t i = 1; i < lines; i++) {
        ssout << std::left << std::setw(colWidth) << itPos;
        col[i] += Color::red(ssout.str().substr(0, colWidth) + ' ');
      }
      continue;
    }

    if (start < 0) {
      end += start * (-1);
      start = 0;
    }

    if (end >= vSize) {
      start += vSize - end;
      end = vSize - 1;
    }

    int32_t digitLength = std::to_string(end).length();

    for (int32_t i = 0; i < lines; i++) {
      ssout.str("");
      ssout.clear();
      ssout << std::right << "[" << std::setw(digitLength) << start + i << "] ";

      if ((start + i) == itPos) {
        ssout << std::left << std::setw(colWidth - 3)
              << printebleVector[start + i];
        col[i] += Color::bold((*dbgColors[(dbgColSel++) % 4])(
            "‣" + ssout.str().substr(0, colWidth - 1) + ' '));
      } else {
        ssout << std::left << std::setw(colWidth) << printebleVector[start + i];
        col[i] += ssout.str().substr(0, colWidth) + ' ';
      }
    }
  }

  for (auto s : col) {
    std::cout << s << '\n';
  }
}

} // namespace loxo
