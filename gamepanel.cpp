#include "gamepanel.h"
#include "qdebug.h"
#include "ui_gamepanel.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <iostream>
#include <QMouseEvent>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);
    //背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);
    //窗口的标题,大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000, 650);
    gameControlInit(); //实例化游戏控制类对象
    updatePlayerScore();//玩家得分
    initCardMap();//初始扑克牌
    initButtonsGroup();//初始化按钮组
    initPlayerContext();//初始化玩家在窗口中的上下文环境
    initGameScene();//卡牌界面
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);//10ms更新一次卡牌位置
    m_animation = new AnimationWindow(this);
    // m_bgm = new BGMControl(this);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    m_gameCtl = new GameControl(this);
    m_gameCtl->playerInit();
    Robot* leftRobot = m_gameCtl->getLeftRobot();
    Robot* rightRobot = m_gameCtl->getRightRobot();
    UserPlayer* user = m_gameCtl->getUserPlayer();
    m_playerList << leftRobot << rightRobot << user;

    connect(m_gameCtl, &GameControl::playerStatusChanged, this, &GamePanel::onPlayerStatusChanged);
    connect(m_gameCtl, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
    connect(m_gameCtl, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
    connect(m_gameCtl, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

    connect(leftRobot, &Player::notifyPickCards, this, &GamePanel::disposeCard);
    connect(rightRobot, &Player::notifyPickCards, this, &GamePanel::disposeCard);
    connect(user, &Player::notifyPickCards, this, &GamePanel::disposeCard);
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScores(
        m_playerList[0]->getScore(),
        m_playerList[1]->getScore(),
        m_playerList[2]->getScore());
}

void GamePanel::initCardMap()
{
    QPixmap pixmap(":/images/card.png");
    m_cardSize.setWidth(pixmap.width() / 13);
    m_cardSize.setHeight(pixmap.height() / 5);

    m_cardBackImg = pixmap.copy(2*m_cardSize.width(), 4*m_cardSize.height(),
                                m_cardSize.width(), m_cardSize.height());
    for(int i=0, suit=Card::Suit_Begin+1; suit<Card::Suit_End; ++suit, ++i)
    {
        for(int j=0, pt=Card::Card_Begin+1; pt<Card::Card_SJ; ++pt, ++j)
        {
            Card card((Card::CardPoint)pt, (Card::CardSuit)suit);
            cropImage(pixmap, j*m_cardSize.width(), i*m_cardSize.height(), card);
        }
    }
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap, 0, 4*m_cardSize.height(), c);

    c.setPoint(Card::Card_BJ);
    cropImage(pixmap, m_cardSize.width(), 4*m_cardSize.height(), c);
}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card &c)
{
    QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
    CardPanel* panel = new CardPanel(this);
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(c);
    panel->hide();
    m_cardMap.insert(c, panel);

}

void GamePanel::initButtonsGroup()
{
    ui->btnGroup->initButtons();
    ui->btnGroup->selectPanel(ButtonGroup::Start);

    connect(ui->btnGroup, &ButtonGroup::startGame, this, [=](){
        ui->btnGroup->selectPanel(ButtonGroup::Empty);
        m_gameCtl->clearPlayerScore();
        updatePlayerScore();
        gameStatusPrecess(GameControl::DispatchCard);
    });
    connect(ui->btnGroup, &ButtonGroup::playHand, this, &GamePanel::onUserPlayHand);
    connect(ui->btnGroup, &ButtonGroup::pass, this, &GamePanel::onUserPass);
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [=](int bet){
        m_gameCtl->getUserPlayer()->grabLordBet(bet);
        ui->btnGroup->selectPanel(ButtonGroup::Empty);
    });

}

void GamePanel::initPlayerContext()
{
    const QRect cardsRect[] =
        {
            QRect(90, 130, 100, height() - 200),
            QRect(rect().right() - 190, 130, 100, height() - 200),
            QRect(250, rect().bottom() - 120, width() - 500, 100)
        };
    const QRect playHandRect[] =
        {
            QRect(260, 150, 100, 100),
            QRect(rect().right() - 360, 150, 100, 100),
            QRect(150, rect().bottom() - 290, width() - 300, 105)
        };
    const QPoint roleImgPos[] =
        {
            QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),
            QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),
            QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)
        };

    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i=0; i<m_playerList.size(); ++i)
    {
        PlayerContext context;
        context.align = i==index ? Horizontal : Vertical;
        context.isFrontSide = i==index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(pt);
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerList.at(i), context);
    }
}

