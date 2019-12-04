/**
 * @file Color.cpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <regex>

#include "Color.hpp"


namespace Manduca::Color
{
  
  std::string white(const std::string &str)          {return "\e[30m"+str+"\e[0m" ;}
  std::string black(const std::string &str)          {return "\e[30m"+str+"\e[0m" ;}
  std::string red(const std::string &str)            {return "\e[31m"+str+"\e[0m" ;}
  std::string green(const std::string &str)          {return "\e[32m"+str+"\e[0m" ;}
  std::string greenLight(const std::string &str)     {return "\e[92m"+str+"\e[0m" ;}
  std::string brown(const std::string &str)          {return "\e[33m"+str+"\e[0m" ;}
  std::string blue(const std::string &str)           {return "\e[34m"+str+"\e[0m" ;}
  std::string magenta(const std::string &str)        {return "\e[35m"+str+"\e[0m" ;}
  std::string cyan(const std::string &str)           {return "\e[36m"+str+"\e[0m" ;}
  std::string gray(const std::string &str)           {return "\e[37m"+str+"\e[0m" ;}
  std::string yellow(const std::string &str)         {return "\e[43m"+str+"\e[0m" ;}

  std::string bg_black(const std::string &str)       {return "\e[40m"+str+"\e[0m" ;}
  std::string bg_red(const std::string &str)         {return "\e[41m"+str+"\e[0m" ;}
  std::string bg_green(const std::string &str)       {return "\e[42m"+str+"\e[0m" ;}
  std::string bg_yell(const std::string &str)        {return "\e[103m"+str+"\e[0m";}
  std::string bg_brown(const std::string &str)       {return "\e[43m"+str+"\e[0m" ;}
  std::string bg_blue(const std::string &str)        {return "\e[44m"+str+"\e[0m" ;}
  std::string bg_magenta(const std::string &str)     {return "\e[45m"+str+"\e[0m" ;}
  std::string bg_cyan(const std::string &str)        {return "\e[46m"+str+"\e[0m" ;}
  std::string bg_gray(const std::string &str)        {return "\e[47m"+str+"\e[0m" ;}

  std::string bold(const std::string &str)           {return "\e[1m"+str+"\e[0m" ;}
  std::string dim(const std::string &str)            {return "\e[2m"+str+"\e[22m" ;}
  std::string italic(const std::string &str)         {return "\e[3m"+str+"\e[23m" ;}
  std::string underline(const std::string &str)      {return "\e[4m"+str+"\e[24m" ;}
  std::string blink(const std::string &str)          {return "\e[5m"+str+"\e[25m" ;}
  std::string reverse_color(const std::string &str)  {return "\e[7m"+str+"\e[27m" ;}

  

  std::string removeColor(const std::string &str){
  //  std::regex ("a|e|i|o|u");
 
   // write the results to an output iterator
  //  std::regex_replace(std::ostreambuf_iterator<char>(std::cout),
                      // text.begin(), text.end(), vowel_re, "*");
 
   // construct a string holding the results
      //  gsub(/(#{"\e"}\[\d*m)/n, '')

    return std::regex_replace(str, std::regex("(\e\\[\\d*m)"), "");

  }
    

} // namespace Manduca



  
