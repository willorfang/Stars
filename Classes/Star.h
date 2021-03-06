//
//  Star.h
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//

#ifndef __Stars__Star__
#define __Stars__Star__

#include "cocos2d.h"

class Star : public cocos2d::Sprite {
public:
    enum StarType {
        StarType0 = 0,
        StarType1,
        StarType2,
        StarType3,
        StarType4,
        StarType5,
        StarTypeCount
    };
    
public:
    static Star* createInstance(StarType type);
    void setStarSize(cocos2d::Size size);

    // animation
    void moveToWithAnimation(cocos2d::Vec2 pos);
    void removeWithAnimation();
    
    //
    static float getRemoveAnimationTime();
    static float getMoveAnimationTime();
    
    CC_SYNTHESIZE(StarType, m_type, Type)
    CC_SYNTHESIZE(bool, m_moving, Moving)
};

#endif /* defined(__Stars__Star__) */
