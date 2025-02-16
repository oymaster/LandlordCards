#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playhand.h"

class Strategy
{
public:

    Strategy(Player* player, const Cards& cards);

    // 制定出牌策略，根据当前牌组来决定最优的出牌方式。
    Cards makeStrategy();

    // 返回第一个出牌策略。通常是先手出牌的策略。
    Cards firstPlay();

    // 获取比指定牌型大的牌（根据出牌规则进行筛选）。
    Cards getGreaterCards(PlayHand type);

    // 判断是否能打败当前指定的牌。
    bool whetherToBeat(Cards& cs);

    // 查找手牌中指定数量（count）相同点数的牌，返回一个符合条件的牌组。
    Cards findSamePointCards(Card::CardPoint point, int count);

    // 查找所有数量为指定数量（count）的牌，并返回符合条件的牌组。
    QVector<Cards> findCardsByCount(int count);

    // 根据给定的点数范围查找牌。
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);

    // 查找指定牌型的牌，如果需要打败其他牌，可以通过beat参数来控制。
    QVector<Cards> findCardType(PlayHand hand, bool beat);

    // 递归方法，用于根据牌型找到顺子单牌（Seq Single）。allSeqRecord 存储了所有可能的顺子单牌，
    // seqSingle 用于记录当前的顺子单牌，cards 是剩余的牌。
    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards);

    // 选择最优的顺子单牌方案，返回最合适的顺子单牌。
    QVector<Cards> pickOptimalSeqSingles();

private:
    // function 类型，定义一个函数指针，指向接受一个Card::CardPoint并返回Cards的成员函数。
    using function = Cards (Strategy::*)(Card::CardPoint point);

    // CardInfo结构体，包含了有关牌的相关信息：开始点数、结束点数、额外的参数、是否打败某张牌、牌数量等。
    struct CardInfo
    {
        Card::CardPoint begin; // 牌的起始点数。
        Card::CardPoint end;   // 牌的结束点数。
        int extra;             // 额外的参数，可能表示特定的牌型要求。
        bool beat;             // 是否需要打败某张牌。
        int number;            // 牌的数量。
        int base;              // 基本点数。
        function getSeq;       // 指向获取顺子的方法。
    };

    // 根据指定的点数和数量，获取对应的牌。
    QVector<Cards> getCards(Card::CardPoint point, int number);

    // 获取指定点数的三张牌组成的牌组或三单/三对牌。
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);

    // 获取指定点数开始的飞机牌型（例如，连续的三张一对牌）。
    QVector<Cards> getPlane(Card::CardPoint begin);

    // 获取指定点数开始的飞机带单牌或带对子。
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);

    // 获取指定信息（CardInfo）中的单牌或顺子单牌。
    QVector<Cards> getSepPairOrSeqSingle(CardInfo &info);

    // 获取基本的顺子对牌（根据点数）。
    Cards getBaseSeqPair(Card::CardPoint point);

    // 获取基本的顺子单牌（根据点数）。
    Cards getBaseSeqSingle(Card::CardPoint point);

    // 获取炸弹牌（四张相同点数的牌）。
    QVector<Cards> getBomb(Card::CardPoint begin);

private:
    Player* m_player;  // 指向玩家的指针，表示当前进行策略决策的玩家。
    Cards m_cards;     // 当前玩家的手牌，包含该玩家所有的牌。
};

#endif // STRATEGY_H
