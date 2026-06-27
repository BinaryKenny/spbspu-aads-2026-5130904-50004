#ifndef DATASETS_H
#define DATASETS_H
#include "BSTree.h"
#include "../common/Vector.h"
#include "Functions.h"
#include <iostream>

namespace khairullin {
  struct Datasets {
    Vector< std::pair<std::string, BSTree< size_t, std::string, Compare< size_t > > * > > vectorOfDatasets;
    using func_t = void(Datasets::*)(std::string & line);
    BSTree< std::string, func_t, Compare< std::string > > * commands;

    Datasets();
    ~Datasets();

    void function(std::string & line);

    void addDataset(std::istream & file);
    void addDataset(std::string name, BSTree< size_t, std::string, Compare< size_t > > * tree);
    std::pair< bool, size_t > hasDataset(const std::string & name) const;

    void print(std::string & line);
    void complement(std::string & line);
    void intersect(std::string & line);
    void union_set(std::string & line);

  };
}
#endif
