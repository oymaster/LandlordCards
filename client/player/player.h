#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString name,QObject *parent = nullptr);
    explicit Player(QObject *parent = nullptr);
    enum Role{Lord,Farmer};
    enum Sex{Man,Woman};
    enum Direction{Left,Right};
    enum Type{Robot,User,Unkonw};

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

    void storePendingInfo(Player* player, const Cards& cards);

    virtual void prepareCallLord();
    virtual void preparePlayHand();
    virtual void thinkCallLord();
    virtual void thinkPlayHand();


signals:
    void notifyGrabLordBet(Player *player, int bet);
    void notifyPlayHand(Player* player, const Cards& card);
    void notifyPickCards(Player* player, const Cards& cards);//拿到地主牌

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
