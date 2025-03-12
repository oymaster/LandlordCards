#include "robot.h"
#include "datamanager.h"
#include "strategy.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include "taskqueue.h"
#include <QDebug>
#include <QThreadPool>

Robot::Robot(QObject *parent) : Player(parent)
{
    m_type = Player::Robot;
}

void Robot::prepareCallLord()
{
    RobotGrabLord* task = new RobotGrabLord(this);
    QThreadPool::globalInstance()->start(task);
    // connect(subThread, &RobotGrabLord::finished, this, [=](){
    //     qDebug() << "RobotGrabLord 子线程对象析构..." << ", Robot name: " << this->getName();
    //     subThread->deleteLater();
    // });
    // subThread->start();
}

void Robot::preparePlayHand()
{
    RobotPlayHand* task = new RobotPlayHand(this);
    QThreadPool::globalInstance()->start(task);
    // connect(subThread, &RobotGrabLord::finished, this, [=](){
    //     qDebug() << "RobotPlayHand 子线程对象析构..." << ", Robot name: " << this->getName();
    //     subThread->deleteLater();
    // });

    // subThread->start();
}

void Robot::thinkCallLord()
{
    if(DataManager::getInstance()->isNetworkMode())
    {
        Task t = TaskQueue::getInstance()->take();
        grabLordBet(t.bet);
        return;
    }
    int weight = 0;
    Strategy st(this, m_cards);
    weight += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;

    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() * 5;

    QVector<Cards> bombs = st.findCardsByCount(4);
    weight += bombs.size() * 5;

    weight += m_cards.pointCount(Card::Card_2) * 3;

    Cards tmp = m_cards;
    tmp.remove(optSeq);
    tmp.remove(bombs);
    Cards card2 = st.getRangeCards(Card::Card_2, Card::Card_2);
    tmp.remove(card2);
    QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
    weight += triples.size() * 4;

    tmp.remove(triples);
    QVector<Cards> pairs = Strategy(this, tmp).findCardsByCount(2);
    weight += pairs.size() * 1;

    if(weight >= 22)
    {
        grabLordBet(3);
    }
    else if(weight < 22 && weight >=18)
    {
        grabLordBet(2);
    }
    else if(weight < 18 && weight >= 10)
    {
        grabLordBet(1);
    }
    else
    {
        grabLordBet(0);
    }

}

void Robot::thinkPlayHand()
{
    qDebug() <<"Robot name: " << this->getName()<<"正在思考如何出牌";
    Strategy st(this, m_cards);
    Cards cs = st.makeStrategy();
    // cs.printAllCardInfo();
    playHand(cs);
}
