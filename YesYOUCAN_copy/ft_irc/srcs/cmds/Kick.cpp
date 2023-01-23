#include "Kick.hpp"

#include "Operation.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void Kick::kickOn(CanClient *client)
{
  try
  {
    isValidFormat();

  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
}

std::map<CanChannel *, int> Kick::getChannel(CanClient *client) {}

int Kick::isOperator(CanClient *client)
{

}

void Kick::changeChannelStatus(CanClient *client) {}

void Kick::snedMSG(CanClient *client)
{

}

int Kick::isValidFormat(void)
{
  // flag KICK <channel> <user> [<comment>]

  if (getSize() < 3)
  {
    throw invalidFormatException();
  }
  else if (!(getSize() == 3 && getFlag() == 0) && \
           !(getSize() == 4 && getFlag() == 1))
  {
    throw invalidFormatException();
  }

  if (server->getChannelList().find(cmd[2]) == server->getChannelList().end())
  {
    throw noSuchChannelException();
  }
  else
  {
    channel = server->getChannelList().find(cmd[2])->second();
    // 빨간줄이 없어지지 않아요,,
  }

  
  if (channel->getClientList().find(cmd[3]) == channel->getClientList().end())
  {
      throw noSuchUserException();
  }
}

int Kick::checkClientLevel(CanClient *client) {
  if (client->getMemberLevel() & CERTIFICATION_FIN) {
    return (FALSE);
  }
  return (TRUE);
}

int Kick::determineFlag(void) { return (1); }

const char *Kick::notOperatorException::what() const throw() {
  return "Kick : not Operator ! No authority ";
}

const char *Kick::noSuchUserException::what() const throw() {
  return "Kick : No such user !";
}

const char *Kick::noSuchChannelException::what() const throw() {
  return "Kick : No such channel !";
}
