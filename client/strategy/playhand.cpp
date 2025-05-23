#include "playhand.h"

PlayHand::PlayHand()
{

}

PlayHand::PlayHand(Cards &cards)
{
    classify(cards);
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
    m_type = type;
    m_pt = pt;
    m_extra = extra;
}

void PlayHand::classify(Cards &cards)
{
    // 将 Cards 对象转换为 CardList，CardList 是一个包含所有牌的列表
    CardList list = cards.toCardList();

    // cardRecord 数组用于统计每种点数的牌出现的次数，数组大小为 Card::Card_End（假设这是所有可能点数的数量）
    int cardRecord[Card::Card_End];

    // 初始化 cardRecord 数组，将所有元素设为 0
    memset(cardRecord, 0, sizeof(int) * Card::Card_End);

    // 遍历 CardList 中的每一张牌，统计每种点数出现的次数
    for(int i = 0; i < list.size(); ++i)
    {
        Card c = list.at(i);  // 获取当前牌
        cardRecord[c.getPoint()]++;  // 根据当前牌的点数更新 cardRecord 数组
    }

    // 清空四个分类容器，这些容器将用于存储不同点数的牌
    m_oneCard.clear();
    m_twoCard.clear();
    m_threeCard.clear();
    m_fourCard.clear();

    // 根据统计结果将点数对应的牌分类到不同的容器中
    for(int i = 0; i < Card::Card_End; ++i)
    {
        if(cardRecord[i] == 1)
        {
            m_oneCard.push_back((Card::CardPoint)i);  // 如果某个点数的牌出现了 1 次，将其添加到 m_oneCard 中
        }
        else if(cardRecord[i] == 2)
        {
            m_twoCard.push_back((Card::CardPoint)i);  // 如果某个点数的牌出现了 2 次，将其添加到 m_twoCard 中
        }
        else if(cardRecord[i] == 3)
        {
            m_threeCard.push_back((Card::CardPoint)i);  // 如果某个点数的牌出现了 3 次，将其添加到 m_threeCard 中
        }
        else if(cardRecord[i] == 4)
        {
            m_fourCard.push_back((Card::CardPoint)i);  // 如果某个点数的牌出现了 4 次，将其添加到 m_fourCard 中
        }
    }
}


void PlayHand::judgeCardType()
{
    m_type = Hand_Unknown;
    m_pt = Card::Card_Begin;
    m_extra = 0;

    if(isPass())
    {
        m_type = Hand_Pass;
    }
    if(isSingle())
    {
        m_type = Hand_Single;
        m_pt = m_oneCard[0];
    }
    else if(isPair())
    {
        m_type = Hand_Pair;
        m_pt = m_twoCard[0];
    }
    else if(isTriple())
    {
        m_type = Hand_Triple;
        m_pt = m_threeCard[0];
    }
    else if(isTripleSingle())
    {
        m_type = Hand_Triple_Single;
        m_pt = m_threeCard[0];
    }
    else if(isTriplePair())
    {
        m_type = Hand_Triple_Pair;
        m_pt = m_threeCard[0];
    }
    else if(isPlane())
    {
        m_type = Hand_Plane;
        m_pt = m_threeCard[0];
    }
    else if(isPlaneTwoSingle())
    {
        m_type = Hand_Plane_Two_Single;
        m_pt = m_threeCard[0];
    }
    else if(isPlaneTwoPair())
    {
        m_type = Hand_Plane_Two_Pair;
        m_pt = m_threeCard[0];
    }
    else if(isSeqPair())
    {
        m_type = Hand_Seq_Pair;
        m_pt = m_twoCard[0];
        m_extra = m_twoCard.size();
    }
    else if(isSeqSingle())
    {
        m_type = Hand_Seq_Single;
        m_pt = m_oneCard[0];
        m_extra = m_oneCard.size();
    }
    else if(isBomb())
    {
        m_type = Hand_Bomb;
        m_pt = m_fourCard[0];
    }
    else if(isBombSingle())
    {
        m_type = Hand_Bomb_Single;
        m_pt = m_fourCard[0];
    }
    else if(isBombPair())
    {
        m_type = Hand_Bomb_Pair;
        m_pt = m_fourCard[0];
    }
    else if(isBombTwoSingle())
    {
        m_type = Hand_Bomb_Two_Single;
        m_pt = m_fourCard[0];
    }
    else if(isBombJokers())
    {
        m_type = Hand_Bomb_Jokers;
    }
    else if(isBombJokersSingle())
    {
        m_type = Hand_Bomb_Jokers_Single;
    }
    else if(isBombJokersPair())
    {
        m_type = Hand_Bomb_Jokers_Pair;
    }
    else if(isBombJokersTwoSingle())
    {
        m_type = Hand_Bomb_Jokers_Two_Single;
    }
}

