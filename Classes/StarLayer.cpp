//
//  StarLayer.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//
#include <map>
#include <assert.h>

#include <SimpleAudioEngine.h>
#include "StarLayer.h"
#include "Star.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

static const float s_leftMargin = 10;
static const float s_rightMargin = 10;
static const float s_bottomMargin = 10;
static const float s_topMargin = 300;

static const char* s_starRemoveEffect = "sound/jewelappear.wav";

StarLayer* StarLayer::createInstance(int rowCount, int columnCount)
{
    auto pRet = create();
    
    // put stars
    if (pRet != nullptr) {
        // init star table's size
        pRet->initStarTableSize(rowCount, columnCount);
        
        // view size
        auto origin = Director::getInstance()->getVisibleOrigin() + Vec2(s_leftMargin, s_bottomMargin);
        pRet->setOrigin(origin);
        
        // star size
        auto size = Director::getInstance()->getVisibleSize();
        Size starSize;
        starSize.width = (size.width - s_leftMargin - s_rightMargin) / columnCount;
        starSize.height = starSize.width;
        pRet->setStarSize(starSize);
        
        // add stars
        for (int i=0; i<=rowCount*0.5; ++i) {
            for (int j=0; j<columnCount; ++j) {
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
#ifdef STAR_DEBUG
        pRet->setDebugNode();
#endif
        
        // touch
        pRet->registerTouchListener();
        
        // schedule droping stars
        pRet->scheduleUpdate();
        pRet->schedule(
            CC_SCHEDULE_SELECTOR(StarLayer::createRandomStar),
            2.0f);
        pRet->schedule(
            CC_SCHEDULE_SELECTOR(StarLayer::updateDropingStar),
            1.0f,
            CC_REPEAT_FOREVER,
            1.0f);
    }
    
    return pRet;
}

#ifdef STAR_DEBUG
void StarLayer::setDebugNode()
{
    m_debugNode = DrawNode::create();
    this->addChild(m_debugNode, 1000);
}
#endif

void StarLayer::initStarTableSize(int rowCount, int columnCount)
{
    m_rowCount = rowCount;
    m_columnCount = columnCount;
    //
    m_starTable.resize(rowCount);
    for (int i=0; i<m_starTable.size(); ++i) {
        m_starTable[i] = vector<Star*>(columnCount, nullptr);
    }
}

void StarLayer::addStar(int rowIndex, int columnIndex, Star* item)
{
    if (item != nullptr) {
        this->addChild(item);
        //
        if (m_starTable[rowIndex][columnIndex] != nullptr) {
            this->removeChild(m_starTable[rowIndex][columnIndex]);
            m_starIndexTable.erase(m_starTable[rowIndex][columnIndex]);
        }
        m_starTable[rowIndex][columnIndex] = item;
        m_starIndexTable[item] = make_pair(rowIndex, columnIndex);
    }

}

void StarLayer::removeStar(int rowIndex, int columnIndex)
{
    if (m_starTable[rowIndex][columnIndex] != nullptr) {
        m_starTable[rowIndex][columnIndex]->removeWithAnimation();
        m_starIndexTable.erase(m_starTable[rowIndex][columnIndex]);
        m_starTable[rowIndex][columnIndex] = nullptr;
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
        int columnIndex, rowIndex;
        columnIndex = (pos.x - m_origin.x) / m_starSize.width;
        rowIndex = (pos.y - m_origin.y) / m_starSize.height;
        if (rowIndex > m_rowCount-1
            || rowIndex < 0
            || columnIndex > m_columnCount-1
            || columnIndex < 0) {
            return false;
        } else {
            this->onStarTouched(rowIndex, columnIndex);
            return true;
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// whether a <key ,value> item is contained in a multimap or not
static bool isMapItemExist(const multimap<int, int>& mapItem, int key, int value) {
    using ValueRange = pair<multimap<int, int>::const_iterator, multimap<int, int>::const_iterator>;
    ValueRange range = mapItem.equal_range(key);
    for (auto i = range.first; i != range.second; ++i) {
        if (i->second == value) {
            return true;
        }
    }
    return false;
}

void StarLayer::findSameStars(multimap<int, int>& mapItem, int rowIndex, int columnIndex) {
    mapItem.insert(make_pair(columnIndex, rowIndex));
    // left
    if (columnIndex > 0
        && !isMapItemExist(mapItem, columnIndex-1, rowIndex)
        && m_starTable[rowIndex][columnIndex-1] != nullptr
        && m_starTable[rowIndex][columnIndex-1]->getType() == m_starTable[rowIndex][columnIndex]->getType()) {
        findSameStars(mapItem, rowIndex, columnIndex-1);
    }
    // right
    if (columnIndex+1 < m_columnCount
        && !isMapItemExist(mapItem, columnIndex+1, rowIndex)
        && m_starTable[rowIndex][columnIndex+1] != nullptr
        && m_starTable[rowIndex][columnIndex+1]->getType() == m_starTable[rowIndex][columnIndex]->getType()) {
        findSameStars(mapItem, rowIndex, columnIndex+1);
    }
    // bottom
    if (rowIndex > 0
        && !isMapItemExist(mapItem, columnIndex, rowIndex-1)
        && m_starTable[rowIndex-1][columnIndex] != nullptr
        && m_starTable[rowIndex-1][columnIndex]->getType() == m_starTable[rowIndex][columnIndex]->getType()) {
        findSameStars(mapItem, rowIndex-1, columnIndex);
    }
    // top
    if (rowIndex+1 < m_rowCount
        && !isMapItemExist(mapItem, columnIndex, rowIndex+1)
        && m_starTable[rowIndex+1][columnIndex] != nullptr
        && m_starTable[rowIndex+1][columnIndex]->getType() == m_starTable[rowIndex][columnIndex]->getType()) {
        findSameStars(mapItem, rowIndex+1, columnIndex);
    }
};

void StarLayer::onStarTouched(int rowIndex, int columnIndex)
{
    // if there is no star
    if (!m_touchEnabled || !hasStarAtIndex(rowIndex, columnIndex)) {
        return;
    }
    
    // <columnIndex, rowIndex> need to be removed
    multimap<int, int> touchedStarIndexes;
    map<int, int> removedTopIndexes;
    this->findSameStars(touchedStarIndexes, rowIndex, columnIndex);
    
    // remove from graph scene
    if (touchedStarIndexes.size() > 1) {
        // disable touch
        m_touchEnabled = false;
        
        for (auto i = touchedStarIndexes.begin(); i != touchedStarIndexes.end(); ++i) {
            int rowIndex = i->second;
            int columnIndex = i->first;
            this->removeStar(rowIndex, columnIndex);
            // find max row
            if (removedTopIndexes.count(columnIndex) == 0
                || removedTopIndexes[columnIndex] < rowIndex) {
                removedTopIndexes[columnIndex] = rowIndex;
            }
        }
        
        // play effect
        SimpleAudioEngine::getInstance()->playEffect(s_starRemoveEffect);
    
        // drop stars
        this->scheduleOnce(
                           [=] (float dt) {
                               for (auto i = removedTopIndexes.begin(); i != removedTopIndexes.end(); ++i) {
                                   for (int rowIndex = i->second + 1; rowIndex < m_rowCount; ++rowIndex) {
                                       int columnIndex = i->first;
                                       // reach top
                                       if (! hasStarAtIndex(rowIndex, columnIndex)) {
                                           break;
                                       }
                                       // drop
                                       size_t removedItemCount = touchedStarIndexes.count(columnIndex);
                                       this->dropStar(m_starTable[rowIndex][columnIndex], (int)removedItemCount);
                                   }
                               }
                               
                               // enable touch
                               this->scheduleOnce(
                                                  [this] (float dt) {
                                                      m_touchEnabled = true;
                                                  },
                                                  Star::getMoveAnimationTime(),
                                                  "RecoverTouch"
                                                  );
                           },
                           Star::getRemoveAnimationTime(),
                           "DropStar"
                           );
    
    }
}

bool StarLayer::dropStar(Star *item, int dropHeightCount)
{
    int rowIndex, columnIndex;
    this->getIndexForStar(item, rowIndex, columnIndex);
    
    if (item->getMoving()) {
        return false;
    } else {
        // move item
        Vec2 pos = item->getPosition() - Vec2(0, dropHeightCount * m_starSize.height);
        item->moveToWithAnimation(pos);
        return true;
    }
}

void StarLayer::createRandomStar(float dt)
{
    // get a random type
    auto type = static_cast<Star::StarType>(arc4random() % Star::StarTypeCount);
    //
    auto item = Star::createInstance(type);
    item->setStarSize(m_starSize);
    // get a random width index
    const int columnIndex = arc4random() % m_columnCount;
    const int rowIndex = m_rowCount - 1;
    Vec2 pos;
    pos.x = m_origin.x + (columnIndex+0.5) * m_starSize.width;
    pos.y = m_origin.y + (rowIndex+0.5) * m_starSize.height;
    item->setPosition(pos);
    
    //
    this->addStar(rowIndex, columnIndex, item);
    m_dropingStar.push_back(item);
}

bool StarLayer::getIndexForStar(Star* item, int& rowIndex, int& columnIndex)
{
    if (m_starIndexTable.count(item) == 1) {
        auto index = m_starIndexTable[item];
        rowIndex = index.first;
        columnIndex = index.second;
        return true;
    }
    return false;
}

bool StarLayer::getIndexForPosition(const cocos2d::Vec2& pos, int& rowIndex, int& columnIndex)
{
    columnIndex = (pos.x - m_origin.x) / m_starSize.width;
    rowIndex = (pos.y - m_origin.y) / m_starSize.height;
    
    return (rowIndex>=0) && (columnIndex>=0);
}

void StarLayer::updateDropingStar(float dt)
{
    for (auto i = m_dropingStar.begin(); i != m_dropingStar.end(); ) {
        // check its bottom neighbor
        int rowIndex, columnIndex;
        bool isExist = this->getIndexForStar(*i, rowIndex, columnIndex);
        if (isExist
            && rowIndex > 0
            && ! this->hasStarAtIndex(rowIndex-1, columnIndex)) {
            // drop by 1
            this->dropStar(*i, 1);
            ++i;
        } else {
            i = m_dropingStar.erase(i);
        }
    }
}

void StarLayer::update(float dt)
{
#ifdef STAR_DEBUG
    m_debugNode->clear();
#endif
    
    for (int rowIndex = 0; rowIndex < m_rowCount; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < m_columnCount; ++columnIndex) {
            // update table
            Star* item = m_starTable[rowIndex][columnIndex];
            if (item != nullptr && item->getMoving()) {
                int updatedRowIndex, updatedColumnIdex;
                this->getIndexForPosition(item->getPosition(), updatedRowIndex, updatedColumnIdex);
                if (updatedRowIndex != rowIndex
                    || updatedColumnIdex != columnIndex) {
                    m_starTable[rowIndex][columnIndex] = nullptr;
                    m_starTable[updatedRowIndex][updatedColumnIdex] = item;
                    m_starIndexTable[item] = make_pair(updatedRowIndex, updatedColumnIdex);
                }
            }
#ifdef STAR_DEBUG
            // draw rect for null position
            if (item == nullptr) {
                Vec2 pos = m_origin + Vec2((columnIndex+0.5)*m_starSize.width, (rowIndex+0.5)*m_starSize.height);
                m_debugNode->drawDot(pos, 15, Color4F(1, 0, 0, 0.5));
            }
#endif
        }
    }
}