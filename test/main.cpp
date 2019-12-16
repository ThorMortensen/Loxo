
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "curser.hpp"
#include "loxo.hpp"
#include "recollection.hpp"

using namespace loxo;

void testCurserMovement() {
  loxo::Curser c;
  std::cout << "hello world";
  c.clearLine();
  std::cout << "987654321";
  c.move(loxo::Curser::Direction_e::LEFT, 6);
  std::string foo;
  std::cin >> foo;

  return;

  for (int i = 0; i < 10; i++) {
    // c.print(std::to_string(i) + "\n");
    std::cout << std::to_string(i) << std::endl;
  }
  // c.flush();

  sleep(3);

  c.jumpLinesUp(5);
  c.print("this is 5 up");

  c.jumpLinesDown(5);
  c.print("this is 5 DOWN");
  c.print("\n");

  c.flush();
}

void testPromptChoose() {
  std::vector<std::string> opt = {"one", "two", "three", "four"};
  loxo::Prompt prompt;

  int32_t answ = prompt.choose("Choose one:", opt);

  std::cout << "You chose number [" << answ << "]" << std::endl;
  std::cout << "You chose opt [" << opt[answ] << "]" << std::endl;
}

void keyCodeDbg() {
  loxo::Curser c;
  c.printDbgKeyPress();
}

void stringTests() {
  std::string ts;

  std::cout << ts.capacity() << std::endl;
  ts.clear();
  ts += 'c';
  std::cout << ts.capacity() << std::endl;
}

void recollectionTests() {
  loxo::Recollection r("loxo-10000-test-sorted");
  r.test();
}

void testPromptAsk() {
  //    loxo::Prompt p("cpploxoTest");
  loxo::Prompt p("cppDojoTest");

  while (true) {
    //    auto a = p.ask("Enter something ~> ", "foo is best!",
    //                   [](const std::string &str) {
    //      if(str != "ok"){
    //        std::cout << lDye::red("\nWrong input please try again!") << '\n';
    //        return false;
    //      }
    //      return true;
    //    });
    auto a = p.ask("UART msg ~> ");
    std::cout << "You msg --> " << a;
    LXO_NL
    std::cout << "Done!";
    p.store();
    LXO_NL;
    if (a == "q") {
      break;
    }
  }
}

void testDebugPrinter() {
  std::vector<std::string> sv{
      "zero",  "one",   "two",  "three", "four",    "five",   "six",
      "seven", "eight", "nine", "ten",   "elleven", "twelve", "theteen",
  };

  std::vector<int32_t> iv{
      12331, 234,  6546, 6456,  3232,   534366, 4,  5564,
      234,   5435, 90,   42311, 123167, 987,    44, 7775,
  };

  auto it = sv.begin() + 5;
  auto it2 = sv.begin() + 6;
  auto it3 = sv.begin() + 10;
  auto it4 = sv.begin() + 0;
  auto it5 = sv.begin() + 3;

  loxo::ppVector<std::vector<std::string>>(4, sv, it, it2, it3, it4, it5);
}

void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";
  // cleanup and close up stuff here
  // terminate program
  exit(signum);
}

void signalHandlerTest() { signal(SIGINT, signalHandler); }

int main(/* int argc, char const *argv[] */) {

  // testDebugPrinter();

  testPromptAsk();
  // testCurserMovement();

  // std::cout << "prompt test starts..." << std::endl;
  // while(true);
  // keyCodeDbg();
  // testPromptChoose();
  // stringTests();
  //   recollectionTests();
  //  signalHandlerTest();
  // std::cout << "prompt test ends..." << std::endl;

  return 0;
}
