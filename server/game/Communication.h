//
// Created by 1 on 25-3-5.
//

#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H
#include "TcpConnection.h"
#include "Codec.h"
#include "AesCrypto.h"
#include "MySqlConn.h"
#include "Room.h"
#include "RoomList.h"
class Communication {
public:
    Communication();
    ~Communication();
    void parseRequest(Buffer *buf);
    void handleAesFenfa(Message* reqMsg,Message& resMsg);
    void handleRegister(Message* reqMsg,Message& resMsg);
    void  handleLogin(Message* reqMsg,Message& resMsg);
    void handleAddRoom(Message* reqMsg,Message& resMsg);
    void handleGameOver(Message *reqMsg);
    void handleSearchRoom(Message *reqMsg, Message &resMsg);
    void handleLeaveRoom(Message* reqMsg, Message& resMsg);
    void handleGoodBye(Message* reqMsg);
    void readyForPlay(std::string roomName, std::string data);
    void dealCards(UserMap players);
    void initCards();
    std::pair<int, int> takeOneCard();
    void startGame(std::string roomName, UserMap players);
    void notifyOtherPlayers(std::string data, std::string roomName, std::string userName);
    void restartGame(Message* reqMsg);
    using sendCallback = function<void(std::string)>;
    using deleteCallback = function<void()>;
    void setCallback(sendCallback fun1,deleteCallback fun2);
private:
    std::string m_aesKey;
    sendCallback sendMessage;
    deleteCallback  disconnect;
    AesCrypto* m_aes = nullptr;
    MySqlConn* m_mysql = nullptr;
    Room* m_redis=nullptr;
    std::multimap<int, int> m_cards;
};

#endif //SERVER_COMMUNICATION_H
