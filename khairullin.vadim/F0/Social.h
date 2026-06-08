#ifndef SOCIAL_H
#define SOCIAL_H
#include "Graph.h"
namespace khairullin {
  struct Social {
    using process_t = void(Social::*)(std::string & line);
    Vector< Graph< std::string > > socials;
    HashTable< std::string, process_t, Hash< std::string >, Equal< std::string > > commands;

    Social();

    std::pair< bool, size_t > hasSocial(const std::string & name);
    Vector< std::string > recommendations(std::string nameSocial, std::string username);
    Vector< std::string > recommendations(std::string nameSocial, std::string username, size_t depth);
    void processor(std::istream &);
    void makeSocial(std::string & line);
    void makeFriends(std::string & line);
    void addUser(std::string & line);
    void stopFriendship(std::string & line);
    void deleteUser(std::string & line);
    void getRecommendation(std::string & line);
    void seekPotentialFriends(std::string & line);
    void showFriends(std::string & line);
    void findUser(std::string & line);
    void countOfFriends(std::string & line);
    void checkFriendship(std::string & line);
  };
}

#endif
