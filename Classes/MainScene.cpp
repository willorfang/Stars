//
//  MainScene.cpp
//  Stars
//
//  Created by Wei Fang on 2015/02/11.
//
//
#include <SimpleAudioEngine.h>
#include "MainScene.h"
#include "StarLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

static const char* s_backgroundMusic = "sound/garden.mp3";

static const int CHILD_ORDER_STAR_LAYER = 1;

void MainScene::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    SimpleAudioEngine::getInstance()->playBackgroundMusic(s_backgroundMusic, true);
}

void MainScene::onExit()
{
    Scene::onExit();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

bool MainScene::init()
{
    if (Scene::init()) {
        m_starLayer = StarLayer::createInstance(14, 10);
        m_starLayer->setBackgroundType(StarLayer::BackgroundType::StarShine);
        this->addChild(m_starLayer, CHILD_ORDER_STAR_LAYER);
        
        return true;
    }
    
    return false;
}