//
//  HudLayer.h
//  t08
//
//  Created by 野口一也 on 2014/06/27.
//
//

#ifndef __t08__HudLayer__
#define __t08__HudLayer__

#include "cocos2d.h"

USING_NS_CC;

class HudLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HudLayer);
};

#endif /* defined(__t08__HudLayer__) */
