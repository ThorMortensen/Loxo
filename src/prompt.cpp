/**
 * @file prompt.cpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#include "loxo.hpp"
#include <functional>

namespace loxo {

Prompt::Prompt(const std::string &nameHistDB) : recall(nameHistDB) {}

Prompt::Prompt() : recall("") {}

void Prompt::test() { recall.test(); }

void Prompt::moveCursor(keyCode direction, std::string &inputStr,
                        const std::string_view &suggestion) {

  int32_t isl = static_cast<int32_t>(inputStr.length());
  if (isl == 0) {
    cursorState = CS::APPEND;
  }

  switch (cursorState) {
  case CS::APPEND:
    if (direction == keyCode::LEFT) {
      if (isl == 0) {
        return;
      }
      cursorOffset++;
      cursorState = (cursorOffset == isl) ? CS::PREPEND : CS::INSERT;
    } else {
      if (suggestion.length() > inputStr.length()) {
        inputStr.push_back(suggestion.at(isl));
      }
    }
    break;
  case CS::PREPEND:
    if (direction == keyCode::RIGHT) {
      cursorOffset--;
      cursorState = (cursorOffset == 0) ? CS::APPEND : CS::INSERT;
    }
    break;
  case CS::INSERT:

    if (direction == keyCode::LEFT) {
      cursorOffset++;
      if (cursorOffset == isl) {
        cursorState = CS::PREPEND;
      }
    } else {
      cursorOffset--;
      if (cursorOffset == 0) {
        cursorState = CS::APPEND;
      }
    }
    break;
  default:
    LXO_DEFAULT_ERR_MSG
    break;
  }
  LXO_NL DBP(static_cast<int>(cursorState)) DBP(isl) DBP(cursorOffset)
}

std::string Prompt::ask(const std::string &question,
                        const std::string &defaultAnsw,
                        std::function<bool(const std::string &answ)> validate) {
  bool done = false;
  keyCode kIn = keyCode::NOP;
  std::string suggestion = defaultAnsw;
  std::string ppStr;
  std::string inputStr;
  cursorOffset = 0;

  recall.load();
  //  recall.dbgPrintAttr();

  if (useLastAsDefault) {
    suggestion = recall.suggestNext("");
  }

  printAskPrompt(question, suggestion, inputStr);

  while (!done) {
    kIn = c.getKeyPress();

    switch (kIn) {
    case keyCode::UP:
      suggestion = recall.suggestNext(inputStr);
      break;
    case keyCode::DOWN:
      suggestion = recall.suggestPrev(inputStr);
      break;
    case keyCode::RIGHT:
    case keyCode::LEFT:
      //      DBP(suggestion);
      moveCursor(kIn, inputStr, suggestion);
      break;
    case keyCode::ENTER:
      if (!suggestion.empty() && inputStr.empty()) {
        inputStr = suggestion;
        c.clearLine();
        printAskPrompt(question, suggestion, inputStr);
      }

      if (disableAutoSave) {
        done = true;
      } else if (validate(inputStr)) {
        recall.save(inputStr);
        done = true;
      }
      break;
    case keyCode::NOP:
      break;
    case keyCode::HOME:
      cursorOffset = inputStr.length();
      break;
    case keyCode::END:
      cursorOffset = 0;
      break;
    case keyCode::DEL:
      if (!inputStr.empty() && cursorState != CS::APPEND) {
        if (cursorState == CS::INSERT) {
          inputStr.erase(inputStr.length() - cursorOffset,
                         1); // remove 1 char
          if (cursorOffset == 0) {
            cursorState = CS::APPEND;
          }
        } else {
          inputStr.erase(0, 1);
        }
        cursorOffset--;
        suggestion = recall.suggest(inputStr);
      }
      break;
    case keyCode::TAB:
      if (suggestion.length() > 0) {
        inputStr = suggestion;
        cursorOffset = 0;
      }
      break;
    case keyCode::BACK_SPACE:
      if (!inputStr.empty() && cursorState != CS::PREPEND) {
        if (cursorState == CS::INSERT) {
          inputStr.erase(inputStr.length() - cursorOffset - 1,
                         1); // remove 1 char
          if (cursorOffset == static_cast<int32_t>(inputStr.length())) {
            cursorState = CS::PREPEND;
          }
        } else {
          inputStr.pop_back();
        }
        suggestion = recall.suggest(inputStr);
      }
      break;
    default:
      char ch = static_cast<char>(kIn);
      if (cursorState == CS::APPEND) {
        inputStr.push_back(ch);
      } else {
        inputStr.insert(inputStr.length() - cursorOffset, 1,
                        ch); // 1 copy of ch
        cursorState = CS::INSERT;
      }
      suggestion = recall.suggest(inputStr);
      break;
    }
    c.clearLine();
    printAskPrompt(question, suggestion, inputStr);
  }

  std::cout << '\n';
  return inputStr;
}

void Prompt::printAskPrompt(const std::string &question,
                            const std::string &suggestion,
                            const std::string &inputStr) {
  int32_t dif = suggestion.length() - inputStr.length();
  if (dif > 0) {
    std::string pp =
        inputStr +
        lDye::dim(lDye::gray(suggestion.substr(inputStr.length(), dif)));
    std::cout << question << pp;
  } else {
    dif = 0;
    std::cout << question << inputStr;
  }
  c.move(Curser::Direction_e::LEFT, dif + cursorOffset);
}

int32_t Prompt::choose(const std::string &question,
                       const std::vector<std::string> &options) {
  int32_t optCount = static_cast<int>(options.size()) - 1;
  int32_t selection = 0;
  bool done = false;
  keyCode kIn = keyCode::NOP;

  std::cout << question << std::endl;
  c.caretShow(false);

  while (!done) {

    for (int32_t i = 0; i <= optCount; i++) {
      if (i == selection) {
        std::cout << lDye::green("‣ " + lDye::bold(options[i]));
      } else {
        std::cout << "  " << options[i];
      }
      if (i < optCount) {
        std::cout << "\n";
      }
    }

    kIn = c.getKeyPress();

    switch (kIn) {
    case keyCode::UP:
      selection--;
      break;
    case keyCode::DOWN:
      selection++;
      break;
    case keyCode::ENTER:
      done = true;
      break;
    default:
      break;
    }

    if (selection > optCount) {
      selection = optCount;
    } else if (selection < 0) {
      selection = 0;
    }

    c.jumpLinesUp(optCount);
    c.flush();
  }

  c.caretShow(true);

  return selection;
}
void Prompt::save(const std::string &string) {
  recall.load();
  recall.save(string);
}

std::string Prompt::ask(const std::string &question,
                        const std::string &defaultAnsw) {
  return ask(question, defaultAnsw,
             ([](const std::string &str) { (void) str; return true; }));
}
std::string Prompt::ask(const std::string &question,
                        std::function<bool(const std::string &answ)> validate) {
  return ask(question, "", validate);
}
std::string Prompt::ask(const std::string &question) {
  return ask(question, "");
}
void Prompt::store(const std::string &string) {
  save(string);
  store();
}
void Prompt::store() { recall.store(); }

// void Prompt::save() { recall.save(lastInputStr); }
// void Prompt::save(const std::string &string) { recall.save(string); }

} // namespace loxo