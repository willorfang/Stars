//
//  StarLayer.h
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//

#ifndef __Stars__StarLayer__
#define __Stars__StarLayer__

#include "cocos2d.h"

#define STAR_DEBUG

class Star;

class StarLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(StarLayer)
    explicit StarLayer()
        : m_touchEnabled(true)
    {}
    
    static StarLayer* createInstance(int rowCount, int columnCount);
    
private:
    // origin is left-bottom
    void initStarTableSize(int rowCount, int columnCount);
    void addStar(int rowIndex, int columnIndex, Star* item);
    void removeStar(int rowIndex, int columnIndex);
    
    // the number of stars
    CC_SYNTHESIZE(int, m_rowCount, RowCount)
    CC_SYNTHESIZE(int, m_columnCount, ColumnCount)
    
    // geometry info
    CC_SYNTHESIZE(cocos2d::Size, m_starSize, StarSize)
    CC_SYNTHESIZE(cocos2d::Vec2, m_origin, Origin)
    
private:
    // touch process
    void registerTouchListener();
    void onStarTouched(int rowIndex, int columnIndex);
    void findSameStars(std::multimap<int, int>& mapItem, int rowIndex, int columnIndex);
    bool hasStarAtIndex(int rowIndex, int columnIndex)
    {
        return m_starTable[rowIndex][columnIndex] != nullptr;
    }
    
    bool dropStar(Star* item, int dropHeightCount);
    void createRandomStar(float dt);
    bool getIndexForStar(Star* item, int& rowIndex, int& columnIndex);
    bool getIndexForPosition(const cocos2d::Vec2& pos, int& rowIndex, int& columnIndex);
    
    void updateDropingStar(float dt);
    void update(float dt) override;
    
#ifdef STAR_DEBUG
    void setDebugNode();
#endif
    
private:
    std::vector< std::vector<Star*> > m_starTable;
    std::map< Star*, std::pair<int, int> > m_starIndexTable;
    std::vector<Star*> m_dropingStar;
    bool m_touchEnabled;
#ifdef STAR_DEBUG
    cocos2d::DrawNode* m_debugNode;
#endif
};

#endif /* defined(__Stars__StarLayer__) */
