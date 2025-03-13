#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:

    enum Role{Lord, Farmer};    // 角色
    enum Sex{Man, Woman};       // 性别
    enum Direction{Left, Right};    // 头像的显示方位
    enum Type{Robot, User, UnKnow}; // 玩家的类型
    explicit Player(QString name,QObject *parent = nullptr);
    explicit Player(QObject *parent = nullptr);

    void setName(QString name);
    QString getName();

    void setRole(Role role);
    Role getRole();

    void setSex(Sex sex);
    Sex getSex();

    void setDirection(Direction direction);
    Direction getDirection();

    void setType(Type type);
    Type getType();

    void setScore(int score);
    int getScore();

    void setWin(bool flag);
    bool isWin();

    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* getPrevPlayer();
    Player* getNextPlayer();

    void grabLordBet(int point);

    void storeDispatchCard(const Card& card);
    void storeDispatchCard(const Cards& cards);

    Cards getCards();
    void clearCards();
    void playHand(const Cards& cards);

    Player* getPendPlayer();
    Cards getPendCards();

    // 存储出牌玩家对象和打出的牌
    void storePendingInfo(Player* player, const Cards& cards);
    // 清除信息
    void clearPendingInfo();

    virtual void prepareCallLord()=0;// 纯虚函数
    virtual void preparePlayHand();
    virtual void thinkCallLord();
    virtual void thinkPlayHand();


signals:
    // 通知已经叫地主下注
    void notifyGrabLordBet(Player* player, int bet);
    // 通知已经出牌
    void notifyPlayHand(Player* player, const Cards& card);
    // 通知已经发牌了
    void notifyPickCards(Player* player, const Cards& cards);

protected:
    int m_score = 0;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    bool m_isWin = false;
    Player *m_prev = nullptr;
    Player * m_next = nullptr;
    Cards m_cards;
    Cards m_pendCards;
    Player *m_pendPlayer = nullptr;
};

#endif // PLAYER_H
