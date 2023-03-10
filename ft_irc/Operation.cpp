#include "Operation.hpp"
#include "Utility.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

Operation::Operation()
{
    std::cout << "constructor" << std::endl;
    this->server = new CanServer();
    this->server->s_On();
}

Operation::Operation(const Operation& obj)
{
    *this = obj;
}

// deep copy
Operation& Operation::operator=(const Operation& obj)
{
    if (this != &obj)
    {

        this->setFd = obj.setFd;
        this->result = obj.result;
        for (int i = 0; i < bufferSize; i++)
        {
            this->buffer[i] = obj.buffer[i];
        }

        // deep copy
        Operation replica(obj);

        CanServer* dummy = this->server;
        this->server = replica.server;
    }
    return *this;
}

Operation::~Operation()
{
    delete server;
}

void Operation::Transmission()
{
    server->s_Select();
    this->setFd = server->Transmission(); // return i(fd)
    if (this->setFd == server->getSocketFd())
    {
        std::cout << "accept client " << std::endl;
    }
    else
    {
        try
        {
            cRecv(this->setFd);
            std::vector<std::string> cmd = util.splitArr(this->buffer);

            CommandChecker(cmd);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    // command 따라서 파싱 하고 실행..?
    // write 테스트
}

void Operation::CommandChecker(std::vector<std::string> argv)
{
    std::string cmd[] = {"PASS", "NICK", "USER", "PING", "JOIN", "PART", "KICK", "NOTICE", "PRIVMSG", "QUIT"};

    for (int i = 0; i < sizeof(cmd) / sizeof(std::string); i++)
    {
        if (argv.front() == cmd[i])
        {
            switch (i)
            {
            case 0:
                this->Pass(argv);
                return;
            case 1:
                this->Nick(argv);
                return;
            case 2:
                this->User(argv);
                return;
            case 3:
                this->Ping(argv);
                return;
            case 4:
                this->Join(argv);
                return;
            case 5:
                this->Part(argv);
                return;
            case 6:
                this->Kick(argv);
                return;
            case 7:
                this->Notice(argv);
                return;
            case 8:
                this->PrivateMSG(argv);
                return;
            case 9:
                this->Quit(argv);
                return;
            default:;
                //  인자가 더해져 들어옴! (자동)
                // ./jisookim #42Seoul : hihi!
            }
        }
    }
}

void Operation::cRecv(int fd)
{
    int ret = recv(fd, buffer, bufferSize, 0);
    if (ret < 0)
        throw(Operation::recvException());
    if (ret == 0)
        throw(Operation::recvException()); // TODO: make exception more specifically.
}

void Operation::Client2ServSend(int fd)
{
    // need to think about it more ...
    std::map<int, CanClient*>::iterator clientIter = server->getClientList().find(fd);
    if (clientIter != server->getClientList().end())
    {
        clientIter->second->cSend(server->getSocketFd());
    }
    else
    {
        // 예외 클래스 생성 필요 할까? 
        std::cerr << "Client2ServSend : non-existent client FD" << std::endl;
    }
}

void Operation::Serv2ClientSend(int fd)
{
    // need to think about it more ...
    std::map<int, CanClient*>::iterator clientIter = server->getClientList().find(fd);
    if (clientIter != server->getClientList().end())
    {
        clientIter->second->cRecv(server->getBuffer());
    }
    else
    {
        // 예외 클래스 생성 필요 할까? 
        std::cerr << "Serv2ClientSend : non-existent client FD" << std::endl;
    }
}

void Operation::Pass(std::vector<std::string> argv)
{
    std::cout << "Pass Called!" << std::endl;

    // if (argv.size() < 2)
    // {
    //     ERR_PASSWDMISMATCH();
    // }
    // // if (argv.size() < 2 || argv.back() != server->getPassword())
    // // {
    // //     ERR_PASSWDMISMATCH();
    // // }
    // else
    {
                std::cout << "vecter.size = " << argv.size() << std::endl;

        std::cout << "vecter.back = " << argv.back() << std::endl;

        std::map<int, CanClient*>::iterator clientIter = server->getClientList().find(setFd);
        if (clientIter != server->getClientList().end())
        {
            clientIter->second->setPassStatus(true);
        }
        else
        {
            // 예외 클래스 생성 필요 할까? 
            std::cerr << "Pass : non-existent client FD" << std::endl;
        }
    }
}

int Operation::Nick(std::vector<std::string> argv)
{
    std::cout << "Nick Called!" << std::endl;
}

int Operation::User(std::vector<std::string> argv)
{
    std::cout << "User Called!" << std::endl;
}

void Operation::Ping(std::vector<std::string> argv)
{
    std::cout << "ping Called!" << std::endl;
}

// void    Pong(std::vector<std::string> argv);
void Operation::Join(std::vector<std::string> argv)
{
    std::cout << "join Called!" << std::endl;
}

void Operation::Part(std::vector<std::string> argv)
{
    std::cout << "part Called!" << std::endl;
}

void Operation::Kick(std::vector<std::string> argv)
{
    std::cout << "kick Called!" << std::endl;
}

void Operation::Notice(std::vector<std::string> argv)
{
    std::cout << "notice Called!" << std::endl;
}

void Operation::PrivateMSG(std::vector<std::string> argv)
{
    std::cout << "privatemsg Called!" << std::endl;
}

void Operation::Quit(std::vector<std::string> argv)
{
    std::cout << "quit Called!" << std::endl;
}

const char* Operation::recvException::what() const throw()
{
    return ("recv() exception occured!\n");
}

void Operation::ERR_PASSWDMISMATCH(void) const
{
    std::cerr << "<Client " << setFd << "> :Password incorrect" << std::endl;
    exit(464);
}