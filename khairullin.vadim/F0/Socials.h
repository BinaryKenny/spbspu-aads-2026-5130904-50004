#ifndef SOCIALS_H
#define SOCIALS_H
#include "Graph.h"
namespace khairullin {
  struct Social {
    Vector< Graph< std::string > > socials;

    void makeFriends(std::string socialName, std::string name1, std::string name2);
    void addUser(std::string socialName, std::string username);
    void stopFriendship(std::string socialName, std::string name1, std::string name2);
    void deleteUser(std::string socialName, std::string username);
    void getRecommendation(std::string socialName, std::string username);
    void seekPotentialFriends(std::string socialName, std::string username);
    void showFriends(std::string socialName, std::string username);
    void findUser(std::string username);
    void countOfFriends(std::string socialName, std::string username);
    void checkFriendship(std::string socialName, std::string name1, std::string name2);
  };
}

#endif //SOCIALS_H
