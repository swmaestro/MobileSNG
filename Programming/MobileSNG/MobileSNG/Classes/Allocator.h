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

class GameSystem;
class Map;

class Allocator
{
private:
    int & m_width;
    
    GameSystem * m_pSystem;
    
    int m_orgSwt;
    
    int m_type, m_id;
    std::string m_name;
    std::vector<int> m_vec;
    
    cocos2d::CCSprite * m_touch;
    cocos2d::CCLayer *& m_tile;
    
    Map *m_pMap;
    
public:
    Allocator(cocos2d::CCLayer *& tile, int & width, Map *pMap);
    
    void init(GameSystem * system, int type, int id);
    
    void Apply();
    void Cancel();
    void Clear();
    
    void TouchesBegin(int i, int j);
    void TouchesMove();
    void TouchesEnd();
};

#endif
