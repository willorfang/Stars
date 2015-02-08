//
//  StarLayer.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//
#include <map>

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
        // touch
        pRet->registerTouchListener();
        
        // init star table's size
        pRet->initStarTableSize(width, height);
        
        // view size
        auto origin = Director::getInstance()->getVisibleOrigin() + Vec2(s_leftMargin, s_bottomMargin);
        pRet->setOrigin(origin);
        
        // star size
        auto size = Director::getInstance()->getVisibleSize();
        Size starSize;
        starSize.width = (size.width - s_leftMargin - s_rightMargin) / width;
        starSize.height = starSize.width;
        pRet->setStarSize(starSize);
        
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
    m_width = width;
    m_height = height;
    //
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

void StarLayer::registerTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this] (Touch* touch, Event* event) -> bool {
        // get pos
        Point pos = this->convertToNodeSpace(touch->getLocation());
        // get node index
        int x, y;
        x = (pos.x - m_origin.x) / m_starSize.width;
        y = (pos.y - m_origin.y) / m_starSize.height;
        if (x > m_width-1 || y > m_height-1) {
            return false;
        } else {
            this->onStarTouched(y, x);
            return true;
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

static bool isMapItemExist(const multimap<int, int>& mapItem, int key, int value) {
    using ValueRange = pair<multimap<int, int>::const_iterator, multimap<int, int>::const_iterator>;
    ValueRange range = mapItem.equal_range(key);
    for (auto i=range.first; i!=range.second; ++i) {
        if (i->second == value) {
            return true;
        }
    }
    return false;
}

void StarLayer::findSameStars(multimap<int, int>& mapItem, int row, int column) {
    mapItem.insert(make_pair(column, row));
    // left
    if (column>0
        && !isMapItemExist(mapItem, column-1, row)
        && m_starTable[row][column-1] != nullptr
        && m_starTable[row][column-1]->getType() == m_starTable[row][column]->getType()) {
        findSameStars(mapItem, row, column-1);
    }
    // right
    if (column+1<m_width
        && !isMapItemExist(mapItem, column+1, row)
        && m_starTable[row][column+1] != nullptr
        && m_starTable[row][column+1]->getType() == m_starTable[row][column]->getType()) {
        findSameStars(mapItem, row, column+1);
    }
    // bottom
    if (row>0
        && !isMapItemExist(mapItem, column, row-1)
        && m_starTable[row-1][column] != nullptr
        && m_starTable[row-1][column]->getType() == m_starTable[row][column]->getType()) {
        findSameStars(mapItem, row-1, column);
    }
    // top
    if (row+1<m_height
        && !isMapItemExist(mapItem, column, row+1)
        && m_starTable[row+1][column] != nullptr
        && m_starTable[row+1][column]->getType() == m_starTable[row][column]->getType()) {
        findSameStars(mapItem, row+1, column);
    }
};

void StarLayer::onStarTouched(int row, int column)
{
    // find the same stars
    multimap<int, int> touchedStarIndexes;
    this->findSameStars(touchedStarIndexes, row, column);
    
    // remove from graph scene
    if (touchedStarIndexes.size() > 1) {
        for (auto i=touchedStarIndexes.begin(); i != touchedStarIndexes.end(); ++i) {
            this->removeStar(i->second, i->first);
        }
    }
    
    // drop
}