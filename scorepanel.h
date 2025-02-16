#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

//分数面板窗口

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();
    void setScores(int left, int right, int user);


private:
    Ui::ScorePanel *ui;
};

#endif // SCOREPANEL_H
