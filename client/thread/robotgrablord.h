#ifndef ROBOTGrabLORD_H
#define ROBOTGrabLORD_H

#include <QRunnable>
#include <QThread>
#include "player.h"

class RobotGrabLord : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit RobotGrabLord(Player* player, QObject *parent = nullptr);

protected:
    void run();

signals:
private:
    Player* m_player;

};

#endif // ROBOTGrabLORD_H
