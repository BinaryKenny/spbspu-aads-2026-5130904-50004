#ifndef DATASETS_H
#define DATASETS_H
#include "BSTree.h"
#include "Vector.h"
#include "Functions.h"

namespace khairullin {
  struct Datasets {
    Vector< std::pair<std::string, BSTree< size_t, std::string, Compare< size_t > > * > > vectorOfDatasets;
    using func_t = void(Datasets::*)(std::istream &);
    BSTree< std::string, func_t, Compare< std::string > > commands;

    Datasets();
    ~Datasets() = default;

    void function(std::istream &);

    void addDataset(std::istream &);
    void addDataset(std::string name, BSTree< size_t, std::string, Compare< size_t > > * tree);
    std::pair< bool, size_t > hasDataset(std::string name);

    void print(std::istream &);
    void complement(std::istream &);
    void intersect(std::istream &);
    void union_set(std::istream &);

  };
}

khairullin::Datasets::Datasets():
vectorOfDatasets(Vector< std::pair< std::string, BSTree< size_t, std::string, Compare< size_t > > * > >()),
commands(BSTree< std::string, func_t, Compare< std::string > >())
{}

void khairullin::Datasets::addDataset(std::istream & file)
{
  std::string line = "";
  std::getline(file, line);
  std::string name = getToken(line);
  if (name == "") {
    throw std::logic_error("<INVALID COMMAND>");
  }
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
    }
    catch (...) {
      throw std::logic_error("<INVALID COMMAND>");
    }
    tree->push(key, value);
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

void khairullin::Datasets::print(std::istream & is)
{
  std::string name = "";
  is >> name;
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
  std::cout << vectorOfDatasets[info.second].first;
  while (iterator.root) {
    std::pair < size_t, std::string > data = iterator.read();
    std::cout << " " << data.first << " " << data.second;
    iterator = iterator.next();
  }
  std::cout << "\n";
}

void khairullin::Datasets::complement(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
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
  iter_tree1 = iter_tree1.begin();
  iter_tree2 = iter_tree2.begin();
  while (iter_tree1.exists() && iter_tree2.exists()) {
    std::pair< size_t, std::string > data = iter_tree1.read();
    size_t key = data.first;
    if (iter_tree2.has(key)) {
      continue;
    }
    iterator = iterator.write(key, data.second);
  }
  addDataset(newSet, *iterator);
}


#endif //DATASETS_H