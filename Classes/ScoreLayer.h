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
    
    void setScore(int value);
    void setTarget(int value);
    void setBest(int value);
    void setStage(int value);
    cocos2d::Vec2 getScorePos() const;
    CC_SYNTHESIZE_READONLY(int, m_score, Score)
    CC_SYNTHESIZE_READONLY(int, m_target, Target)
    CC_SYNTHESIZE_READONLY(int, m_best, Best)
    CC_SYNTHESIZE_READONLY(int, m_stage, Stage)
    
private:
    void runUpdateAnimation(cocos2d::Label* label);
    
private:
    cocos2d::Label* m_scoreLabel;
    cocos2d::Label* m_targetLabel;
    cocos2d::Label* m_bestLabel;
    cocos2d::Label* m_stageLabel;
};

#endif /* defined(__Stars__ScoreLayer__) */