void GamePanel::initGameScene()
{
    std::cout<<"初始化卡牌界面"<<std::endl;
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(m_cardBackImg, m_cardBackImg);
    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(m_cardBackImg, m_cardBackImg);
    //地主牌
    for(int i=0; i<3; ++i)
    {
        CardPanel* panel = new CardPanel(this);
        panel->setImage(m_cardBackImg, m_cardBackImg);
        m_last3Card.push_back(panel);
        panel->hide();
    }
    //扑克牌位置
    m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
                           height() / 2 - 100);
    m_baseCard->move(m_baseCardPos);
    m_moveCard->move(m_baseCardPos);

    int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;
    for(int i=0; i<3; ++i)
    {
        m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);
    }
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    m_gameStatus = status;
    switch(status)
    {
    case GameControl::DispatchCard:
        startDispatchCard();
        break;
    case GameControl::CallingLord:
    {
        CardList last3Card = m_gameCtl->getSurplusCards().toCardList();
        for(int i=0; i<last3Card.size(); ++i)
        {
            QPixmap front = m_cardMap[last3Card.at(i)]->getImage();
            m_last3Card[i]->setImage(front, m_cardBackImg);
            m_last3Card[i]->hide();
        }
        m_gameCtl->startLordCard();
        break;
    }
    case GameControl::PlayingHand:
        m_baseCard->hide();
        m_moveCard->hide();
        for(int i=0; i<m_last3Card.size(); ++i)
        {
            m_last3Card.at(i)->show();
        }
        for(int i=0; i<m_playerList.size(); ++i)
        {
            PlayerContext &context = m_contextMap[m_playerList.at(i)];
            context.info->hide();
            Player* player = m_playerList.at(i);
            QPixmap pixmap = loadRoleImage(player->getSex(), player->getDirection(), player->getRole());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
        break;
    default:
        break;
    }
}


void GamePanel::startDispatchCard()
{
    for(auto it = m_cardMap.begin(); it!= m_cardMap.end(); ++it)
    {
        it.value()->setSelected(false);
        it.value()->setFrontSide(true);
        it.value()->hide();
    }
    for(int i=0; i<m_last3Card.size(); ++i)
    {
        m_last3Card.at(i)->hide();
    }
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i=0; i<m_playerList.size(); ++i)
    {
        m_contextMap[m_playerList.at(i)].lastCards.clear();
        m_contextMap[m_playerList.at(i)].info->hide();
        m_contextMap[m_playerList.at(i)].roleImg->hide();
        m_contextMap[m_playerList.at(i)].isFrontSide = i==index ? true : false;
    }
    m_gameCtl->resetCardData();
    m_baseCard->show();
    ui->btnGroup->selectPanel(ButtonGroup::Empty);
    m_timer->start(10);
    // m_bgm->playAssistMusic(BGMControl::Dispatch);
}

void GamePanel::cardMoveStep(Player *player, int curPos)
{
    QRect cardRect = m_contextMap[player].cardRect;
    const int unit[] = {
        (m_baseCardPos.x() - cardRect.right()) / 100,
        (cardRect.left() - m_baseCardPos.x()) / 100,
        (cardRect.top() - m_baseCardPos.y()) / 100
    };//左，右，下
    QPoint pos[] =
        {
            QPoint(m_baseCardPos.x() - curPos * unit[0], m_baseCardPos.y()),
            QPoint(m_baseCardPos.x() + curPos * unit[1], m_baseCardPos.y()),
            QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos * unit[2]),
        };

    int index = m_playerList.indexOf(player);
    m_moveCard->move(pos[index]);

    if(curPos == 0)
    {
        m_moveCard->show();
    }
    if(curPos == 100)
    {
        m_moveCard->hide();
    }
}

