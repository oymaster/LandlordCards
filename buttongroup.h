#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

//按钮组窗口，开始，抢地主等按钮显示

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel{Start, PlayCard, PassOrPlay, CallLord, Empty};
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    void initButtons();
    void selectPanel(Panel type, int bet = 0);

signals:
    void startGame();
    void playHand();
    void pass();
    void betPoint(int bet);

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
