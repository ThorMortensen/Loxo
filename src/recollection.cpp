/**
 * @file recollection.cpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#include <algorithm>
#include <cstdlib> /* getenv */
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "dbgPrinter.hpp"
#include "recollection.hpp"

// ============ Fix for filesystem include ==============
#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#endif
// =======================================================

namespace loxo {

Recollection::Recollection(const std::string &fileName) : fileName(fileName) {
  std::string home(getenv("HOME"));
  folder = home + "/.loxo-history";
  absPath = folder + "/" + fileName;
}

std::string Recollection::suggestNextInBounds(const std::string &suggestionSeed,
                                              bool forward) {
  if (data.empty()){
    return "";
  }

  if (suggestionSeed.empty()) {
    return forward ? recallNext() : recallPrev();
  }

  state = State::IN_BOUNDS;

  forward ? dataIt++ : dataIt--;

  if (dataIt->first.rfind(suggestionSeed, 1) == std::string::npos) {
    if (!forward) {
      // Add 255 char to find last occurrence of seed.
      findSuggestion(suggestionSeed + static_cast<char>(255), false);
      dataIt--;
      validDataIt = true;
    } else {
      findSuggestion(suggestionSeed, false);
    }
  }

  return validDataIt ? dataIt->first : "";
}

std::string Recollection::suggestNext(const std::string &suggestionSeed) {
  return suggestNextInBounds(suggestionSeed, true);
}
std::string Recollection::suggestPrev(const std::string &suggestionSeed) {
  return suggestNextInBounds(suggestionSeed, false);
}

std::string Recollection::recallNext() {
  if (data.empty()){
    return "";
  }

  if (state != State::HISTORY_CRAWL) {
    histIt = history.cbegin();
  } else {
    histIt++;
    if (histIt == history.cend()) {
      histIt = history.cbegin();
    }
  }
  dataIt = histIt->content;
  state = State::HISTORY_CRAWL;

  return dataIt->first;
}
std::string Recollection::recallPrev() {

  if (data.empty()){
    return "";
  }

  if (state != State::HISTORY_CRAWL) {
    histIt = std::prev(history.cend());
  } else {
    if (histIt == history.cbegin()) {
      histIt = history.cend();
    }
    histIt--;
  }
  dataIt = histIt->content;
  state = State::HISTORY_CRAWL;

  return dataIt->first;
}

bool Recollection::findSuggestion(const std::string &suggestionSeed,
                                  bool useUpperBound) {
  validDataIt = false;

  if (suggestionSeed.empty() || data.empty()) {
    return validDataIt;
  }

  dataIt = useUpperBound ? data.upper_bound(suggestionSeed)
                         : data.lower_bound(suggestionSeed);

  if ((dataIt->first).length() < suggestionSeed.length()) {
    return validDataIt;
  }

  if ((dataIt->first).compare(0, suggestionSeed.length(), suggestionSeed) ==
      0) {
    validDataIt = true;
  }

  return validDataIt;
}

std::string Recollection::suggest(const std::string &suggestionSeed) {
  state = State::SEARCHING;
  return findSuggestion(suggestionSeed, false) ? dataIt->first : "";
}

void Recollection::load() {

  if (isLoaded) {
    state = State::SEARCHING;
    dataIt = data.begin();
    return;
  }

  std::ifstream df(absPath, std::ifstream::in);
  std::string line;

  if (df.good()) {
    while (std::getline(df, line)) {
      history.emplace_back(mhist());
      auto h = std::prev(history.end());
      auto d = data.insert(std::make_pair(std::move(line), h));
      d.first->second->content = d.first;
    }
  }

  dataIt = data.begin();
  isLoaded = true;
}
void Recollection::save(const std::string &str) {

  if (str.empty()) {
    return;
  }

  auto d = data.insert(std::make_pair(str, history.end()));

  if (d.first->second != history.end()) {
    history.erase(d.first->second);
  }

  history.emplace_front(mhist());
  auto h = history.begin();
  h->content = d.first;
  d.first->second = h;

  state = State::SEARCHING;
  dataIt = data.begin();
  dbgPrintIsLoaded = false;
}

void Recollection::store() {

  if (!fs::is_directory(folder)) {
    fs::create_directory(folder);
  }

  std::ofstream df(absPath);

  if (!df.good()) {
    std::cerr << "Open output file's at " << absPath << " failed!!\n";
    return;
  }

  for (auto h : history) {
    df << h.content->first << '\n';
  }
}

Recollection::~Recollection() { store(); }

// ------------------------ Debug stuff -------------------------

void Recollection::dbgPrintContent() {
  DBP(dbgPrintVector.size())
  if (data.empty()) {
    std::cout << "Data is empty! " << '\n';
    return;
  }

  if (!dbgPrintIsLoaded){
    loadDbgPrint();
  }

  auto d = std::upper_bound(dbgPrintVector.begin(), dbgPrintVector.end(),
                            dataIt->first);
  std::cout << "d --> " << dataIt->first << '\n';
  ppVector<std::vector<std::string>>(DBG_PEEK_SIZE, dbgPrintVector, --d);
}

void Recollection::dbgPrintAttr() {
  std::cout << "fileName: " << fileName << std::endl;
  std::cout << "folder: " << folder << std::endl;
  std::cout << "absPath: " << absPath << std::endl;
}
void Recollection::test() {
  load();
  dbgPrintAttr();
  dbgPrintContent();
  store();
}
void Recollection::loadDbgPrint() {
  dbgPrintVector.clear();
  for (const auto &st : data) {
    dbgPrintVector.emplace_back(st.first);
  }
  dbgPrintIsLoaded = true;
}

} // namespace loxo
