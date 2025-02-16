#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include "card.h"
#include "playhand.h"

struct BetRecord
{
    BetRecord()
    {
        reset();
    }
    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player* player;
    int bet;
    int times;
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    enum GameStatus{
        DispatchCard,CallingLord,PlayingHand
    };
    enum PlayerStatus{
        ThinkingForCallLord,ThinkingForPlayHand,Winning
    };

    explicit GameControl(QObject *parent = nullptr);

    void playerInit();

    Robot* getLeftRobot();
    Robot* getRightRobot();
    UserPlayer* getUserPlayer();

    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer();

    Player* getPendPlayer();
    Cards getPendCards();

    void initAllCards();//初始化卡牌
    Card takeOneCard();//发牌
    Cards getSurplusCards();//地主牌
    void resetCardData();//每回合重置卡牌数据
    void startLordCard();//准备叫地主
    void becomeLord(Player *player, int bet);//当地主
    void clearPlayerScore(); //清空得分
    int getPlayerMaxBet();//获取最大下注分
    void onGrabBet(Player* player, int bet);//处理叫地主
    void onPlayHand(Player *player, const Cards &card);//处理出牌


signals:
    void playerStatusChanged(Player* player, PlayerStatus status);
    void notifyGrabLordBet(Player* player, int bet, bool flag);
    void gameStatusChanged(GameStatus status);
    void notifyPlayHand(Player* player, const Cards& card);
    void pendingInfo(Player* player, const Cards& card);

private:
    Robot *m_robotLeft;
    Robot *m_robotRight;
    UserPlayer *m_user;
    Player *m_currPlayer;
    Player *m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;
    int m_curBet = 0;
};

#endif // GAMECONTROL_H
