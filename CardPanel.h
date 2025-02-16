#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"

//单张卡牌窗口

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);//显式构造函数

    //给卡牌获取图片
    void setImage(QPixmap &front,QPixmap &back);
    QPixmap getImage();

    //扑克牌显示哪一面
    void setFrontSide(bool flag);
    bool isFrontSide();

    //是否被选中，例如选取即将打出
    void setSelected(bool flag);
    bool isSelected();

    //扑克牌花色
    void setCard(Card& card);
    Card getCard();

    //所有者
    void setOwner(Player *player);
    Player *getOwner();

    void clicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void cardSelected(Qt::MouseButton button);

private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isFront;
    bool m_isSelected;
    Card m_card;
    Player *m_owner;


};

#endif // CARDPANEL_H
