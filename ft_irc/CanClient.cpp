#include "CanClient.hpp"
#include "CanChannel.hpp"
#include <map>
#include <utility>

CanClient::CanClient()
{
}

CanClient::CanClient(const CanClient& ref)
{
    *this = ref;
}

CanClient& CanClient::operator=(const CanClient& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

CanClient::~CanClient()
{
    
}

CanClient::CanClient(const struct sockaddr_in addr) : addr(addr)
{
}

void CanClient::setNickname(const std::string name)
{
    this->nickname = name;
}

std::string CanClient::getNickname(void) const
{
    return (this->nickname);
}

void CanClient::setUsername(const std::string name)
{
    this->username = name;
}

std::string CanClient::getUsername(void) const
{
    return (this->username);
}

void CanClient::setRealname(const std::string name)
{
    this->username = name;
}

std::string CanClient::getRealname(void) const
{
    return (this->username);
}

void CanClient::addChannelElement(std::string key, CanChannel* pNewChannel)
{
    try
    {
        // if (ret.second == false)
        if (this->channelList.insert(std::make_pair(key, pNewChannel)).second == false)
        {
            throw(CanClient::addChannelException());
        }
        pNewChannel->addClientElement(mySetFd, this);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void CanClient::deleteChannelElement(std::string key)
{
    try
    {
        std::map<std::string, CanChannel*>::iterator channelIter = (this->channelList).find(key);
        if ( channelIter != this->channelList.end())
        {
            channelIter->second->deleteClientElement(mySetFd);
            std::cout << "fd[" << mySetFd << "] : go out from \"" << key << "\"" << std::endl;
        }
        else
        {
            std::cout << "fd[" << mySetFd << "] : doesn't belong \"" << key << "\"" << std::endl;
        }    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    } 
}

void CanClient::cSend(int fd)
{
    try
    {
        int ret = send(fd, this->sendBuffer.c_str(), this->sendBuffer.length(), 0);
        if (ret < 0)
        {
            throw CanClient::cSendException();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void CanClient::cRecv(std::string msg)
{
    setRecvBuffer(msg);
}

void CanClient::setSendBuffer(std::string msg)
{
    this->sendBuffer = msg;
}

void CanClient::setRecvBuffer(std::string msg)
{
    this->recvBuffer = msg;
}

void CanClient::setPassStatus(const bool value)
{
    this->passStatus = value;
}

std::string& CanClient::getSendBuffer(void)
{
    return (this->sendBuffer);
}

std::string& CanClient::getRecvBuffer(void)
{
    return (this->recvBuffer);
}

bool CanClient::getPassStatus(void) const
{
    return (this->passStatus);
}

const char* CanClient::addChannelException::what() const throw()
{
    return "CanClient Exception :: you are already joined the channel";
}

const char* CanClient::cSendException::what() const throw()
{
    return "CanClient Exception :: couldn't send socket error";
}
