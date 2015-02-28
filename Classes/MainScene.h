//
//  MainScene.h
//  Stars
//
//  Created by Wei Fang on 2015/02/11.
//
//

#ifndef __Stars__MainScene__
#define __Stars__MainScene__

#include "cocos2d.h"

class StarLayer;
class ScoreLayer;

class MainScene : public cocos2d::Scene
{
public:
    CREATE_FUNC(MainScene)
    
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    
protected:
    virtual bool init() override;
    
private:
    StarLayer* m_starLayer;
    ScoreLayer* m_scoreLayer;
};

#endif /* defined(__Stars__MainScene__) */
