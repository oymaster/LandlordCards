#ifndef ROBOTGrabLORD_H
#define ROBOTGrabLORD_H

#include <QThread>
#include "player.h"

class RobotGrabLord : public QThread
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
