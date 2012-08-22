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
class ObjectInfoMgr;

class Allocator : public cocos2d::CCLayer
{
private:
    int & m_width;
    
    MapMgr * m_pMapMgr;
    ObjectInfoMgr * m_pInfoMgr;
    
    int m_type, m_id;
    std::string m_name;
    std::vector<int> m_vec;
    
    cocos2d::CCSprite * m_touch;
    cocos2d::CCLayer *& m_tile;
    
public:
    Allocator(cocos2d::CCLayer *& tile, int & width);
    
    void init(MapMgr * mapMgr, ObjectInfoMgr * infoMgr, int type, int id);
    
    void Apply();
    void Clear();
    
    void TouchesBegin(int i, int j);
    void TouchesMove();
    void TouchesEnd();
};

#endif