bool PlayHand::isPass()
{
    if(m_oneCard.size() == 0 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

PlayHand::HandType PlayHand::getHandType()
{
    return m_type;
}

Card::CardPoint PlayHand::getCardPoint()
{
    return m_pt;
}

int PlayHand::getExtra()
{
    return m_extra;
}

bool PlayHand::canBeat(const PlayHand &other)
{
    // 如果当前手牌类型是未知类型，则不能打败任何手牌
    if (m_type == Hand_Unknown)
    {
        return false;
    }

    // 如果对方的手牌类型是“过”操作（即没有出牌），当前手牌可以打败对方
    if (other.m_type == Hand_Pass)
    {
        return true;
    }

    // 如果当前手牌是王炸（Hand_Bomb_Jokers），它能够打败任何手牌
    if (m_type == Hand_Bomb_Jokers)
    {
        return true;
    }

    // 如果当前手牌是炸弹（Hand_Bomb）并且对方的手牌是单张、顺子等基本牌型（手牌类型从 Hand_Single 到 Hand_Seq_Single），
    // 那么炸弹可以打败这些手牌
    if (m_type == Hand_Bomb && other.m_type >= Hand_Single && other.m_type <= Hand_Seq_Single)
    {
        return true;
    }

    // 如果当前手牌类型和对方的手牌类型相同（比如同为顺子、对子等），
    // 进一步比较它们的牌面大小。如果是顺子或连对（Hand_Seq_Single 或 Hand_Seq_Pair），
    // 需要额外比较附加的牌的数量（extra）来确保手牌的强度是一样的。
    if (m_type == other.m_type)
    {
        if (m_type == Hand_Seq_Pair || m_type == Hand_Seq_Single)
        {
            // 对于顺子或连对，比较主牌面（m_pt）和附加牌的数量（m_extra）
            return m_pt > other.m_pt && m_extra == other.m_extra;
        }
        else
        {
            // 对于其他牌型（比如单牌、对牌），只需要比较主牌面的大小（m_pt）
            return m_pt > other.m_pt;
        }
    }

    // 如果上述条件都不满足，当前手牌不能打败对方
    return false;
}


bool PlayHand::isSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriple()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2 &&
            m_oneCard[0] != Card::Card_SJ && m_oneCard[1] != Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 2 && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() >= 3 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_twoCard.begin(), m_twoCard.end());
        if(m_twoCard.last() - m_twoCard.first() == (m_twoCard.size() -1) &&
            m_twoCard.first() >= Card::Card_3 && m_twoCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if(m_oneCard.size() >= 5 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() -1) &&
            m_oneCard.first() >= Card::Card_3 && m_oneCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.first() != Card::Card_SJ && m_oneCard.last() != Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.first() == Card::Card_SJ && m_oneCard.last() == Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersSingle()
{
    if(m_oneCard.size() == 3 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard[1] == Card::Card_SJ && m_oneCard[2] == Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersPair()
{
    if(m_oneCard.size() == 2 && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle()
{
    if(m_oneCard.size() == 4 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard[2] == Card::Card_SJ && m_oneCard[3] == Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

