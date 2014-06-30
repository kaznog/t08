#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Maze.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
private:
    Vec2 tileCoordForPosition(Vec2 position);
    void setPlayerPosition(Vec2 position);
    void setViewPointCenter(Vec2 position);
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    TMXLayer* _meta;
    
    Maze* _maze;
    Sprite* _player;
};

#endif // __HELLOWORLD_SCENE_H__
