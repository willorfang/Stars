//
//  StarLayer.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//

#include "StarLayer.h"
#include "Star.h"

USING_NS_CC;
using namespace std;

static const float s_leftMargin = 10;
static const float s_rightMargin = 10;
static const float s_bottomMargin = 10;
static const float s_topMargin = 300;

StarLayer* StarLayer::createInstance(int width, int height)
{
    auto pRet = create();
    
    // put stars
    if (pRet != nullptr) {
        // init star table's size
        pRet->initStarTableSize(width, height);
        
        // view size
        auto origin = Director::getInstance()->getVisibleOrigin() + Vec2(s_leftMargin, s_bottomMargin);
        auto size = Director::getInstance()->getVisibleSize();
        // star size
        Size starSize;
        starSize.width = (size.width - s_leftMargin - s_rightMargin) / width;
        starSize.height = starSize.width;
        
        // add stars
        for (int i=0; i<height; ++i) {
            for (int j=0; j<width; ++j) {
                // get a random type
                auto type = static_cast<Star::StarType>(arc4random() % Star::StarTypeCount);
                //
                auto item = Star::createInstance(type);
                item->setStarSize(starSize);
                Vec2 pos;
                pos.x = origin.x + (j+0.5) * starSize.width;
                pos.y = origin.y + (i+0.5) * starSize.height;
                item->setPosition(pos);
                //
                pRet->addStar(i, j, item);
            }
        }
    }
    
    return pRet;
}

void StarLayer::initStarTableSize(int width, int height)
{
    m_starTable.resize(height);
    for (int i=0; i<m_starTable.size(); ++i) {
        m_starTable[i].resize(width);
    }
}

void StarLayer::addStar(int i, int j, Star* item)
{
    if (item != nullptr) {
        this->addChild(item);
        //
        if (m_starTable[i][j] != nullptr) {
            this->removeChild(m_starTable[i][j]);
        }
        m_starTable[i][j] = item;
    }

}

void StarLayer::removeStar(int i, int j)
{
    if (m_starTable[i][j] != nullptr) {
        this->removeChild(m_starTable[i][j]);
        m_starTable[i][j] = nullptr;
    }
}
