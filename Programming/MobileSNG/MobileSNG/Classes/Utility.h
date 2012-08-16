//
//  Utility.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

template<typename TYPE>
struct POINT
{
    TYPE x, y;
    
    POINT(TYPE _x = 0, TYPE _y = 0)
    {
        x = _x;
        y = _y;
    }
};

template<typename TYPE>
struct SIZE
{
    TYPE width, height;
    
    SIZE(TYPE _w = 0, TYPE _h = 0)
    {
        width   = _w;
        height  = _h;
    }
};

template <typename TYPE>
inline bool intersectBoxWithBox(POINT<TYPE> pos1, SIZE<TYPE> size1,
                                POINT<TYPE> pos2, SIZE<TYPE> size2)
{
    if( pos2.x > pos1.x + size1.width )
        return false;
    if( pos2.x + size2.width < pos1.x )
        return false;
    if( pos2.y > pos1.y + size1.height )
        return false;
    if( pos2.y + size2.height < pos2.y )
        return false;
    
    return true;
}

template <typename TYPE>
inline bool intersectBoxWithPoint(POINT<TYPE> boxPos, SIZE<TYPE> boxSize, POINT<TYPE> pos)
{
    if( (boxPos.x <= pos.x && pos.x < boxPos.x + boxSize.width) &&
       (boxPos.y <= pos.y && pos.y < boxPos.y + boxSize.height) )
        return true;
    
    return false;
}
