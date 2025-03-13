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

// 游戏的总窗口

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    using orderMap = QMap<int, QPair<QByteArray, int>>;

    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    enum AnimationType { ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet };

    // 初始化游戏控制类信息
    void gameControlInit();

    // 更新分数面板的分数
    void updatePlayerScore();

    // 切割并存储图片
    void initCardMap();

    // 裁剪图片
    void cropImage(QPixmap& pix, int x, int y, Card& c);

    // 初始化游戏按钮组
    void initButtonsGroup();

    // 初始化玩家在窗口中的上下文环境
    void initPlayerContext();

    // 初始化游戏场景
    void initGameScene();

    // 游戏状态切换 (发牌-叫地主-游戏过程)
    void gameStatusPrecess(GameControl::GameStatus status);

    // 开始发牌
    void startDispatchCard();

    // 玩家出牌移动步骤
    void cardMoveStep(Player *player, int curPos);

    // 处理发下去的单张牌
    void disposeCard(Player *player, const Cards &cards);

    // 更新玩家卡牌信息
    void updatePlayerCards(Player *player);

    // 加载角色图片
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    // 处理发牌
    void onDispatchCard();

    // 玩家状态切换
    void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status);

    // 处理叫地主
    void onGrabLordBet(Player* player, int bet, bool flag);

    // 处理玩家出牌
    void onDisposePlayHand(Player* player, const Cards& cards);

    // 处理玩家选牌
    void onCardSelected(Qt::MouseButton button);

    // 处理用户玩家出牌
    void onUserPlayHand();

    // 用户玩家放弃出牌
    void onUserPass();

    // 显示动画
    void showAnimation(AnimationType type, int bet = 0);

    // 隐藏打出的牌
    void hidePlayerDropCards(Player* player);

    // 显示结束得分面板
    void showEndingScorePanel();

    // 初始化闹钟倒计时
    void initCountDown();

    // 处理网络模式下玩家的发牌次序和分数
    void initGamePanel(QByteArray msg);

    // 更新用户名和分数
    void updatePlayerInfo(orderMap &info);

    // 开始游戏
    void startGame();
    void startGame(int index);

    // 通知服务器当前玩家出牌了
    void notifyOtherPlayHand(Cards& cs);

protected:
    // 处理窗口绘制事件
    void paintEvent(QPaintEvent *ev) override;

    // 处理鼠标移动事件
    void mouseMoveEvent(QMouseEvent* ev) override;

    // 处理窗口关闭事件
    void closeEvent(QCloseEvent* ev) override;

signals:
    void panelClose();

private:
    enum CardAlign { Horizontal, Vertical };

    struct PlayerContext {
        QRect cardRect; // 玩家扑克牌显示的区域
        QRect playHandRect; // 出牌的区域
        CardAlign align; // 卡牌排列方式 (水平/垂直)
        bool isFrontSide; // 是否正面显示
        QLabel* info; // 游戏过程中的提示信息 (比如: 不出)
        QLabel* roleImg; // 头像
        Cards lastCards; // 刚打出的牌
    };

    Ui::GamePanel *ui; // UI 界面对象
    QPixmap m_bkImage; // 背景图
    GameControl* m_gameCtl; // 游戏控制类
    QVector<Player*> m_playerList; // 玩家列表
    QMap<Card, CardPanel*> m_cardMap; // 卡牌数据和窗口的映射
    QSize m_cardSize; // 单张卡牌窗口大小
    QPixmap m_cardBackImg; // 卡牌背后图
    QMap<Player*, PlayerContext> m_contextMap; // 玩家信息的映射
    CardPanel* m_baseCard; // 中间的牌堆窗口
    CardPanel* m_moveCard; // 移动的卡牌窗口
    QVector<CardPanel*> m_last3Card; // 地主牌窗口
    QPoint m_baseCardPos; // 移动窗口坐标
    GameControl::GameStatus m_gameStatus; // 当前游戏状态
    QTimer* m_timer; // 当前窗口的定时器
    AnimationWindow* m_animation; // 动画窗口
    CardPanel* m_curSelCard; // 选择的卡牌窗口
    QSet<CardPanel*> m_selectCards; // 选择的卡牌集合
    QRect m_cardsRect; // 玩家手牌的显示区域
    QHash<CardPanel*, QRect> m_userCards; // 玩家手牌位置映射
    CountDown* m_countDown; // 倒计时对象
    BGMControl* m_bgm; // 背景音乐控制对象
    QByteArrayList m_nameList; // 玩家姓名列表
};

#endif // GAMEPANEL_H
