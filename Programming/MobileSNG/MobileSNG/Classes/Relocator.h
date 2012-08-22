//
//  Relocator.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 8. 20..
//
//

#ifndef __MobileSNG__Relocator__
#define __MobileSNG__Relocator__

#include "cocos2d.h"
#include <vector>

class MapMgr;
class ObjectInfoMgr;

class Relocator : public cocos2d::CCLayer
{
private:
    int & m_width;
    
    MapMgr * m_pMapMgr;
    ObjectInfoMgr * m_pInfoMgr;
    
    cocos2d::CCLayer *& m_tile;
    
public:
    Relocator(cocos2d::CCLayer *& tile, int & width);
    
    void init(MapMgr * mapMgr);
    
    void Apply();
    void Clear();
    
    void TouchesBegin(int i, int j);
    void TouchesMove(int i, int j);
    void TouchesEnd(int i, int j);
};

#endif /* defined(__MobileSNG__Relocator__) */
