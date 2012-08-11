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

class Allocator : public cocos2d::CCLayer
{
private:
    int m_width;
    
    MapMgr * m_pMapMgr;

    cocos2d::CCSprite * m_touch;
    
public:
    int m_type, m_id;
    std::vector<int> m_vec;
    
    ~Allocator();
    
    void init(MapMgr * mapMgr, int width, int type, int id);
    
    void Apply();
    
    void TouchesBegin(int i, int j);
    void TouchesMove();
    void TouchesEnd(int i, int j);
};

#endif
