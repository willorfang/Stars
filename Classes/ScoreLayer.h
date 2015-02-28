//
//  ScoreLayer.h
//  Stars
//
//  Created by Wei Fang on 2015/02/25.
//
//

#ifndef __Stars__ScoreLayer__
#define __Stars__ScoreLayer__

#include "cocos2d.h"

class ScoreLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ScoreLayer)
    
    explicit ScoreLayer()
    : m_score(0),
      m_target(0),
      m_stage(0),
      m_best(0)
    {}
    
    virtual bool init() override;
    
    CC_SYNTHESIZE(int, m_score, Score)
    CC_SYNTHESIZE(int, m_target, Target)
    CC_SYNTHESIZE(int, m_best, Best)
    CC_SYNTHESIZE(int, m_stage, Stage)
    
private:
    cocos2d::Label* m_scoreLabel;
    cocos2d::Label* m_targetLabel;
    cocos2d::Label* m_bestLabel;
    cocos2d::Label* m_stageLabel;
};

#endif /* defined(__Stars__ScoreLayer__) */
