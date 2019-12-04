#include "Prompt.hpp"
#include "Manduca.hpp"
#include <functional>

namespace Manduca {

Prompt::Prompt(const std::string &nameHistDB) : recall(nameHistDB) {}

Prompt::Prompt() : recall("") {}

void Prompt::test() { recall.test(); }

void Prompt::moveCursor(KeyCode direction, std::string &inputStr,
                        const std::string_view &suggestion) {

  int32_t isl = static_cast<int32_t>(inputStr.length());
  if (isl == 0) {
    return;
  }

  switch (cursorState) {
  case CS::APPEND:
    if (direction == KeyCode::LEFT) {
      cursorOffset++;
      cursorState = (cursorOffset == isl - 1) ? CS::PREPEND : CS::INSERT;
    } else {
      if (suggestion.length() > inputStr.length()) {
        inputStr.push_back(suggestion.at(isl));
      }
    }
    break;
  case CS::PREPEND:
    if (direction == KeyCode::RIGHT) {
      cursorOffset--;
      cursorState = (cursorOffset == 0) ? CS::APPEND : CS::INSERT;
    }
    break;
  case CS::INSERT:

    if (direction == KeyCode::LEFT) {
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
    DEFAULT_ERR_MSG
    break;
  }
}

std::string Prompt::ask(const std::string &question,
                        const std::string &defaultAnsw,
                        std::function<bool(const std::string &answ)> validate) {
  bool done = false;
  KeyCode kIn = KeyCode::NOP;
  std::string suggestion = defaultAnsw;
  std::string ppStr;
  std::string inputStr;

  recall.load();
  //  recall.dbgPrintAttr();

  printAskPrompt(question, suggestion, inputStr);

  while (!done) {
    kIn = c.getKeyPress();

    switch (kIn) {
    case KeyCode::UP:
      suggestion = recall.suggestNext(inputStr);
      break;
    case KeyCode::DOWN:
      suggestion = recall.suggestPrev(inputStr);
      break;
    case KeyCode::RIGHT:
    case KeyCode::LEFT:
      moveCursor(kIn, inputStr, suggestion);
      break;
    case KeyCode::ENTER:
      if(!defaultAnsw.empty() && inputStr.empty()){
        inputStr = defaultAnsw;
        c.clearLine();
        printAskPrompt(question, suggestion, inputStr);
      }

      if (disableAutoSave){
        done = true;
      } else if (validate(inputStr)) {
        recall.save(inputStr);
        done = true;
      }
      break;
    case KeyCode::NOP:
      break;
    case KeyCode::HOME:
      cursorOffset = inputStr.length();
      break;
    case KeyCode::END:
      cursorOffset = 0;
      break;
    case KeyCode::DEL:
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
    case KeyCode::TAB:
      if (suggestion.length() > 0) {
        inputStr = suggestion;
        cursorOffset = 0;
      }
      break;
    case KeyCode::BACK_SPACE:
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
      }
      suggestion = recall.suggest(inputStr);
      break;
    }
//        NL;
    //    recall.dbgPrintContent();
    //    NL;
    c.clearLine();
    printAskPrompt(question, suggestion, inputStr);
  }

  std::cout << '\n';
  return inputStr;
}

void Prompt::printAskPrompt(const std::string &question, const std::string &suggestion, const std::string &inputStr){
  int32_t dif = suggestion.length() - inputStr.length();
  if (dif > 0) {
    std::string pp =
        inputStr +
        mDye::dim(mDye::gray(suggestion.substr(inputStr.length(), dif)));
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
  KeyCode kIn = KeyCode::NOP;

  std::cout << question << std::endl;
  c.caretShow(false);

  while (!done) {

    for (int32_t i = 0; i <= optCount; i++) {
      if (i == selection) {
        std::cout << mDye::green("‣ " + mDye::bold(options[i]));
      } else {
        std::cout << "  " << options[i];
      }
      if (i < optCount) {
        std::cout << "\n";
      }
    }

    kIn = c.getKeyPress();

    switch (kIn) {
    case KeyCode::UP:
      selection--;
      break;
    case KeyCode::DOWN:
      selection++;
      break;
    case KeyCode::ENTER:
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
             ([](const std::string &str) { return true; }));
}
std::string Prompt::ask(const std::string &question,
                        std::function<bool(const std::string &answ)> validate) {
  return ask(question, "", validate);
}
std::string Prompt::ask(const std::string &question) {
  return ask(question, "");
}

// void Prompt::save() { recall.save(lastInputStr); }
// void Prompt::save(const std::string &string) { recall.save(string); }

} // namespace Manduca