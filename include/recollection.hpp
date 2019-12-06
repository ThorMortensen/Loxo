/**
 * @file recollection.hpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include <deque>
#include <list>
#include <map>
#include <string>
#include <vector>



namespace loxo {
class Recollection {
public:
  Recollection(const std::string &fileName);
  Recollection(Recollection &&) = default;
  Recollection(const Recollection &) = default;
  Recollection &operator=(Recollection &&) = default;
  Recollection &operator=(const Recollection &) = default;
  ~Recollection();

  void load();
  void store();
  void save(const std::string &str);

  std::string suggest(const std::string &suggestionSeed);
  std::string suggestNext(const std::string &suggestionSeed);
  std::string suggestPrev(const std::string &suggestionSeed);
  std::string recallNext();
  std::string recallPrev();

  void test();
  void dbgPrintContent();
  void dbgPrintAttr();

private:
  constexpr static int DBG_PEEK_SIZE = 15;

  struct mhist {
    std::map<std::string, std::list<mhist>::iterator>::iterator content;
  };

  typedef std::list<mhist>::const_iterator histIt_ct;
  typedef std::list<mhist>::iterator histIt_t;
  typedef std::map<std::string, std::list<mhist>::iterator>::iterator mapIt_t;

  enum class State {
    IN_BOUNDS,
    SEARCHING,
    HISTORY_CRAWL,
  };

  State state = State::SEARCHING;

  std::string fName;
  std::string folder;
  std::string absPath;
  std::list<mhist> history;
  std::map<std::string, histIt_t> data;
  mapIt_t dataIt;
  histIt_ct histIt;
  bool validDataIt = true;
  bool isLoaded = false;

  std::string suggestNextInBounds(const std::string &suggestionSeed,
                                  bool forward);
  bool findSuggestion(const std::string &suggestionSeed, bool useUpperBound);

  //--------- Debug stuff ------------
  bool dbgPrintIsLoaded = false;
  std::vector<std::string> dbgPrintVector;
  void loadDbgPrint();
  void incrDataIt();
  void decrDataIt();
};
} // namespace loxo