void GamePanel::onDispatchCard()
{
    static int curMovePos = 0;
    Player* curPlayer = m_gameCtl->getCurrentPlayer();
    if(curMovePos >= 100)
    {
        Card card = m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cs(card);
        disposeCard(curPlayer,cs);
        m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
        curMovePos = 0;
        cardMoveStep(curPlayer, curMovePos);
        if(m_gameCtl->getSurplusCards().cardCount() == 3)
        {
            m_timer->stop();
            gameStatusPrecess(GameControl::CallingLord);
            // m_bgm->stopAssistMusic();
            return;
        }
    }
    cardMoveStep(curPlayer, curMovePos);
    curMovePos += 15;
}

void GamePanel::disposeCard(Player *player, const Cards &cards)
{
    Cards& myCard = const_cast<Cards&>(cards);
    CardList list = myCard.toCardList();
    for(int i=0; i<list.size(); ++i)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->setOwner(player);
    }
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player *player)
{
    Cards cards = player->getCards();
    CardList list = cards.toCardList();

    m_cardsRect = QRect();
    m_userCards.clear();
    int cardSpace = 20;
    QRect cardsRect = m_contextMap[player].cardRect;
    for(int i=0; i<list.size(); ++i)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->show();
        panel->raise();
        panel->setFrontSide(m_contextMap[player].isFrontSide);

        if(m_contextMap[player].align == Horizontal)
        {
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - m_cardSize.height()) / 2;
            if(panel->isSelected())
            {
                topY -= 10;
            }
            panel->move(leftX + cardSpace * i, topY);
            m_cardsRect = QRect(leftX, topY,cardSpace * i + m_cardSize.width(), m_cardSize.height());
            int curWidth = 0;
            if(list.size() - 1 == i)
            {
                curWidth = m_cardSize.width();
            }
            else
            {
                curWidth = cardSpace;
            }
            QRect cardRect(leftX + cardSpace * i, topY, curWidth, m_cardSize.height());
            m_userCards.insert(panel, cardRect);
        }
        else
        {
            int leftX = cardsRect.left() + (cardsRect.width() - m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + i * cardSpace);
        }
    }
    //显示打出的牌
    QRect playCardRect = m_contextMap[player].playHandRect;
    Cards lastCards = m_contextMap[player].lastCards;
    if(!lastCards.isEmpty())
    {
        int playSpacing = 24;
        CardList lastCardList = lastCards.toCardList();
        CardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i=0; itplayed != lastCardList.constEnd(); ++itplayed, i++)
        {
            CardPanel* panel = m_cardMap[*itplayed];
            panel->setFrontSide(true);
            panel->raise();
            if(m_contextMap[player].align == Horizontal)
            {
                int leftBase = playCardRect.left() +
                               (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
                int top = playCardRect.top() + (playCardRect.height() - panel->height()) /2 ;
                panel->move(leftBase + i * playSpacing, top);
            }
            else
            {
                int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
                int top = playCardRect.top() ;
                panel->move(left, top + i * playSpacing);
            }
            panel->show();
        }
    }
}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status)
{
    switch (status)
    {
    case GameControl::ThinkingForCallLord:
        if(player == m_gameCtl->getUserPlayer())
        {
            ui->btnGroup->selectPanel(ButtonGroup::CallLord,m_gameCtl->getPlayerMaxBet());
        }
        break;
    case GameControl::ThinkingForPlayHand:
        hidePlayerDropCards(player);
        if(player == m_gameCtl->getUserPlayer())
        {
            Player* pendPlayer = m_gameCtl->getPendPlayer();
            if(pendPlayer == m_gameCtl->getUserPlayer() || pendPlayer == nullptr)
            {
                ui->btnGroup->selectPanel(ButtonGroup::PlayCard);
            }
            else
            {
                ui->btnGroup->selectPanel(ButtonGroup::PassOrPlay);
            }
        }
        else
        {
            ui->btnGroup->selectPanel(ButtonGroup::Empty);
        }
        break;
    case GameControl::Winning:
        // m_bgm->stopBGM();
        m_contextMap[m_gameCtl->getLeftRobot()].isFrontSide = true;
        m_contextMap[m_gameCtl->getRightRobot()].isFrontSide = true;
        updatePlayerCards(m_gameCtl->getLeftRobot());
        updatePlayerCards(m_gameCtl->getRightRobot());
        updatePlayerScore();
        m_gameCtl->setCurrentPlayer(player);
        // showEndingScorePanel();
        break;
    default:
        break;
    }
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png";
    lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png";
    farmerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png";
    farmerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png";

    QImage image;
    int random = QRandomGenerator::global()->bounded(2);
    if(sex == Player::Man && role == Player::Lord)
    {
        image.load(lordMan.at(random));
    }
    else if(sex == Player::Man && role == Player::Farmer)
    {
        image.load(farmerMan.at(random));
    }
    else if(sex == Player::Woman && role == Player::Lord)
    {
        image.load(lordWoman.at(random));
    }
    else if(sex == Player::Woman && role == Player::Farmer)
    {
        image.load(farmerWoman.at(random));
    }

    QPixmap pixmap;
    if(direct == Player::Left)
    {
        pixmap = QPixmap::fromImage(image);
    }
    else
    {
        pixmap = QPixmap::fromImage(image.mirrored(true, false));
    }
    return pixmap;
}

