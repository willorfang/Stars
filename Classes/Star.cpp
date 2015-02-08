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

ActionInterval* Star::moveToWithAnimation(cocos2d::Vec2 pos)
{
    ActionInterval* pRet = nullptr;
    
    const float dt = 0.3f;
    auto moveTo = MoveTo::create(dt, pos);
    pRet = EaseBackIn::create(moveTo);
    
    return pRet;
}

ActionInterval* Star::removeWithAnimation()
{
    ActionInterval* pRet = nullptr;
    
    const float dt = 0.1f;
    auto scaleTo = ScaleTo::create(dt, 0.1);
    auto callback = CallFunc::create(
        [this] () {
           this->removeFromParent();
        }
    );
    
    pRet = Sequence::create(scaleTo, callback, NULL);
    return pRet;
}