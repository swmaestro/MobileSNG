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
#include <vector>

class MapMgr;

class Editor : public cocos2d::CCLayer
{
private:
    int m_width;
    
    MapMgr * m_pMapMgr;

    cocos2d::CCSprite * m_touch;
    
public:
    bool m_isSetter;
    int m_setType, m_setID;
    std::vector<int> m_setVec;
    
    ~Editor();
    
    void init(MapMgr * mapMgr, int width);
    void init(MapMgr * mapMgr, int width, int type, int id);
    
    void Apply();
    
    void TouchesBegin(int i, int j);
    void TouchesMove();
    void TouchesEnd(int i, int j);
};

#endif
