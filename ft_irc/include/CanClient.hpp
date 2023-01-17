#ifndef CAN_CLIENT_HPP
#define CAN_CLIENT_HPP

#include <map>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string>
#include <sys/socket.h>

class CanChannel;

class CanClient
{
private:
    bool passStatus;

    std::string nickname;
    std::string username;
    std::string realname;
    struct sockaddr_in addr;

    std::map<std::string, CanChannel*> channelList; // channelList which the client belongs
    int mySetFd;

    std::string sendBuffer;
    std::string recvBuffer;
public:
    CanClient();
    CanClient(const CanClient& ref);
    CanClient(const struct sockaddr_in addr);
    CanClient& operator=(const CanClient& ref);
    ~CanClient();

    // setter
    void setNickname(const std::string name);
    void setUsername(const std::string name);
    void setRealname(const std::string name);
    void setSendBuffer(std::string msg);
    void setRecvBuffer(std::string msg);
    void setPassStatus(const bool value);

    // getter
    std::string getNickname(void) const;
    std::string getUsername(void) const;
    std::string getRealname(void) const;
    std::string& getSendBuffer(void);
    std::string& getRecvBuffer(void);
    bool getPassStatus(void) const;

    void addChannelElement(std::string key, CanChannel* pNewChannel);   // join channel
    void deleteChannelElement(std::string key);    // come outside channel

    void cSend(int fd);
    void cRecv(std::string msg);

    class addChannelException : public std::exception
    {
        virtual const char* what() const throw();
    };

    class cSendException : public std::exception
    {
        virtual const char* what() const throw();
    };
};

#endif