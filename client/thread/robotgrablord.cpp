#include "robotGrablord.h"

RobotGrabLord::RobotGrabLord(Player* player, QObject *parent) : QObject(parent),QRunnable()
{
    m_player = player;
    setAutoDelete(true);
}

void RobotGrabLord::run()
{
    QThread::msleep(2000);
    m_player->thinkCallLord();
}
