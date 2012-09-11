//
//  PlayerMap.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 9. 9..
//
//

#ifndef __MobileSNG__PlayerMap__
#define __MobileSNG__PlayerMap__

#include "Map.h"

class PlayerMap : public Map
{
private:
    Allocator         * m_pAllocator;
    
    bool                m_isAllocating, m_isEditing;
    
public:
    PlayerMap(int width);
    ~PlayerMap();
    
    virtual bool init(GameSystem * system);
    virtual void update(float dt);
    
    void beginEdit();
    void beginEdit(int type, int id);
    void endEdit(bool apply);
    
    virtual void ccTouchesBegan(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
};

#endif /* defined(__MobileSNG__PlayerMap__) */
