#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>


namespace khairullin {
  std::string getToken(std::string & line)
  {
    std::string token = "";
    std::string result = "";
    size_t new_start = 0;
    for (size_t i = 0; i < line.length() && line[i] != ' '; i++) {
      token += line[i];
      new_start = i;
    }
    if (new_start == line.length() - 1) {
      line = result;
      return token;
    }
    new_start += 2;
    for (size_t i = new_start; i < line.length() && (line[i] != '\0' && line[i] != '\n'); i++) {
      result += line[i];
    }
    line = result;
    return token;
  }
}
#endif

