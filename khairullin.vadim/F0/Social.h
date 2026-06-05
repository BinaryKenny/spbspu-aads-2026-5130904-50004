#ifndef SOCIAL_H
#define SOCIAL_H
#include "Graph.h"
namespace khairullin {
  struct Social {
    Vector< Graph< std::string > > socials;

    Social();

    std::pair< bool, size_t > hasSocial(const std::string & name);
    Vector< std::string > recommendations(std::string nameSocial, std::string username);
    Vector< std::string > recommendations(std::string nameSocial, std::string username, size_t depth);
    void makeSocial(std::istream &);
    void makeFriends(std::istream &);
    void addUser(std::istream &);
    void stopFriendship(std::istream &);
    void deleteUser(std::istream &);
    void getRecommendation(std::istream &);
    // getRecommendation <social> <username> <parameter> ("other", "deep")
    void seekPotentialFriends(std::istream &);
    void showFriends(std::istream &);
    void findUser(std::istream &);
    void countOfFriends(std::istream &);
    void checkFriendship(std::istream &);
  };
}

#endif
