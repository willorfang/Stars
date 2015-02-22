//
//  Star.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//

#include "Star.h"

USING_NS_CC;
using namespace std;

static vector<string> s_StarIconArray= {
    "res/StarType0.jpg",
    "res/StarType1.jpg",
    "res/StarType2.jpg",
    "res/StarType3.jpg",
    "res/StarType4.jpg",
    "res/StarType5.jpg"
};

Star* Star::createInstance(StarType type)
{
    auto pRet = new Star();
    //
    if (pRet && pRet->initWithFile(s_StarIconArray[type])) {
        pRet->setType(type);
        pRet->autorelease();
        pRet->setMoving(false);
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void Star::setStarSize(cocos2d::Size size)
{
    auto contentSize = this->getContentSize();
    this->setScaleX(size.width / contentSize.width);
    this->setScaleY(size.height / contentSize.height);
}

const float m_removeAnimationTime = 0.1f;
const float m_dropAnimationTime = 0.3f;

float Star::getRemoveAnimationTime()
{
    return m_removeAnimationTime;
}

float Star::getMoveAnimationTime()
{
    return m_dropAnimationTime;
}

void Star::moveToWithAnimation(cocos2d::Vec2 pos)
{
    ActionInterval* pRet = nullptr;
    m_moving = true;
    
    auto moveTo = MoveTo::create(m_dropAnimationTime, pos);
    auto easeIn = EaseBackIn::create(moveTo);
    auto callback = CallFunc::create(
        [this] () {
            m_moving = false;
        }
    );
    
    pRet = Sequence::create(easeIn, callback, NULL);
    this->runAction(pRet);
}

void Star::removeWithAnimation()
{
    ActionInterval* pRet = nullptr;
    
    auto scaleTo = ScaleTo::create(m_removeAnimationTime, 0.1f);
    auto callback = CallFunc::create(
        [this] () {
           this->removeFromParent();
        }
    );
    
    pRet = Sequence::create(scaleTo, callback, NULL);
    this->runAction(pRet);
}