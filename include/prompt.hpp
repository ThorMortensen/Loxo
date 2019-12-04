/**
 * @file prompt.hpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "curser.hpp"
#include "recollection.hpp"

namespace loxo {

class Prompt {
public:
  // Prompt() = default;
  Prompt(const std::string &nameHistDB);
  Prompt();
  Prompt(Prompt &&) = default;
  Prompt(const Prompt &) = default;
  Prompt &operator=(Prompt &&) = default;
  Prompt &operator=(const Prompt &) = default;
  ~Prompt() = default;

  int32_t choose(const std::string &question,
                 const std::vector<std::string> &options);

  std::string ask(const std::string &question, const std::string &defaultAnsw);
  std::string ask(const std::string &question);
  std::string ask(const std::string &question,
                  std::function<bool(const std::string &answ)> validate);
  std::string ask(const std::string &question,
      const std::string &defaultAnsw,
                  std::function<bool(const std::string &answ)> validate);

  bool disableAutoSave = false;

  void save(const std::string &string);
  void store(const std::string &string);
  void store();

  void test();

private:
  Curser c;
  Recollection recall;
  enum class CS { APPEND, INSERT, PREPEND };

  CS cursorState = CS::APPEND;

  int32_t cursorOffset = 0;

  void moveCursor(keyCode direction, std::string &inputStr,
                  const std::string_view &suggestion);
  void printAskPrompt(const std::string &question,
                      const std::string &suggestion,
                      const std::string &inputStr);
};

} // namespace loxo