void GamePanel::onGrabLordBet(Player *player, int bet, bool flag)
{
    PlayerContext context = m_contextMap[player];
    if(bet == 0)
    {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    }
    else
    {
        if(flag)
        {
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }
        else
        {
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
        showAnimation(Bet, bet);
    }
    context.info->show();

    // m_bgm->playerRobLordMusic(bet, (BGMControl::RoleSex)player->getSex(), flag);
}

void GamePanel::onDisposePlayHand(Player *player, const Cards &cards)
{
    //隐藏上一轮打出的牌
    auto it = m_contextMap.find(player);
    it->lastCards = cards;
    Cards& myCards = const_cast<Cards&>(cards);
    PlayHand hand(myCards);
    //显示对于牌型动画
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Plane ||
        type == PlayHand::Hand_Plane_Two_Pair ||
        type == PlayHand::Hand_Plane_Two_Single)
    {
        showAnimation(Plane);
    }
    else if(type == PlayHand::Hand_Seq_Pair)
    {
        showAnimation(LianDui);
    }
    else if(type == PlayHand::Hand_Seq_Single)
    {
        showAnimation(ShunZi);
    }
    else if(type == PlayHand::Hand_Bomb)
    {
        showAnimation(Bomb);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        showAnimation(JokerBomb);
    }
    if(cards.isEmpty())
    {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
        // m_bgm->playPassMusic((BGMControl::RoleSex)player->getSex());
    }
    else
    {
        // if(m_gameCtl->getPendPlayer() == player || m_gameCtl->getPendPlayer() == nullptr)
        // {
        //     m_bgm->playCardMusic(cards, true, (BGMControl::RoleSex)player->getSex());
        // }
        // else
        // {
        //     m_bgm->playCardMusic(cards, false, (BGMControl::RoleSex)player->getSex());
        // }
    }
    //更新剩余牌
    updatePlayerCards(player);
    // if(player->getCards().cardCount() == 2)
    // {
    //     m_bgm->playLastMusic(BGMControl::Last2, (BGMControl::RoleSex)player->getSex());
    // }
    // else if(player->getCards().cardCount() == 1)
    // {
    //     m_bgm->playLastMusic(BGMControl::Last1, (BGMControl::RoleSex)player->getSex());
    // }
}

void GamePanel::onCardSelected(Qt::MouseButton button)
{
    //判断是不是出牌状态
    if(m_gameStatus == GameControl::DispatchCard ||
        m_gameStatus == GameControl::CallingLord)
    {
        return;
    }
    //2.判断发出信号的牌的所有者是不是当前用户玩家

    CardPanel* panel = static_cast<CardPanel*>(sender());
    if(panel->getOwner() != m_gameCtl->getUserPlayer())
    {
        return;
    }
    m_curSelCard = panel;
    //3.保存当前被选中的牌的窗口对象
    if(button == Qt::LeftButton)
    {
        panel->setSelected(!panel->isSelected());
        updatePlayerCards(panel->getOwner());
        QSet<CardPanel*>::const_iterator it = m_selectCards.find(panel);
        if(it == m_selectCards.constEnd())
        {
            m_selectCards.insert(panel);
        }
        else
        {
            m_selectCards.erase(it);
        }
        // m_bgm->playAssistMusic(BGMControl::SelectCard);
    }

    //4.判断参数的鼠标键是左键还是右键
    else if(button == Qt::RightButton)
    {
        onUserPlayHand();
    }
}


