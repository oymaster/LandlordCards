#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <CardPanel.h>
#include <QMainWindow>
#include "animationwindow.h"
#include "gamecontrol.h"
#include <QLabel>
#include <QMap>
#include "bgmcontrol.h"
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
    void gameControlInit();// 初始化游戏控制类信息
    void updatePlayerScore(); // 更新分数面板的分数
    void initCardMap(); // 切割并存储图片
    void cropImage(QPixmap& pix, int x, int y, Card& c);// 裁剪图片
    void initButtonsGroup();// 初始化游戏按钮组
    void initPlayerContext();// 初始化玩家在窗口中的上下文环境
    void initGameScene();  // 初始化游戏场景
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
    void showEndingScorePanel();
    void initCountDown();

protected:
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent* ev);

private:
    enum CardAlign{Horizontal, Vertical};
    struct PlayerContext
    {
        QRect cardRect; // 1. 玩家扑克牌显示的区域
        QRect playHandRect;// 2. 出牌的区域
        CardAlign align;
        bool isFrontSide;
        QLabel* info;// 游戏过程中的提示信息, 比如: 不出
        QLabel* roleImg;//头像
        Cards lastCards;//刚打出的牌
    };

    Ui::GamePanel *ui;
    QPixmap m_bkImage;//背景图
    GameControl* m_gameCtl;//游戏控制类
    QVector<Player*> m_playerList;//玩家列表
    QMap<Card, CardPanel*> m_cardMap;//卡牌数据和窗口的映射
    QSize m_cardSize;//单张卡牌窗口大小
    QPixmap m_cardBackImg;//卡牌背后图
    QMap<Player*,PlayerContext> m_contextMap;//玩家信息的映射
    CardPanel* m_baseCard;//中间的牌堆窗口
    CardPanel* m_moveCard;//移动的卡牌窗口
    QVector<CardPanel*> m_last3Card;//地主牌窗口
    QPoint m_baseCardPos;//移动窗口坐标
    GameControl::GameStatus m_gameStatus;//当前游戏状态
    QTimer* m_timer;//当前窗口的定时器
    AnimationWindow* m_animation;//动画窗口
    CardPanel* m_curSelCard;//选择的卡牌窗口
    QSet<CardPanel*> m_selectCards;//
    QRect m_cardsRect;
    QHash<CardPanel*, QRect> m_userCards;
    CountDown* m_countDown;
    BGMControl* m_bgm;
};
#endif // GAMEPANEL_H
