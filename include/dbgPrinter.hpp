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
#include <syslog.h>
#include <unistd.h>
#include <vector>

#include "color.hpp"

namespace loxo {

#ifndef /*D*/ ISABLE_LXO_DBG_PRINT
template <class T> struct is_c_str : std::integral_constant<bool, false> {};

template <> struct is_c_str<char *> : std::integral_constant<bool, true> {};

template <> struct is_c_str<const char *> : std::integral_constant<bool, true> {};

template <typename T> struct is_string { static const bool value = false; };

template <class T, class Traits, class Alloc> struct is_string<std::basic_string<T, Traits, Alloc>> { static const bool value = true; };

#define LXO_NL std::cout << '\n';
#define MARKER                                                                                                                                                                     \
  std::cout << "\n\n"                                                                                                                                                              \
            << __FUNCTION__ << "(" << __LINE__ << ") "                                                                                                                             \
            << "@@@@@@!!!MARKER!!!--------!!!MARKER!!!@@@@@@\n\n";



class loxoLogger{
   enum LogPriority {
     kLogEmerg   = LOG_EMERG,   // system is unusable
     kLogAlert   = LOG_ALERT,   // action must be taken immediately
     kLogCrit    = LOG_CRIT,    // critical conditions
     kLogErr     = LOG_ERR,     // error conditions
     kLogWarning = LOG_WARNING, // warning conditions
     kLogNotice  = LOG_NOTICE,  // normal, but significant, condition
     kLogInfo    = LOG_INFO,    // informational message
     kLogDebug   = LOG_DEBUG    // debug-level message
   };

 public:
  explicit loxoLogger(std::ostream& os = std::cout) : os_(os){}
 private:

   template<typename T> friend std::ostream& operator<<(loxoLogger&, T);
   std::ostream& os_;
};
#define ENALBE_SYS_LOG
template<typename T>
std::ostream& operator<<(loxoLogger& log, T op) {
#ifdef ENALBE_SYS_LOG
  syslog(op)
#else

  log.os_ << op;
  return log.os_;
#endif

  log.os_ << op;
  return log.os_;
}

//template<typename T>
//std::ostream& operator<<(loxoLogger& log, T op, int logPrio) {
//  log.os_ << (op , logPrio);
//  return log.os_;
//}



loxoLogger lxoLogger;

//    constexpr auto loxoLogger(const char *fn, int32_t ln, const char *argStr, args al) {
//  if (is_c_str<args>::value || is_string<args>::value) {
//    std::clog << fn << "(" << ln << "): " << al << "\r\n";
//  } else {
//    std::clog << fn << "(" << ln << "): {" << argStr << "}-->[" << (al) << "]\r\n";
//  }
//  return (al);
//}


template <typename args> constexpr auto debugPrint(const char *fn, int32_t ln, const char *argStr, args al) {
  if (is_c_str<args>::value || is_string<args>::value) {
    lxoLogger << fn << "(" << ln << "): " << al << "\r\n";
  } else {
    lxoLogger << fn << "(" << ln << "): {" << argStr << "}-->[" << (al) << "]\r\n";
  }

  return (al);
}

template <typename args> constexpr auto debugPrintHex(const char *fn, int32_t ln, const char *argStr, args al) {
  if (is_c_str<args>::value || is_string<args>::value) {
    std::cout << fn << "(" << ln << "): " << al << "\r\n";
  } else {
    std::cout << fn << "(" << ln << "): {" << argStr << "}-->[";
    std::cout << "0x" << std::hex << static_cast<uint64_t>((al));
    std::cout << std::dec << "]\r\n";
  }
  return (al);
}

template <typename args> constexpr auto debugPrintArrHex(const char *fn, int32_t ln, const char *argStr, args al, size_t size) {
  std::cout << "=== " << fn << "(" << ln << ") START ===\r\n";
  for (size_t i = 0; i < size; ++i) {
    std::cout << " " << argStr << "[" << i << "] --> ["
              << "0x" << std::hex << static_cast<uint64_t>((al[i]));
    std::cout << std::dec << "]\r\n";
  }
  std::cout << "=== " << fn << "(" << ln << ") END ===\r\n";
  return (al);
}

#define DBP(...) debugPrint(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);
#define DBP_HEX(...) debugPrintHex(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);
#define DBP_ARR(arr, size) debugPrintArrHex(__FUNCTION__, __LINE__, #arr, arr, size);
#else
#define LXO_NL
#define MARKER
template <typename args> constexpr auto debugPrint(const char *fn, int32_t ln, const char *argStr, args al) {
  (void)fn;
  (void)ln;
  (void)argStr;
  return (al);
}
#define DBP(...) debugPrint(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);
#define DBP_HEX(...) debugPrint(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);
#define DBP_ARR(...) ddebugPrint(__FUNCTION__, __LINE__, #__VA_ARGS__, __VA_ARGS__);
#endif

#define LXO_PRINT_ERRNO std::cerr << "[" << __FILE__ << "] " << __FUNCTION__ << "(" << __LINE__ << "): Errno(" << errno << ")--> " << strerror(errno) << "\r\n";

#define LXO_DEFAULT_ERR_MSG                                                                                                                                                        \
  std::cerr << "Defaulted in \""                                                                                                                                                   \
               "["                                                                                                                                                                 \
            << __FILE__ << "] " << __FUNCTION__ << '(' << __LINE__ << ")\"!! \n";

template <typename T, typename... iteratorList> void ppVector(int32_t lines, const T &printebleVector, iteratorList... iterators) {

  int32_t colWidth = 15;
  int32_t dbgColSel = 0;
  int32_t vSize = static_cast<int32_t>(printebleVector.size());
  static const std::size_t itCount = sizeof...(iterators);
  std::string (*dbgColors[])(const std::string &str) = {Color::green, Color::blue, Color::magenta, Color::cyan};
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
        ssout << std::left << std::setw(colWidth - 3) << printebleVector[start + i];
        col[i] += Color::bold((*dbgColors[(dbgColSel++) % 4])("‣" + ssout.str().substr(0, colWidth - 1) + ' '));
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
