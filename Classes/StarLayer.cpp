//
//  StarLayer.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/07.
//
//
#include <map>
#include <assert.h>
#include <algorithm>

#include <SimpleAudioEngine.h>
#include "StarLayer.h"
#include "Star.h"
#include "ScoreLayer.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

static const float s_leftMargin = 10;
static const float s_rightMargin = 10;
static const float s_bottomMargin = 10;
static const float s_topMargin = 300;

static const int CHILD_ORDER_BACKGROUND = 0;
static const int CHILD_ORDER_STAR       = 1;
static const int CHILD_ORDER_SCORE      = 2;
static const int CHILD_ORDER_DEBUG      = 1000;

static const char* s_starRemoveEffect = "sound/jewelappear.wav";
static const string s_fontFile = "fonts/arial-unicode-26.fnt";

static vector<string> s_backgroundArray = {
    "res/StarShine.png",
};

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
    this->addChild(m_debugNode, CHILD_ORDER_DEBUG);
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
        this->addChild(item, CHILD_ORDER_STAR);
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
    if (!m_touchEnabled
        || !hasStarAtIndex(rowIndex, columnIndex)) {
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
        vector<Vec2> removeStarArray;
        
        for (auto i = touchedStarIndexes.begin(); i != touchedStarIndexes.end(); ++i) {
            int rowIndex = i->second;
            int columnIndex = i->first;
            removeStarArray.push_back(m_starTable[rowIndex][columnIndex]->getPosition());
            //
            this->removeStar(rowIndex, columnIndex);
            // find max row
            if (removedTopIndexes.count(columnIndex) == 0
                || removedTopIndexes[columnIndex] < rowIndex) {
                removedTopIndexes[columnIndex] = rowIndex;
            }
            
        }
        
        // play effect
        SimpleAudioEngine::getInstance()->playEffect(s_starRemoveEffect);
        
        // update scores
        this->runAddScoreAnimation(removeStarArray);
        
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
                               
                               this->scheduleOnce(
                                                  [this] (float dt) {
                                                      m_touchEnabled = true;
                                                  },
                                                  Star::getMoveAnimationTime(),
                                                  "RecoverTouch");
                               
#ifdef STAR_DEBUG
                               m_debugNode->clear();
                               // draw rect for null position
                               for (int i=0; i<m_rowCount; ++i) {
                                   for (int j=0; j<m_columnCount; ++j) {
                                       if (!hasStarAtIndex(i, j)) {
                                           Vec2 pos = m_origin + Vec2((j+0.5)*m_starSize.width, (i+0.5)*m_starSize.height);
                                           m_debugNode->drawDot(pos, 15, Color4F(1, 0, 0, 0.5));
                                       }
                                   }
                               }
#endif
                               
                           },
                           Star::getRemoveAnimationTime(),
                           "DropStarTouched"
                           );
    }
    
}

bool StarLayer::dropStar(Star *item, int dropHeightCount)
{
    int rowIndex, columnIndex;
    this->getIndexForStar(item, rowIndex, columnIndex);
    
    if (item->getMoving()
        || hasStarAtIndex(rowIndex - dropHeightCount, columnIndex)) {
        return false;
    } else {
        // move item
        Vec2 pos = item->getPosition() - Vec2(0, dropHeightCount * m_starSize.height);
        item->moveToWithAnimation(pos);
        
        // update table
        m_starTable[rowIndex][columnIndex] = nullptr;
        int updatedRowIndex = rowIndex - dropHeightCount;
        m_starTable[updatedRowIndex][columnIndex] = item;
        m_starIndexTable[item] = make_pair(updatedRowIndex, columnIndex);
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

void StarLayer::setBackgroundType(BackgroundType type)
{
    if (type < BackgroundType::MaxValue) {
        if (m_background && m_background->getParent()) {
            m_background->removeFromParent();
        }
        int index = static_cast<int>(type);
        m_background = Sprite::create(s_backgroundArray[index]);
        auto screenSize = Director::getInstance()->getVisibleSize();
        auto imageSize = m_background->getContentSize();
        float scaleX = screenSize.width/imageSize.width;
        float scaleY = screenSize.height/imageSize.height;
        m_background->setScale(max(scaleX, scaleY));
        m_background->setPosition(Vec2(screenSize.width/2, screenSize.height/2));
        this->addChild(m_background, CHILD_ORDER_BACKGROUND);
    }
}

void StarLayer::runAddScoreAnimation(vector<Vec2> starPosArray)
{
    const float dt = 1.0f;
    const Vec2 destPos = m_scoreLayer->getScorePos();
    
    float scaleValue = 1.0f;
    int scoreValue = 5;
    int totalScoreValue = 0;
    for(int i=0; i<starPosArray.size(); ++i) {
        totalScoreValue += scoreValue;
        // label
        auto label = Label::createWithBMFont(s_fontFile, to_string(scoreValue));
        label->setPosition(starPosArray[i]);
        label->setScale(scaleValue);
        this->addChild(label, CHILD_ORDER_SCORE);
        // action
        auto moveTo = MoveTo::create(dt, destPos);
        auto scaleTo = ScaleTo::create(dt, 0.3f);
        auto callback = CallFunc::create(
            [=] () {
                label->removeFromParent();
                if (i >= starPosArray.size() - 1) {
                    m_scoreLayer->setScore(m_scoreLayer->getScore() + totalScoreValue);
                }
            }
        );
        auto action = Spawn::create(moveTo, scaleTo, nullptr);
        auto easeIn = EaseIn::create(action, 3.0f);
        auto seq = Sequence::create(easeIn, callback, nullptr);
        
        label->runAction(seq);
        
        //
        scaleValue += 0.2f;
        scaleValue = min(scaleValue, 2.0f);
        scoreValue += 10;
    }
}