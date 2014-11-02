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
        sprite->setScale(3.0);
        sprite->setPosition(Point(i*sprite->getContentSize().width, sprite->getContentSize().height / 2.0));
        sprite->runAction(moveGroundSpriteForever);
        this->addChild(sprite);
    }
    
    // skyline
    auto skyTexture = new Texture2D();
    skyTexture = Director::getInstance()->getTextureCache()->addImage("sky.png");
    
    for (int i = 0; i < 2.0 + this->getContentSize().width / (skyTexture->getContentSize().width * 2.0); i++) {
        auto moveSkySprite = MoveBy::create(0.1 * skyTexture->getContentSize().width * 2.0, Point(-skyTexture->getContentSize().width * 2.0, 0));
        auto resetSkySprite = MoveBy::create(0.0, Point(skyTexture->getContentSize().width * 2.0, 0));
        auto moveSkySpritesSequence = Sequence::createWithTwoActions(moveSkySprite, resetSkySprite);
        auto moveSkySpritesForever = RepeatForever::create(moveSkySpritesSequence);
        
        auto sprite = Sprite::createWithTexture(skyTexture);
        sprite->setScale(3.0);
        sprite->setPositionZ(-20);
        sprite->setPosition(i * sprite->getContentSize().width, sprite->getContentSize().height / 2.0 + groundTexture->getContentSize().height * 2.0);
        sprite->runAction(moveSkySpritesForever);
        this->addChild(sprite);
    }
    
    // create the pipes textures
    pipeTextureUp = new Texture2D();
    pipeTextureDown = new Texture2D();
    pipeTextureUp = Director::getInstance()->getTextureCache()->addImage("PipeUp.png");
    pipeTextureDown = Director::getInstance()->getTextureCache()->addImage("PipeDown.png");
    
    // create the pipes movement actions
    // spawn the pipes
    auto spawn = CallFunc::create(CC_CALLBACK_0(FlappyBird::spawnPipes, this));
    auto delay = DelayTime::create(2.0);
    auto spawnThenDelay = Sequence::create(spawn, delay, NULL);
    auto spawnThenDelayForever = RepeatForever::create(spawnThenDelay);
    this->runAction(spawnThenDelayForever);
}

// function
void FlappyBird::removePipe(cocos2d::Node *pipe) {
    this->removeChild(pipe);
}
void FlappyBird::spawnPipes() {
    auto distanceToMove = (float)(this->getContentSize().width + 2.0 * pipeTextureUp->getContentSize().width);
    auto movePipes = MoveBy::create(0.01 * distanceToMove, Point(-distanceToMove, 0));
    auto removePipes = CallFuncN::create(CC_CALLBACK_1(FlappyBird::removePipe, this));
    movePipesAndRemove = Sequence::create(movePipes, removePipes, NULL);
    
    int verticalPipeGap = 300.0;
    
    auto pipePair = Node::create();
    pipePair->setPosition(this->getContentSize().width + pipeTextureUp->getContentSize().width * 2.0, 0);
    pipePair->setPositionZ(-10);
    
    int height = this->getContentSize().height / 4.0;
    int y = arc4random() % height + height;
    
    auto pipeDown = Sprite::createWithTexture(pipeTextureDown);
    pipeDown->setScale(3.0);
    pipeDown->setPosition(0.0, (float)y + pipeDown->getContentSize().height + float(verticalPipeGap));
    
    PhysicsBody *pipeDownBody = PhysicsBody::createBox(Size(2.0 * pipeDown->getContentSize().width, 2.0 * pipeDown->getContentSize().height));
    pipeDownBody->setDynamic(false);
    pipeDown->setPhysicsBody(pipeDownBody);
    pipePair->addChild(pipeDown);
    
    auto pipeUp = Sprite::createWithTexture(pipeTextureUp);
    pipeUp->setScale(3.0);
    pipeUp->setPosition(0.0, (float)y);
    
    PhysicsBody *pipeUpBody = PhysicsBody::createBox(Size(2.0 * pipeUp->getContentSize().width, 2.0 * pipeUp->getContentSize().height));
    pipeUpBody->setDynamic(false);
    pipeUp->setPhysicsBody(pipeUpBody);
    pipePair->addChild(pipeUp);
    
    pipePair->runAction(movePipesAndRemove);
    this->addChild(pipePair);
    
}
void FlappyBird::update(float delta) {}

// Touches
bool FlappyBird::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {return true;}
void FlappyBird::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
void FlappyBird::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
