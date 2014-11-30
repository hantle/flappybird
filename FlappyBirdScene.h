//
//  FlappyBirdScene.h
//  hellococos2dx
//
//  Created by Macbook on 2014. 10. 26..
//
//

#ifndef __hellococos2dx__FlappyBirdScene__
#define __hellococos2dx__FlappyBirdScene__

#include "cocos2d.h"

class FlappyBird : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // start
    cocos2d::Sprite *bird;
    cocos2d::Texture2D *pipeTextureUp;
    cocos2d::Texture2D *pipeTextureDown;
    cocos2d::Sequence *movePipesAndRemove;
    cocos2d::Node *moving;
    cocos2d::Node *pipes;
    cocos2d::LabelTTF *scoreLabelNode;
    cocos2d::RepeatForever *spawnThenDelayForever;
    
    
    int birdCategory;
    int worldCategory;
    int pipeCategory;
    int scoreCategory;
    int blink;
    int birdspeed;
    int movingspeed;
    bool canRestart;
    int score;
    // function
    void resetScene();
    void setRestart();
    void Blink();
    void die();
    void removePipe(cocos2d::Node *pipe);
    void spawnPipes();
    void update(float delta);
    
    void Run();
    
    // Contact Listener
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    // Touches
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    // end
    CREATE_FUNC(FlappyBird);
};

#endif // __HELLOWORLD_SCENE_H__
