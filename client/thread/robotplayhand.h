#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include "player.h"

#include <QRunnable>
#include <QThread>

class RobotPlayHand : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* player, QObject *parent = nullptr);

signals:
protected:
    void run() override;

private:
    Player* m_player;

};

#endif // ROBOTPLAYHAND_H
