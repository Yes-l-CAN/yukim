#include "Join.hpp"
#include "Operation.hpp"

Join::Join() {}

Join::~Join() {}

void Join::joinOn(CanClient *client)
{
  // flag JOIN <channel>
  try
  {
    isValidFormat();
    if (cmd[2] == "0")
    {
      leaveAll(client);
    }
    if (isKicked(client) == TRUE)
    {
      return ;
    }
    isAlreadyJoined(client);
    addClient(client);
    addChannel(client);
    sendMSG(client);

  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

int Join::isKicked(CanClient *client)
{
  if (channel->getKickedList().find(client->getSockFd()) != channel->getKickedList().end()) 
  {
    // kicked
    return (TRUE);
  }
  return (FALSE);
}

void Join::leaveAll(CanClient *client) 
{
  if (client != NULL)
  {
    std::map<std::string, CanChannel *>::iterator it;
    for(it = client->getChannelList().begin(); it != client->getChannelList().end(); it++)
    {







    }
  }
}

void Join::addClient(CanClient *client)
{
  if (client != NULL)
  {
    channel->addClientElement(client->getSockFd(), client);
  }
}

void Join::addChannel(CanClient *client)
{
  if (client != NULL)
  {
    client->addChannelElement(channel->getChannelName(), channel);
  }
}

void Join::sendMSG(CanClient *client) 
{
  std::string msgBuff;
  // :<source> JOIN <channel>

  msgBuff = ":" + client->getUsername() + " JOIN #" + channel->getChannelName() + "\r\n";
  channel->broadcast(msgBuff);
}

int Join::isAlreadyJoined(CanClient *client)
{
  if (client != NULL)
  {
    std::map<std::string, CanChannel *>::iterator find_it;
    std::map<std::string, CanChannel *>::iterator end_it;
    find_it = client->getChannelList().find(channel->getChannelName());
    end_it = client->getChannelList().end();
    if (find_it != end_it)
    {
      throw alreadyJoinedException();
    }
  }

  return (TRUE);
}

int Join::isValidFormat(void) 
{
  if (getSize() < 2 && (getSize() < ADD_LIMIT + 2))
  {
    throw invalidFormatException();
  }

  if (getFlag() != determineFlag())
  {
    throw invalidFormatException();
  }
  return (TRUE);
}

int Join::checkClientLevel(CanClient *client) {
  if (client->getMemberLevel() & CERTIFICATION_FIN == 0) {
    return (FALSE);
  }
  return (true);
}

int Join::determineFlag(void) { return (0); }

const char *Join::invalidChannelException::what() const throw() {
  return "Join : Invalid Channel !";
}

const char *Join::alreadyJoinedException::what() const throw() {
  return "Join : Already joined !";
}

const char *Join::channelOverflowException::what() const throw() {
  return "Join : channel Overflow";
}
