#include "HelloWorldScene.h"
#include "HudLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer, 0);
    auto hud = new HudLayer();
    hud->init();
    scene->addChild(hud, 1);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _maze = new Maze();
    _maze->autorelease();
    _maze->init();
    _maze->make();
    _maze->rmx = _maze->startx;
    _maze->rmy = _maze->starty;
    Vec2 mappos = _maze->getpos();
    
    _maze->getroom();
    Vec2 downpos = _maze->getpos();
    
    
    _tileMap    = TMXTiledMap::create("basemap.tmx");
    _background = _tileMap->getLayer("Background");
    _meta       = _tileMap->getLayer("Meta");
    _meta->removeTileAt(Vec2(0, 0));
    _meta->setTileGID(TILECOORD_DOWNSTAIRS, downpos);
    
    this->addChild(_tileMap);

    mappos.x = _tileMap->getTileSize().width * mappos.x + (_tileMap->getTileSize().width / 2);
    mappos.y = _tileMap->getTileSize().height * _tileMap->getMapSize().height - (mappos.y * _tileMap->getTileSize().height + (_tileMap->getTileSize().height / 2));
    _player = Sprite::create("Player.png");
    _player->setPosition(mappos);
    this->addChild(_player, ZORDER_PLAYER);
    this->setViewPointCenter(_player->getPosition());
    
    for (int sy = 0; sy < MAP_TILE_HEIGHT; sy++) {
        for (int sx = 0; sx < MAP_TILE_WIDTH; sx++) {
            if (_maze->map1[sy][sx] == 0) {
                _background->setTileGID(TILECOORD_FLOOR, Vec2(sx, sy));
            } else if (_maze->map1[sy][sx] == 1 || _maze->map1[sy][sx] == 2) {
                _background->setTileGID(TILECOORD_WALL, Vec2(sx, sy));
            }
            
        }
    }
    this->enableTouchEvent(true);
    return true;
}

void HelloWorld::setViewPointCenter(cocos2d::Vec2 position)
{
    auto winSize = Director::getInstance()->getWinSize();
    float x = MAX(position.x, winSize.width / 2);
    float y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
    auto actualPosition = Vec2(x, y);
    auto centerOfView = Vec2(winSize.width/2, winSize.height/2);
    auto viewPoint = centerOfView - actualPosition;
    this->setPosition(viewPoint);
}

void HelloWorld::setPlayerPosition(cocos2d::Vec2 position)
{
    Vec2 tileCoord = this->tileCoordForPosition(position);
    int tileGid = _background->getTileGIDAt(tileCoord, nullptr);
    if (tileGid == TILECOORD_WALL) {
        return;
    } else {
        tileGid = _meta->getTileGIDAt(tileCoord, nullptr);
        if (tileGid == TILECOORD_DOWNSTAIRS) {
            Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.5f, HelloWorld::createScene()));
        }
    }
    _player->setPosition(position);
}

Vec2 HelloWorld::tileCoordForPosition(cocos2d::Vec2 position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    
    return Vec2(x, y);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#pragma mark -
#pragma mark touch events
void HelloWorld::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    auto location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    
    location = this->convertToNodeSpace(location);
    
    Vec2 playerPos = _player->getPosition();
    Vec2 diff = location - playerPos;
    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
    
    if (playerPos.x < (_tileMap->getMapSize().width * _tileMap->getTileSize().width)
        &&  playerPos.y < (_tileMap->getMapSize().height * _tileMap->getTileSize().height)
        &&  playerPos.y >= 0
        &&  playerPos.x >= 0
        ) {
        this->setPlayerPosition(playerPos);
    }
    this->setViewPointCenter(_player->getPosition());
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
}

#pragma mark -