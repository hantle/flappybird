//
//  FlappyBirdScene.cpp
//  hellococos2dx
//
//  Created by Macbook on 2014. 10. 26..
//
//

#include "FlappyBirdScene.h"

USING_NS_CC;

Scene* FlappyBird::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
//    auto world = scene->getPhysicsWorld();
    
    // 'layer' is an autorelease object
    auto layer = FlappyBird::create();
    
//    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    world->setGravity(Point(0, -5.0));
//    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FlappyBird::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->Run();

    return true;
}

// function
void FlappyBird::removePipe(cocos2d::Node *pipe) {}
void FlappyBird::spawnPipes() {}
void FlappyBird::update(float delta) {}

void FlappyBird::Run() {
//    auto skyColor = Color3B(81.0, 192.0/255.0, 201.0/255.0);
    glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
    
    auto groundTexture = new Texture2D();
    groundTexture = Director::getInstance()->getTextureCache()->addImage("land.png");
    
    
    
    for (int i = 0; i < 2.0 + this->getContentSize().width / (groundTexture->getContentSize().width * 2.0); i++) {
        auto moveGroundSprite = MoveBy::create(0.02*groundTexture->getContentSize().width*2.0, Point(-groundTexture->getContentSize().width*2.0, 0));
        auto resetGroundSprite = MoveBy::create(0, Point(groundTexture->getContentSize().width*2.0, 0));
        auto moveGroundSequence = Sequence::createWithTwoActions(moveGroundSprite, resetGroundSprite);
        auto moveGroundSpriteForever = RepeatForever::create(moveGroundSequence);
        
        auto sprite = Sprite::createWithTexture(groundTexture);
        sprite->setScale(2.0);
        sprite->setPosition(Point(i*sprite->getContentSize().width, sprite->getContentSize().height / 2.0));
        sprite->runAction(moveGroundSpriteForever);
        this->addChild(sprite);
    }
}

// Touches
bool FlappyBird::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {return true;}
void FlappyBird::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
void FlappyBird::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
