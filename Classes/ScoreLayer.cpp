//
//  ScoreLayer.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/25.
//
//

#include "ScoreLayer.h"

USING_NS_CC;
using namespace std;

static const string s_fontFile = "fonts/arial-unicode-26.fnt";
static const int s_labelFirstAlignRow   = 20;
static const int s_labelSecondAlignRow  = 200;
static const int s_labelHeightInterval  = 50;

bool ScoreLayer::init()
{
    if (Layer::init()) {
        // create
        m_scoreLabel = Label::createWithBMFont(s_fontFile, "Score: 0");
        this->addChild(m_scoreLabel);
        m_targetLabel = Label::createWithBMFont(s_fontFile, "Target: 0");
        this->addChild(m_targetLabel);
        m_bestLabel = Label::createWithBMFont(s_fontFile, "Best: 0");
        this->addChild(m_bestLabel);
        m_stageLabel = Label::createWithBMFont(s_fontFile, "Stage: 0");
        this->addChild(m_stageLabel);
        // pos & size
        auto screenOrigin = Director::getInstance()->getVisibleOrigin();
        auto screenSize = Director::getInstance()->getVisibleSize();
        m_stageLabel->setAnchorPoint(Vec2(0, 0));
        m_stageLabel->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow,
                                       screenOrigin.y + screenSize.height - s_labelHeightInterval));
        m_bestLabel->setAnchorPoint(Vec2(0, 0));
        m_bestLabel->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow,
                                    screenOrigin.y + screenSize.height - s_labelHeightInterval));
        m_targetLabel->setAnchorPoint(Vec2(0, 0));
        m_targetLabel->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow,
                                        screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
        m_scoreLabel->setAnchorPoint(Vec2(0, 0));
        m_scoreLabel->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow,
                                       screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
        return true;
    }
    return false;
}