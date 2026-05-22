#ifndef DATASETS_H
#define DATASETS_H
#include "BSTree.h"
#include "Vector.h"
#include "Functions.h"
#include <iostream>

namespace khairullin {
  struct Datasets {
    Vector< std::pair<std::string, BSTree< size_t, std::string, Compare< size_t > > * > > vectorOfDatasets;
    using func_t = void(Datasets::*)(std::string & line);
    BSTree< std::string, func_t, Compare< std::string > > * commands;

    Datasets();
    ~Datasets() = default;

    void function(std::string & line);

    void addDataset(std::istream & file);
    void addDataset(std::string name, BSTree< size_t, std::string, Compare< size_t > > * tree);
    std::pair< bool, size_t > hasDataset(std::string name);

    void print(std::string & line);
    void complement(std::string & line);
    void intersect(std::string & line);
    void union_set(std::string & line);

  };
}

khairullin::Datasets::Datasets():
vectorOfDatasets(Vector< std::pair< std::string, BSTree< size_t, std::string, Compare< size_t > > * > >()),
commands(new BSTree< std::string, func_t, Compare< std::string > >())
{
  commands->push("print", &Datasets::print);
  commands->push("complement", &Datasets::complement);
  commands->push("intersect", &Datasets::intersect);
  commands->push("union", &Datasets::union_set);
}

void khairullin::Datasets::function(std::string & line)
{
  std::string functionName = "";
  functionName = getToken(line);
  func_t function = nullptr;
  try {
    function = commands->get(functionName);
  }
  catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  (this->*function)(line);
}

