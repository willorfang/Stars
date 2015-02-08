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

class Star;

class StarLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(StarLayer)
    static StarLayer* createInstance(int width, int height);
    
    // origin is left-bottom
    void initStarTableSize(int width, int height);
    void addStar(int i, int j, Star* item);
    void removeStar(int i, int j);
    
    // the number of stars
    CC_SYNTHESIZE(int, m_width, Width)
    CC_SYNTHESIZE(int, m_height, Height)
    
    // geometry info
    CC_SYNTHESIZE(cocos2d::Size, m_starSize, StarSize)
    CC_SYNTHESIZE(cocos2d::Vec2, m_origin, Origin)
    
    // touch process
    void registerTouchListener();
    void onStarTouched(int row, int column);
    void findSameStars(std::multimap<int, int>& mapItem, int row, int column);
    
private:
    std::vector< std::vector<Star*> > m_starTable;
    
};

#endif /* defined(__Stars__StarLayer__) */
