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
static const int s_labelNumberOffset    = 80;
static const int s_labelHeightInterval  = 50;

bool ScoreLayer::init()
{
    if (Layer::init()) {
        auto screenOrigin = Director::getInstance()->getVisibleOrigin();
        auto screenSize = Director::getInstance()->getVisibleSize();
        
        {
            auto label = Label::createWithBMFont(s_fontFile, "Score");
            this->addChild(label);
            label->setAnchorPoint(Vec2(0, 0));
            label->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow,
                                    screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
            m_scoreLabel = Label::createWithBMFont(s_fontFile, "0");
            this->addChild(m_scoreLabel);
            m_scoreLabel->setAnchorPoint(Vec2(0, 0));
            m_scoreLabel->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow + s_labelNumberOffset,
                                           screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
        }
        {
            auto label = Label::createWithBMFont(s_fontFile, "Target");
            this->addChild(label);
            label->setAnchorPoint(Vec2(0, 0));
            label->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow,
                                            screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
            m_targetLabel = Label::createWithBMFont(s_fontFile, "0");
            this->addChild(m_targetLabel);
            m_targetLabel->setAnchorPoint(Vec2(0, 0));
            m_targetLabel->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow + s_labelNumberOffset,
                                            screenOrigin.y + screenSize.height - s_labelHeightInterval*2));
        }
        {
            auto label = Label::createWithBMFont(s_fontFile, "Best");
            this->addChild(label);
            label->setAnchorPoint(Vec2(0, 0));
            label->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow,
                                screenOrigin.y + screenSize.height - s_labelHeightInterval));
            m_bestLabel = Label::createWithBMFont(s_fontFile, "0");
            this->addChild(m_bestLabel);
            m_bestLabel->setAnchorPoint(Vec2(0, 0));
            m_bestLabel->setPosition(Vec2(screenOrigin.x + s_labelSecondAlignRow + s_labelNumberOffset,
                                          screenOrigin.y + screenSize.height - s_labelHeightInterval));
        }
        {
            auto label = Label::createWithBMFont(s_fontFile, "Stage");
            this->addChild(label);
            label->setAnchorPoint(Vec2(0, 0));
            label->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow,
                                    screenOrigin.y + screenSize.height - s_labelHeightInterval));
            m_stageLabel = Label::createWithBMFont(s_fontFile, "0");
            this->addChild(m_stageLabel);
            m_stageLabel->setAnchorPoint(Vec2(0, 0));
            m_stageLabel->setPosition(Vec2(screenOrigin.x + s_labelFirstAlignRow + s_labelNumberOffset,
                                           screenOrigin.y + screenSize.height - s_labelHeightInterval));
        }
        
        return true;
    }
    return false;
}

void ScoreLayer::setScore(int value)
{
    m_score = value;
    m_scoreLabel->setString(to_string(value));
    this->runUpdateAnimation(m_scoreLabel);
}

void ScoreLayer::setTarget(int value)
{
    m_target = value;
    m_targetLabel->setString(to_string(value));
}

void ScoreLayer::setBest(int value)
{
    m_best = value;
    m_bestLabel->setString(to_string(value));
}

void ScoreLayer::setStage(int value)
{
    m_stage = value;
    m_stageLabel->setString(to_string(value));
}

void ScoreLayer::runUpdateAnimation(cocos2d::Label *label)
{
    // reset
    label->stopAllActions();
    label->setScale(1);
    
    //
    const float dt = 0.2f;
    const float scaleValue = 1.5f;
    
    auto scaleIn = ScaleBy::create(dt, scaleValue);
    auto scaleOut = ScaleBy::create(dt, 1/scaleValue);
    auto seq = Sequence::create(scaleIn, scaleOut, nullptr);
    label->runAction(seq);
}

Vec2 ScoreLayer::getScorePos() const
{
    Vec2 pos = m_scoreLabel->getPosition();
    pos.x += m_scoreLabel->getContentSize().width / 2;
    return pos;
}