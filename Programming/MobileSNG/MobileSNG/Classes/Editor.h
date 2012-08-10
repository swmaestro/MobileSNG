//
//  Editor.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 10..
//
//

#ifndef MobileSNG_Editor_h
#define MobileSNG_Editor_h

#include "cocos2d.h"

class Editor : public cocos2d::CCLayer
{
private:
    int m_width;
    
    bool m_isSetter;
    int m_setType, m_setID;
    
    cocos2d::CCSprite * m_touch;
    
public:
    ~Editor();
    
    void init(int width);
    void init(int width, int type, int id);
    
    void TouchesBegin(int i, int j);
    void TouchesMove();
    void TouchesEnd(int i, int j);
};

#endif
