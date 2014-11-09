//
//  FlappyBirdScene.cpp
//  hellococos2dx
//
//  Created by Macbook on 2014. 10. 26..
//
//

#include "FlappyBirdScene.h"

#define FORCE 30.0

USING_NS_CC;

Scene* FlappyBird::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    auto world = scene->getPhysicsWorld();
    
    // 'layer' is an autorelease object
    auto layer = FlappyBird::create();
    
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    world->setGravity(Point(0, -9.0*FORCE));
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
    
    birdCategory = 1 << 0;
    worldCategory = 1 << 1;
    pipeCategory = 1 << 2;
    scoreCategory = 1 << 3;
    
    this->Run();

    return true;
}


void FlappyBird::Run() {
//    auto skyColor = Color3B(81.0, 192.0/255.0, 201.0/255.0);
    glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
    
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
        sprite->setPositionZ(0);
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
    
    // setup our bird
    auto birdTexture1 = new Texture2D();
    auto birdTexture2 = new Texture2D();
    birdTexture1 = Director::getInstance()->getTextureCache()->addImage("bird-01.png");
    birdTexture2 = Director::getInstance()->getTextureCache()->addImage("bird-02.png");
    
    Rect rect = Rect::ZERO;
    rect.size = birdTexture1->getContentSize();
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.2);
    anim->addSpriteFrameWithTexture(birdTexture1, rect);
    anim->addSpriteFrameWithTexture(birdTexture2, rect);
    
    auto repeatedAnimation = RepeatForever::create(Animate::create(anim));
    
    bird = Sprite::createWithTexture(birdTexture1);
    bird->setScale(2.0);
    bird->setPosition(this->getContentSize().width * 0.35, this->getContentSize().height * 0.6);
    bird->setPositionZ(10);
    bird->runAction(repeatedAnimation);
    
    bird->setPhysicsBody(PhysicsBody::createCircle(2.0*bird->getContentSize().height/2.0));
    bird->getPhysicsBody()->setDynamic(true);
    bird->getPhysicsBody()->setRotationEnable(false);
    
    bird->getPhysicsBody()->setCategoryBitmask(birdCategory);
    bird->getPhysicsBody()->setCollisionBitmask(worldCategory | pipeCategory);
    bird->getPhysicsBody()->setContactTestBitmask(worldCategory | pipeCategory);
    
    this->addChild(bird);
    
    auto ground = Node::create();
    ground->setPosition(groundTexture->getContentSize().width/2.0, groundTexture->getContentSize().height/2.0);
    ground->setPhysicsBody(PhysicsBody::createBox(Size(this->getContentSize().width, 2.4*groundTexture->getContentSize().height * 1.0)));
    ground->getPhysicsBody()->setDynamic(false);
    ground->getPhysicsBody()->setCategoryBitmask(worldCategory);
    this->addChild(ground);
    
    // Touch Events (Single)
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBird::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FlappyBird::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    scheduleUpdate();
}

// function
bool FlappyBird::onContactBegin(PhysicsContact &contact) {
    if ((contact.getShapeA()->getCategoryBitmask() & scoreCategory) == scoreCategory || (contact.getShapeB()->getCategoryBitmask() & scoreCategory) == scoreCategory) {
//        score++
        
    } else {
        bird->getPhysicsBody()->setCollisionBitmask(worldCategory);
        
        auto rotate = RotateBy::create(1, (M_PI)*bird->getPosition().y*0.01);
        auto callback = CallFunc::create(CC_CALLBACK_0(FlappyBird::die, this));
        bird->runAction(Sequence::createWithTwoActions(rotate, callback));
        
        // Flash background if contact is detected
        this->removeChildByTag(10);
        
        auto bgSequence = Sequence::createWithTwoActions(CallFunc::create(CC_CALLBACK_0(FlappyBird::Blink, this)), DelayTime::create(0.5));
        auto backgroundAnimtaion = Repeat::create(bgSequence, 6);
        this->runAction(backgroundAnimtaion);
    }
    
    return true;
}
void FlappyBird::Blink() {
    if (blink) {
        glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
        blink = 0;
    } else {
        glClearColor(1, 0, 0, 1);
        blink = 1;
    }
}
void FlappyBird::die() {
//    bird->speed = 0;
    bird->setRotation(0);
    CCLOG("die");
}
void FlappyBird::removePipe(cocos2d::Node *pipe) {
    this->removeChild(pipe);
}
void FlappyBird::spawnPipes() {
//    CCLOG("spawn pipes : %f", this->getContentSize().width);
    
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
    pipeDown->setPositionZ(-10);
    
    PhysicsBody *pipeDownBody = PhysicsBody::createBox(Size(2.0 * pipeDown->getContentSize().width, 2.0 * pipeDown->getContentSize().height));
    pipeDownBody->setDynamic(false);
    pipeDown->setPhysicsBody(pipeDownBody);
    pipeDown->getPhysicsBody()->setCategoryBitmask(pipeCategory);
    pipeDown->getPhysicsBody()->setContactTestBitmask(birdCategory);
    
    pipePair->addChild(pipeDown);
    
    auto pipeUp = Sprite::createWithTexture(pipeTextureUp);
    pipeUp->setScale(3.0);
    pipeUp->setPosition(0.0, (float)y);
    pipeUp->setPositionZ(-10);
    
    PhysicsBody *pipeUpBody = PhysicsBody::createBox(Size(2.0 * pipeUp->getContentSize().width, 2.0 * pipeUp->getContentSize().height));
    pipeUpBody->setDynamic(false);
    pipeUp->setPhysicsBody(pipeUpBody);
    pipeUp->getPhysicsBody()->setCategoryBitmask(pipeCategory);
    pipeUp->getPhysicsBody()->setContactTestBitmask(birdCategory);
    pipePair->addChild(pipeUp);
    
    auto contactNode = Node::create();
    contactNode->setPosition(pipeDown->getContentSize().width + bird->getContentSize().width/2, this->getContentSize().height/2);
    contactNode->setPhysicsBody(PhysicsBody::createBox(Size(pipeUp->getContentSize().width, this->getContentSize().height)));
    contactNode->getPhysicsBody()->setDynamic(false);
    contactNode->getPhysicsBody()->setCategoryBitmask(scoreCategory);
    contactNode->getPhysicsBody()->setContactTestBitmask(birdCategory);
    pipePair->addChild(contactNode);
    
    pipePair->runAction(movePipesAndRemove);
    this->addChild(pipePair);
    
}

float clamp(float min, float max, float value) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    } else {
        return value;
    }
}
void FlappyBird::update(float delta) {
    bird->setRotation(clamp(-40, 40, -bird->getPhysicsBody()->getVelocity().y));
}// Touches
bool FlappyBird::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    
    bird->getPhysicsBody()->setVelocity(Vect(0,0));
    bird->getPhysicsBody()->applyImpulse(Vect(0, 700*FORCE));
    
    return true;
}
void FlappyBird::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
void FlappyBird::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {}