void GamePanel::onUserPlayHand()
{
    if(m_gameStatus != GameControl::PlayingHand)
    {
        return;
    }
    if(m_gameCtl->getCurrentPlayer() != m_gameCtl->getUserPlayer())
    {
        return;
    }
    if(m_selectCards.isEmpty())
    {
        return;
    }
    Cards cs;
    for(auto it = m_selectCards.begin(); it != m_selectCards.end(); ++it)
    {
        Card card = (*it)->getCard();
        cs.add(card);
    }
    PlayHand hand(cs);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Unknown)
    {
        return;
    }
    if(m_gameCtl->getPendPlayer() != m_gameCtl->getUserPlayer())
    {
        Cards cards = m_gameCtl->getPendCards();
        if(!hand.canBeat(PlayHand(cards)))
        {
            return;
        }
    }
    // m_countDown->stopCountDown();
    m_gameCtl->getUserPlayer()->playHand(cs);
    m_selectCards.clear();
}


void GamePanel::onUserPass(){

    // m_countDown->stopCountDown();
    qDebug()<< "GameControl::onUserPass信息:" << "目前玩家 " <<this->m_gameCtl->getCurrentPlayer()->getName()<<"放弃出牌";
    Player* curPlayer = m_gameCtl->getCurrentPlayer();
    Player* userPlayer = m_gameCtl->getUserPlayer();
    if(curPlayer != userPlayer)
    {
        return;
    }
    Player* pendPlayer = m_gameCtl->getPendPlayer();
    if(pendPlayer == userPlayer || pendPlayer == nullptr)
    {
        return;
    }
    Cards empty;
    userPlayer->playHand(empty);
    for(auto it = m_selectCards.begin(); it != m_selectCards.end(); ++it)
    {
        (*it)->setSelected(false);
    }
    m_selectCards.clear();
    updatePlayerCards(userPlayer);
}

void GamePanel::showAnimation(AnimationType type, int bet)
{
    switch (type) {
    case AnimationType::LianDui:
        break;
    case AnimationType::ShunZi:
        break;
    case AnimationType::Plane:
        break;
    case AnimationType::Bomb:
        break;
    case AnimationType::JokerBomb:
        break;
    case AnimationType::Bet:
        m_animation->setFixedSize(160,98);
        m_animation->move((width()-m_animation->width())/2,(height()-m_animation->height())/2-140);
        m_animation->showBetScore(bet);
        break;
    default:
        break;
    }
}

void GamePanel::hidePlayerDropCards(Player *player)
{
    auto it = m_contextMap.find(player);
    if(it != m_contextMap.end())
    {
        if(it->lastCards.isEmpty())
        {
            it->info->hide();
        }
        else
        {
            CardList list = it->lastCards.toCardList();
            for(auto last = list.begin(); last != list.end(); ++last)
            {
                m_cardMap[*last]->hide();
            }
        }
        it->lastCards.clear();
    }
}

void GamePanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

void GamePanel::mouseMoveEvent(QMouseEvent *ev)
{
    // 判断鼠标左键是否按下
    if(ev->buttons() & Qt::LeftButton)
    {
        // 获取鼠标当前位置
        QPoint pt = ev->pos();
        // 如果鼠标不在卡片区域内，则取消选中的卡片
        if(!m_cardsRect.contains(pt))
        {
            m_curSelCard = nullptr;
        }
        else
        {
            // 获取用户的所有卡片面板列表
            QList<CardPanel*> list = m_userCards.keys();
            // 遍历每个卡片面板
            for(int i = 0; i < list.size(); ++i)
            {
                CardPanel* panel = list.at(i);
                // 判断鼠标是否在该卡片的区域内，且该卡片没有被选中
                if(m_userCards[panel].contains(pt) && m_curSelCard != panel)
                {
                    // 如果鼠标在该卡片区域内，触发点击事件
                    panel->clicked();
                    // 更新当前选中的卡片
                    m_curSelCard = panel;
                }
            }
        }
    }
}











