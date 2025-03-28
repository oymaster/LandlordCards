#include "card.h"

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit)
{
    setPoint(point);
    setSuit(suit);
}

void Card::setPoint(CardPoint point)
{
    m_point = point;
}

void Card::setSuit(CardSuit suit)
{
    m_suit = suit;
}

Card::CardPoint Card::getPoint() const
{
    return m_point;
}

Card::CardSuit Card::getSuit() const
{
    return m_suit;
}

bool lessSort(const Card &c1, const Card &c2)
{
    if(c1.getPoint() == c2.getPoint()){
        return c1.getSuit()<c2.getSuit();
    }
    return c1.getPoint()<c2.getPoint();
}

bool greaterSort(const Card &c1, const Card &c2)
{
    if(c1.getPoint() == c2.getPoint()){
        return c1.getSuit()>c2.getSuit();
    }
    return c1.getPoint()>c2.getPoint();
}


bool operator < (const Card &c1, const Card &c2)
{
    return lessSort(c1,c2);
}


bool operator == (const Card &c1, const Card &c2)
{
    return(c1.getPoint()==c2.getPoint()&&c1.getSuit()==c2.getSuit());
}

uint qHash(const Card &card)
{
    return card.getPoint()*100+card.getSuit();
}

QDataStream& operator<<(QDataStream& in, const Card& card)
{
    int suit = static_cast<int>(card.getSuit());
    int point = static_cast<int>(card.getPoint());
    in << suit << point;
    return in;
}

QDataStream& operator>>(QDataStream& out, Card& card)
{
    int suit, point;
    out >> suit >> point;
    card.setSuit(static_cast<Card::CardSuit>(suit));
    card.setPoint(static_cast<Card::CardPoint>(point));
    return out;
}

