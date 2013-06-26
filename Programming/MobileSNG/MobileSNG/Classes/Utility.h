//
//  Utility.h
//  MobileSNG
//
//  Created by 박 진 on 12. 8. 17..
//
//

#pragma once

#include <queue>
#include "cocos2d.h"

#define HIWORD(l)   ((short) (((long) ((l) >> 16)) & 0x0000FFFF))
#define LOWORD(l)   ((short) ((l) & 0x0000FFFF))

#define GETWORD_X(w) LOWORD(w)
#define GETWORD_Y(w) HIWORD(w)

#define MAKEWORD(x,y) ((long)((LOWORD(x) & 0x0000FFFF)|((((long)LOWORD(y)) << 16) & 0xFFFF0000)))

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
    //CA Edit
    //>, < to >=, <=
    
    if( pos2.x >= pos1.x + size1.width )
        return false;
    if( pos2.x + size2.width <= pos1.x )
        return false;
    if( pos2.y >= pos1.y + size1.height )
        return false;
    if( pos2.y + size2.height <= pos1.y )
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

template <typename T>
static void stdQueueAllClear(std::queue<T> &stdQueue)
{
    while (stdQueue.empty()==false)
        stdQueue.pop();
}

static void TextFieldKeyBoardOn(cocos2d::CCNode *pNode, cocos2d::CCSet *pTouches, cocos2d::CCTextFieldTTF **ppTextFields, int num = 1)
{
    if(pNode->isVisible() == false)
        return;
    
    cocos2d::CCPoint point;
    cocos2d::CCTouch * pTouch = static_cast<cocos2d::CCTouch *>(*(pTouches->begin()));
    
    point = pTouch->locationInView();
    point = cocos2d::CCDirector::sharedDirector()->convertToGL(point);
    point.x -= pNode->getPositionX();
    point.y -= pNode->getPositionY();
    
    //뭐 여기서 남은 작업해주면되
    POINT<int> boxPos;
    SIZE<int>  boxSize;
    POINT<int> touchPoint = POINT<int>(point.x, point.y);
    
    for (int i=0; i<num; ++i) {
        boxPos.x = ppTextFields[i]->getPositionX();
        boxPos.y = ppTextFields[i]->getPositionY();
        boxSize.width = ppTextFields[i]->getContentSize().width;
        boxSize.height = ppTextFields[i]->getContentSize().height;
        
        if(intersectBoxWithPoint(boxPos, boxSize, touchPoint))
        {
            ppTextFields[i]->attachWithIME();
            break;
        }
        
        printf("%f %f \n", ppTextFields[i]->getAnchorPoint().x, ppTextFields[i]->getAnchorPoint().y);
    }
    
    printf("x %f, y %f \n", point.x, point.y);
}

static bool isExistFile(const char* path)
{
    bool has = false;
    
    FILE *p = fopen(path, "rb");
    has = p;
    fclose(p);
    
    return has;
}

#define SAFE_DELETE(P) if(P){delete P; P = NULL;}
