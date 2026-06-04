#include "Social.h"
#include "Functions.h"

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
