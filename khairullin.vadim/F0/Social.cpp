#include "Social.h"
#include "Functions.h"
#include <random>

khairullin::Social::Social():
socials(Vector< Graph< std::string > >())
{}

std::pair< bool, size_t > khairullin::Social::hasSocial(const std::string & socialName)
{
  for (size_t i = 0; i < socials.getSize(); i++) {
    if (socials[i].name == socialName) {
      return std::make_pair(true, i);
    }
  }
  return std::make_pair(false, 0);
}

khairullin::Vector<std::string> khairullin::Social::recommendations(std::string nameSocial,
    std::string username)
{
  auto infoSocial = hasSocial(nameSocial);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & mainSocial = socials[infoSocial.second];
  auto infoUser = mainSocial.hasVertex(username);
  if (!infoUser.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Vector< std::string > & currentFriends =  mainSocial.edges[infoUser.second];
  Vector< std::string > potentialFriends;
  for (size_t i = 0; i < socials.getSize(); i++) {
    Graph< std::string > & social = socials[i];
    auto infoName = social.hasVertex(username);
    if (!infoName.first) {
      continue;
    }
    Vector< std::string > & friends = social.edges[infoName.second];
    for (size_t j = 0; j < friends.getSize(); i++) {
      std::string people = friends[j];
      bool chance = !potentialFriends.hasValue(people).first;
      chance = chance && mainSocial.hasVertex(people).first;
      chance = chance && !currentFriends.hasValue(people).first;
      if (chance) {
        try {
          potentialFriends.pushBack(people);
        }
        catch (...) {
          return potentialFriends;
        }
      }
    }
  }
  return potentialFriends;
}

khairullin::Vector<std::string> khairullin::Social::recommendations(std::string nameSocial,
    std::string username, size_t depth)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  auto infoSocial = hasSocial(nameSocial);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  auto infoUser = social.hasVertex(username);
  if (!infoUser.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Vector< std::string > & currentFriends = social.edges[infoUser.second];
  Vector< std::string > potentialFriends;
  Vector< std::string > & temp = currentFriends;
  for (size_t i = 0; i < depth + 1; i++) {
    std::uniform_int_distribution< size_t > randomizer(0, temp.getSize() - 1);
    std::string people = temp[randomizer(gen)];
    size_t peopleIndex = social.hasVertex(people).first;
    temp = social.edges[peopleIndex];
  }
  for (size_t i = 0; i < temp.getSize(); i++) {
    std::string user = temp[i];
    if (!currentFriends.hasValue(user).first) {
      try {
        potentialFriends.pushBack(user);
      }
      catch (...) {
        return potentialFriends;
      }
    }
  }
  return potentialFriends;
}

void khairullin::Social::makeSocial(std::istream & is)
{
  std::string name = "";
  std::getline(is, name);
  if (name.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > newSocial(name);
  socials.pushBack(newSocial);
}

void khairullin::Social::makeFriends(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  if (line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::string socialName = getToken(line);
  std::string name1 = getToken(line);
  std::string name2 = getToken(line);
  if (socialName.empty() || name1.empty() || name2.empty() || !line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  auto infoName1 = social.hasVertex(name1);
  auto infoName2 = social.hasVertex(name2);
  if (!infoName1.first || !infoName2.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  social.connect(name1, name2);
}

void khairullin::Social::addUser(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  std::string socialName = getToken(line);
  std::string name = getToken(line);
  if (name.empty() || socialName.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  social.addNode(name);
}

void khairullin::Social::stopFriendship(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  if (line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::string socialName = getToken(line);
  std::string name1 = getToken(line);
  std::string name2 = getToken(line);
  if (socialName.empty() || name1.empty() || name2.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  try {
    social.disconnect(name1, name2);
  }
  catch (...) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::cout << name1 << " and " << name2 << " are not friends from that moment\n";
}

void khairullin::Social::deleteUser(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  if (line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::string socialName = getToken(line);
  std::string name = getToken(line);
  if (socialName.empty() || name.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  if (!social.deleteNode(name)) {
    throw std::bad_alloc();
  }
}

void khairullin::Social::getRecommendation(std::istream & is)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::string line = "";
  std::getline(is, line);
  std::string socialName = getToken(line);
  std::string name = getToken(line);
  std::string parameter = getToken(line);
  if (socialName.empty() || name.empty() || parameter.empty() || !line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Vector< std::string > recUser;
  if (parameter == "other") {
    try {
      recUser = recommendations(socialName, name);
    }
    catch (std::logic_error & e) {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  else {
    size_t depth = 0;
    try {
      depth = std::stoi(parameter);
    }
    catch (...) {
      throw std::logic_error("<INVALID COMMAND>");
    }
    try {
      recUser = recommendations(socialName, name, depth);
    }
    catch (std::logic_error & e) {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  if (recUser.isEmpty()) {
    std::cout << "No recommendations\n";
    return;
  }
  std::uniform_int_distribution< size_t > randomizer(0, recUser.getSize() - 1);
  std::cout << recUser[randomizer(gen)] << "\n";
}

void khairullin::Social::showFriends(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  std::string socialName = getToken(line);
  std::string name = getToken(line);
  if (socialName.empty() || name.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  auto infoName = social.hasVertex(name);
  if (!infoName.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Vector< std::string > & vec = social.edges[infoName.second];
  if (vec.isEmpty()) {
    std::cout << "This users have 0 friends\n";
    return;
  }
  std::cout << vec[0];
  for (size_t i = 1; i < vec.getSize(); i++) {
    std::cout << " " << vec[i];
  }
  std::cout << "\n";
}

void khairullin::Social::findUser(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  std::string name = getToken(line);
  if (name.empty() || !line.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::string output = "";
  size_t counter = 0;
  for (size_t i = 0; i < socials.getSize(); i++) {
    Graph< std::string > & social = socials[i];
    if (social.hasVertex(name).first) {
      output = output + social.name + " ";
      counter++;
    }
  }
  for (size_t i = 0; i < counter; i++) {
    std::cout << getToken(output);
  }
  if (counter == 0) {
    std::cout << "This person doesn't have socials";
  }
  std::cout << "\n";
}

void khairullin::Social::countOfFriends(std::istream & is)
{
  std::string line = "";
  std::getline(is, line);
  std::string socialName = getToken(line);
  std::string name = getToken(line);
  if (socialName.empty() || name.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  auto infoName = social.hasVertex(name);
  if (!infoName.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::cout << social.edges[infoName.second].getSize() << "\n";
}

void khairullin::Social::checkFriendship(std::istream & is)
{
  std::string line = " ";
  std::getline(is, line);
  std::string socialName = getToken(line);
  std::string name1 = getToken(line);
  std::string name2 = getToken(line);
  if (socialName.empty() || name1.empty() || name2.empty()) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto infoSocial = hasSocial(socialName);
  if (!infoSocial.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph< std::string > & social = socials[infoSocial.second];
  auto infoName1 = social.hasVertex(name1);
  auto infoName2 = social.hasVertex(name2);
  if (!infoName1.first || !infoName2.first) {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Vector< std::string > & vec1 = social.edges[infoName1.second];
  Vector< std::string > & vec2 = social.edges[infoName2.second];
  if (vec1.hasValue(name2).first && vec2.hasValue(name1).first) {
    std::cout << "They are friends in the " << socialName << "\n";
  }
  else {
    std::cout << "They are NOT friends in the " << socialName << "\n";
  }
}
