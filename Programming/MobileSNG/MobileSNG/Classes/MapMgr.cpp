//
//  MapMgr.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 29..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "MapMgr.h"

using namespace std;

MapMgr::MapMgr(int & mapLevel, ObjectIndexMgr *pObjIdxMgr) : m_mapLevel(mapLevel)
{
    m_pObjIdxMgr = pObjIdxMgr;
}

MapMgr::~MapMgr()
{
    int size = m_vObjects.size();
    
    for(int i=0; i<size; ++i)
        delete m_vObjects[i];
    
    m_vObjects.clear();
}

bool MapMgr::_isInMap(POINT<int> &pos, SIZE<int> size)
{
    //이건 추후에 결정되어야 할거야 지금상으로는 얼마가 될지 모르겠거든.
    return true;
}

ObjectInMap* MapMgr::_CreateObject(ObjectInMap *pObject, ObjectInfoMgr *pInfoMgr, int time)
{
    OBJECT_TYPE     type    = pObject->GetType();
    ObjectInMap     *object = NULL;
    
    if( type == OBJECT_TYPE_BUILDING )
        object = dynamic_cast<ObjectInMap*>(new Building(pObject, time, pInfoMgr));
    
    else if( type == OBJECT_TYPE_ORNAMENT )
        object = dynamic_cast<ObjectInMap*>(new Ornament(pObject));
    
    else if( type == OBJECT_TYPE_FIELD )
        object = dynamic_cast<ObjectInMap*>(new Field(pObject));
    
    return object;
}

bool MapMgr::addCrop(Field *pField, int id, int time, ObjectInfoMgr *pInfoMgr)
{
    int idx = m_pObjIdxMgr->cropIndex();
    if(idx == -1) return false;

    if(pField->addCrop(id, time, idx, pInfoMgr))
    {
        m_pObjIdxMgr->addCropIndex(idx);
        return true;
    }
    
    return false;
}

void MapMgr::removeCrop(Field *pField)
{
    Crop *pCrop = pField->GetCrop();
    int index;
    if(pCrop)   index = pCrop->GetIndex();
    m_pObjIdxMgr->removeCropIndex(index);

    pField->removeCrop();
}
/*
void MapMgr::UpdateObjects(ObjectInfoMgr *pInfoMgr)
{
    vector<ObjectInMap*>::iterator iter;
    
    for( iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter )
        (*iter)->UpdateSystem(pInfoMgr);
}
*/
bool MapMgr::addObject(ObjectInMap *pInfo, ObjectInfoMgr *pInfoMgr, int time)
{
    ObjectInMap *object;
    int idx;

    if( (object = _CreateObject(pInfo, pInfoMgr, time)) == NULL )
    {
        printf("%s <- CreateObject Error, Can't alloc\n", __FUNCTION__);
        return false;
    }
    
    if(object->GetType() == OBJECT_TYPE_BUILDING)
    {
        idx = m_pObjIdxMgr->buildingIndex();

        if(idx == -1)
        {
            printf("%s <- Index Full\n", __FUNCTION__);
            delete object;
            return false;
        }

        object->SetIndex(idx);
    }
    
    m_pObjIdxMgr->addBuildingIndex(idx);
    m_vObjects.push_back(object);
    
    return true;
}

bool MapMgr::isObjectInMap(POINT<int> pos)
{
    vector<ObjectInMap*>::iterator iter;
    
    for( iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter )
    {
        if(intersectBoxWithPoint((*iter)->m_position, (*iter)->GetSize(), pos))
            return true;
    }
    
    return false;
}

bool MapMgr::isObjectInMap(POINT<int> pos, SIZE<int> size)
{
    vector<ObjectInMap*>::iterator iter;
    
    for( iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter )
    {
        if(intersectBoxWithBox((*iter)->m_position, (*iter)->GetSize(), pos, size))
            return true;
    }

    return false;
}

ObjectInMap* MapMgr::FindObject(POINT<int> pos)
{
    vector<ObjectInMap*>::iterator iter;
    
    for( iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter )
    {
        if(intersectBoxWithPoint((*iter)->m_position, (*iter)->GetSize(), pos))
            return (*iter);
    }
    
    return NULL;
}

vector<ObjectInMap*> MapMgr::FindObjects(POINT<int> pos, SIZE<int> size)
{
    vector<ObjectInMap*> vObjects;
    vector<ObjectInMap*>::iterator iter;
    
    for(iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter)
    {
        if(intersectBoxWithBox((*iter)->m_position, (*iter)->GetSize(), pos, size))
            vObjects.push_back((*iter));
    }
    
    return vObjects;
}

bool MapMgr::moveObject(POINT<int> &pos, ObjectInMap *obj2)
{    
    vector<ObjectInMap*>::iterator iter;
    SIZE<int> size = obj2->GetSize();
    
    if(_isInMap(pos, size) == false)
        return false;
    
    for(iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter)
    {        
        if(intersectBoxWithBox((*iter)->m_position, (*iter)->GetSize(), pos, size))
            return false;
    }
    
    obj2->m_position = pos;
    
    return true;
}

#pragma mark Remove

void MapMgr::removeObject(ObjectInMap *obj)
{
    vector<ObjectInMap*>::iterator iter;
    
    for(iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter)
    {
        if( (*iter) == obj )
        {
            m_vObjects.erase(iter);
            return;
        }
    }
}

void MapMgr::removeObject(POINT<int> &pos)
{
    vector<ObjectInMap*>::iterator iter;
    
    for(iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter)
    {
        if( intersectBoxWithPoint((*iter)->m_position, (*iter)->GetSize(), pos) )
        {
            m_vObjects.erase(iter);
            return;
        }
    }
}

void MapMgr::removeObjects(POINT<int> &pos, SIZE<int> &size)
{
    vector<ObjectInMap*>::iterator iter;
    
    for(iter = m_vObjects.begin(); iter != m_vObjects.end(); ++iter)
    {
        if( intersectBoxWithBox((*iter)->m_position, (*iter)->GetSize(), pos, size) )
            m_vObjects.erase(iter);
    }
}

#pragma mark harvest


#pragma mark -
#pragma mark Get

int MapMgr::GetMapLevel()
{
    return m_mapLevel;
}

vector<ObjectInMap *> MapMgr::GetAllObject()
{
    return m_vObjects;
}

#pragma mark -