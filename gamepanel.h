#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <CardPanel.h>
#include <QMainWindow>
#include "animationwindow.h"
#include "gamecontrol.h"
#include <QLabel>
#include <QMap>
#include "countdown.h"
#include "player.h"

//游戏的总窗口

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();
    enum AnimationType{ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet};
    void gameControlInit();
    void updatePlayerScore();
    void initCardMap();
    void cropImage(QPixmap& pix, int x, int y, Card& c);
    void initButtonsGroup();
    void initPlayerContext();
    void initGameScene();
    void gameStatusPrecess(GameControl::GameStatus status);//游戏状态切换 发牌-叫地主-游戏过程
    void startDispatchCard();//开始发牌
    void onDispatchCard();//处理发牌
    void cardMoveStep(Player *player, int curPos);
    void disposeCard(Player *player, const Cards &cards);//处理发下去的单张牌
    void updatePlayerCards(Player *player);//更新玩家卡牌信息
    void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status);//玩家状态切换
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);
    void onGrabLordBet(Player* player, int bet, bool flag);//处理叫地主
    void onDisposePlayHand(Player* player, const Cards& cards);//处理玩家出牌
    void onCardSelected(Qt::MouseButton button);
    void onUserPlayHand();
    void onUserPass();

    void showAnimation(AnimationType type, int bet = 0);//显示动画

    void hidePlayerDropCards(Player* player);//隐藏打出的牌
    // void showEndingScorePanel();
    // void initCountDown();

protected:
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent* ev);

private:
    enum CardAlign{Horizontal, Vertical};
    struct PlayerContext
    {
        QRect cardRect;
        QRect playHandRect;
        CardAlign align;
        bool isFrontSide;
        QLabel* info;
        QLabel* roleImg;//头像
        Cards lastCards;//刚打出的牌
    };

    Ui::GamePanel *ui;
    QPixmap m_bkImage;
    GameControl* m_gameCtl;
    QVector<Player*> m_playerList;
    QMap<Card, CardPanel*> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*,PlayerContext> m_contextMap;
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer* m_timer;
    AnimationWindow* m_animation;
    CardPanel* m_curSelCard;
    QSet<CardPanel*> m_selectCards;
    QRect m_cardsRect;
    QHash<CardPanel*, QRect> m_userCards;
    CountDown* m_countDown;

};
#endif // GAMEPANEL_H
