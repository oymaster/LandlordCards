#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "cards.h"
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QWaitCondition>

// 玩家 A 押注，生成一个任务（bet = 100）
// 玩家 B 押注，生成另一个任务（bet = 200）
// 有一个专门的线程负责处理这些任务（比如发牌、判断输赢）

// 任务类型
struct Task
{
    int bet = 0;
    Cards cs;
};

// 饿汉模式
class TaskQueue : public QObject
{
    Q_OBJECT
public:
    TaskQueue(const TaskQueue& t) = delete;
    TaskQueue& operator=(const TaskQueue& t) = delete;
    static TaskQueue* getInstance();
    // 1. 添加任务
    void add(Task &t);
    // 2. 取出一个任务
    Task take();
    // 3. 得到任务队列中任务的数量
    int size();
    // 4. 清空任务队列
    void clear();

private:
    explicit TaskQueue(QObject *parent = nullptr);

private:
    static TaskQueue m_taskQ;
    QQueue<Task> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;

signals:
};

#endif // TASKQUEUE_H
