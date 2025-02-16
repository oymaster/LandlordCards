#include "cards.h"
#include <QRandomGenerator>
#include <QDebug>
Cards::Cards() {}

Cards::Cards(const Card &card)
{
    add(card);
}

void Cards::add(const Card &card)
{
    m_cards.insert(card);
}

void Cards::add(const Cards &cards){
    m_cards.unite(cards.m_cards);
}

void Cards::add(const QVector<Cards>& cards){
    for(auto i : cards){
        add(i);
    }
}

Cards &Cards::operator <<(const Card &card)
{
    add(card);
    return *this;
}

Cards &Cards::operator <<(const Cards &cards)
{
    add(cards);
    return *this;
}

void Cards::remove(const Card &card)
{
    m_cards.remove(card);
}

void Cards::remove(const Cards &cards)
{
    m_cards.subtract(cards.m_cards);
}

void Cards::remove(const QVector<Cards> &cards)
{
    for(auto i : cards){
        remove(i);
    }
}

int Cards::cardCount()
{
    return m_cards.size();
}

bool Cards::isEmpty()
{
    return m_cards.isEmpty();
}

bool Cards::isEmpty() const
{
    return m_cards.isEmpty();
}

void Cards::clear()
{
    m_cards.clear();
}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint mx = Card::Card_Begin;
    if(!m_cards.isEmpty()){
        for(auto it = m_cards.begin(); it!=m_cards.end();++it){
            mx = std::max(mx,it->getPoint());
        }
    }
    return mx;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint mn = Card::Card_End;
    if(!m_cards.isEmpty()){
        for(auto it = m_cards.begin(); it!=m_cards.end();++it){
            mn = std::min(mn,it->getPoint());
        }
    }
    return mn;
}

int Cards::pointCount(Card::CardPoint point)
{
    int count = 0;
    for(auto it = m_cards.begin(); it!=m_cards.end(); ++it)
    {
        if(it->getPoint() == point)
        {
            count++;
        }
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return m_cards.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    return m_cards.contains(cards.m_cards);
}

Card Cards::takeRandomCard()
{
    int num = QRandomGenerator::global()->bounded(m_cards.size());
    QSet<Card>::const_iterator it = m_cards.constBegin();
    for(int i=0; i<num; ++i, ++it);
    Card card = *it;
    m_cards.erase(it);
    return card;
}

CardList Cards::toCardList(SortType type)
{
    CardList list;
    for(auto it = m_cards.begin();it!=m_cards.end();++it){
        list << *it;
    }
    if(type == Asc)
    {
        std::sort(list.begin(), list.end(), lessSort);
    }
    else if(type == Desc)
    {
        std::sort(list.begin(), list.end(), greaterSort);
    }
    return list;
}

void Cards::printAllCardInfo()
{
    QString text;
    char pts[] = "JQKA2";
    for(auto it = m_cards.begin(); it != m_cards.end(); ++it)
    {
        QString msg;
        Card::CardPoint pt = it->getPoint();
        Card::CardSuit suit = it->getSuit();
        if(suit == Card::CardSuit::Club)
        {
            msg = "梅花";
        }
        else if(suit == Card::CardSuit::Diamond)
        {
            msg = "方片";
        }
        else if(suit == Card::CardSuit::Heart)
        {
            msg = "红桃";
        }
        else
        {
            msg = "黑桃";
        }
        if(pt>=Card::Card_3 && pt <= Card::Card_10)
        {
            msg = QString("%1%2").arg(msg).arg(pt+2);
        }
        else if(pt >= Card::Card_J && pt <= Card::Card_2)
        {
            msg = QString("%1%2").arg(msg).arg(pts[pt-Card::Card_J]);
        }
        if(pt == Card::Card_BJ)
        {
            msg = "Big Joker";
        }
        if(pt == Card::Card_SJ)
        {
            msg = "Small Joker";
        }
        msg += "  ";
        text += msg;
    }
    qDebug() << text;
}

