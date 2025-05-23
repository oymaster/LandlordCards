#ifndef CARD_H
#define CARD_H

#include <QDataStream>
#include <QVector>

class Card
{
public:
    enum CardSuit{
        Suit_Begin,
        Diamond,
        Club,
        Heart,
        Spade,
        Suit_End
    };
    enum CardPoint{
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,
        Card_BJ,
        Card_End
    };

    Card();
    Card(CardPoint point, CardSuit suit);
    void setPoint(CardPoint point);
    void setSuit(CardSuit suit);
    CardPoint getPoint() const;
    CardSuit getSuit() const;

    // 添加友元函数
    friend QDataStream& operator<<(QDataStream& in, const Card& card);
    friend QDataStream& operator>>(QDataStream& out, Card& card);


private:
    CardPoint m_point;
    CardSuit m_suit;

};

bool lessSort(const Card& c1,const Card& c2);
bool greaterSort(const Card& c1,const Card& c2);
bool operator <(const Card& c1,const Card& c2);
bool operator ==(const Card& c1,const Card& c2);

uint qHash(const Card& card);

using CardList = QVector<Card>;

#endif // CARD_H
