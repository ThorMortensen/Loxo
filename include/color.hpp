/**
 * @file Color.hpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once

#include <string>


// Use mDye alias in Manduca.hpp
namespace Manduca::Color {

std::string white(const std::string &str);
std::string black(const std::string &str);
std::string red(const std::string &str);
std::string green(const std::string &str);
std::string greenLight(const std::string &str);
std::string brown(const std::string &str);
std::string blue(const std::string &str);
std::string magenta(const std::string &str);
std::string cyan(const std::string &str);
std::string gray(const std::string &str);
std::string yellow(const std::string &str);
std::string bg_black(const std::string &str);
std::string bg_red(const std::string &str);
std::string bg_green(const std::string &str);
std::string bg_yell(const std::string &str);
std::string bg_brown(const std::string &str);
std::string bg_blue(const std::string &str);
std::string bg_magenta(const std::string &str);
std::string bg_cyan(const std::string &str);
std::string bg_gray(const std::string &str);
std::string bold(const std::string &str);
std::string dim(const std::string &str);
std::string italic(const std::string &str);
std::string underline(const std::string &str);
std::string blink(const std::string &str);
std::string reverse_color(const std::string &str);

std::string removeColor(const std::string &str);




} // namespace Manduca::Color