void khairullin::Datasets::addDataset(std::istream & file)
{
  std::string line = "";
  std::getline(file, line);
  if (line.empty()) {
    return;
  }
  std::string name = getToken(line);
  BSTree< size_t, std::string, Compare< size_t > > * tree = nullptr;
  std::string str_key = getToken(line);
  std::string value = getToken(line);
  if (str_key == "" || value == "") {
    vectorOfDatasets.pushBack(std::make_pair(name, tree));
    return;
  }
  size_t key = 0;
  try {
    key = std::stoi(str_key);
  }
  catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try {
    tree = new BSTree< size_t, std::string, Compare< size_t > >(key, value, nullptr);
  }
  catch (...) {
    throw std::bad_alloc();
  }
  while (line != "") {
    str_key = getToken(line);
    value = getToken(line);
    if (str_key == "" || value == "") {
      throw std::logic_error("<INVALID COMMAND>");
    }
    try {
      key = std::stoi(str_key);
      tree->push(key, value);
    }
    catch (...) {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  vectorOfDatasets.pushBack(std::make_pair(name, tree));
}

void khairullin::Datasets::addDataset(std::string name,
  BSTree< size_t, std::string, Compare< size_t > > * tree)
{
  vectorOfDatasets.pushBack(std::make_pair(name, tree));
}

std::pair< bool, size_t > khairullin::Datasets::hasDataset(std::string name)
{
  for (size_t i = 0; i < vectorOfDatasets.getSize(); i++) {
    if (vectorOfDatasets[i].first == name) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

void khairullin::Datasets::print(std::string & line)
{
  std::string name = getToken(line);
  auto info = hasDataset(name);
  if (name.empty() || !info.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BSTree< size_t, std::string, Compare< size_t > > * tree = vectorOfDatasets[info.second].second;
  if (!tree) {
    throw std::logic_error("<EMPTY>");
  }
  auto iterator = BSTIterator< size_t, std::string, Compare< size_t > >{tree};
  iterator = iterator.begin();
  auto endIterator = iterator.end();
  std::cout << vectorOfDatasets[info.second].first;
  while (iterator != endIterator) {
    std::pair < size_t, std::string > data = iterator.read();
    std::cout << " " << data.first << " " << data.second;
    iterator = iterator.next();
  }
  std::cout << "\n";
}

void khairullin::Datasets::complement(std::string & line)
{
  std::string newSet = getToken(line);
  std::string first = getToken(line);
  std::string second = getToken(line);
  if (newSet.empty() || first.empty() || second.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoFirst = hasDataset(first);
  auto infoSecond = hasDataset(second);
  auto infoNewSet = hasDataset(newSet);
  if (!infoFirst.first || !infoSecond.first || infoNewSet.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BSTree< size_t, std::string, Compare< size_t > > * tree = nullptr;
  auto tree1 = vectorOfDatasets[infoFirst.second].second;
  auto tree2 = vectorOfDatasets[infoSecond.second].second;
  BSTIterator< size_t, std::string, Compare< size_t > > iterator(tree);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree1(tree1);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree2(tree2);
  auto root_tree1 = iter_tree1;
  auto root_tree2 = iter_tree2;
  iter_tree1 = iter_tree1.begin();
  iter_tree2 = iter_tree2.begin();
  auto endIterator1 = iter_tree1.end();
  auto endIterator2 = iter_tree2.end();
  while (iter_tree1 != endIterator1) {
    std::pair< size_t, std::string > data = iter_tree1.read();
    size_t key = data.first;
    if (root_tree2.has(key)) {
      iter_tree1 = iter_tree1.next();
      continue;
    }
    iterator = iterator.write(key, data.second);
    iter_tree1 = iter_tree1.next();
  }
  while (iter_tree2 != endIterator2) {
    std::pair< size_t, std::string > data = iter_tree2.read();
    size_t key = data.first;
    if (root_tree1.has(key)) {
      iter_tree2 = iter_tree2.next();
      continue;
    }
    iterator = iterator.write(key, data.second);
    iter_tree2 = iter_tree2.next();
  }
  addDataset(newSet, *iterator);
}

void khairullin::Datasets::intersect(std::string & line)
{
  std::string newSet = getToken(line);
  std::string firstName = getToken(line);
  std::string secondName = getToken(line);
  if (newSet.empty() || firstName.empty() || secondName.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoFirst = hasDataset(firstName);
  auto infoSecond = hasDataset(secondName);
  auto infoNewSet = hasDataset(newSet);
  if (!infoFirst.first || !infoSecond.first || infoNewSet.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BSTree< size_t, std::string, Compare< size_t > > * tree = nullptr;
  auto tree1 = vectorOfDatasets[infoFirst.second].second;
  auto tree2 = vectorOfDatasets[infoSecond.second].second;
  BSTIterator< size_t, std::string, Compare< size_t > > iterator(tree);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree1(tree1);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree2(tree2);
  iter_tree1 = iter_tree1.begin();
  auto endIterator = iter_tree1.end();
  while (iter_tree1 != endIterator) {
    std::pair< size_t, std::string > data = iter_tree1.read();
    if (iter_tree2.has(data.first)) {
      iterator = iterator.write(data.first, data.second);
    }
    iter_tree1 = iter_tree1.next();
  }
  addDataset(newSet, *iterator);
}

void khairullin::Datasets::union_set(std::string & line)
{
  std::string newSet = getToken(line);
  std::string firstName = getToken(line);
  std::string secondName = getToken(line);
  if (newSet.empty() || firstName.empty() || secondName.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoFirst = hasDataset(firstName);
  auto infoSecond = hasDataset(secondName);
  auto infoNewSet = hasDataset(newSet);
  if (!infoFirst.first || !infoSecond.first || infoNewSet.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  BSTree< size_t, std::string, Compare< size_t > > * tree = nullptr;
  auto tree1 = vectorOfDatasets[infoFirst.second].second;
  auto tree2 = vectorOfDatasets[infoSecond.second].second;
  BSTIterator< size_t, std::string, Compare< size_t > > iterator(tree);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree1(tree1);
  BSTConstIterator< size_t, std::string, Compare< size_t > > iter_tree2(tree2);
  auto root_tree1 = iter_tree1;
  iter_tree1 = iter_tree1.begin();
  iter_tree2 = iter_tree2.begin();
  auto endIterator1 = iter_tree1.end();
  auto endIterator2 = iter_tree2.end();
  while (iter_tree1 != endIterator1) {
    std::pair< size_t, std::string > data = iter_tree1.read();
    iterator = iterator.write(data.first, data.second);
    iter_tree1 = iter_tree1.next();
  }
  while (iter_tree2 != endIterator2) {
    std::pair< size_t, std::string > data = iter_tree2.read();
    if (root_tree1.has(data.first)) {
      iter_tree2 = iter_tree2.next();
      continue;
    }
    iterator = iterator.write(data.first, data.second);
    iter_tree2 = iter_tree2.next();
  }
  addDataset(newSet, *iterator);
}
#endif
