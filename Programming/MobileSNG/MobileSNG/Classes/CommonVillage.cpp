//
//  CommonVillage.cpp
//  MobileSNG
//
//  Created by 박 진 on 12. 9. 2..
//
//

#include "CommonVillage.h"
#include "Building.h"
#include "Crop.h"
#include "Field.h"

using namespace std;
using namespace rapidxml;

CommonVillage::CommonVillage(Network *pNetwork) : m_pNetwork(pNetwork), m_pInfoMgr(NULL)
{
    m_pInfoMgr = new ObjectInfoMgr;
}

CommonVillage::~CommonVillage()
{
    SAFE_DELETE(m_pInfoMgr);
}

bool CommonVillage::_networkNormalResult(CURL_DATA *pData)
{
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(pData->pContent);
    
    if(xmlDoc.first_node() == NULL) return false;
    
    const char *value = xmlDoc.first_node()->first_node()->value();
    
    if(strcmp(value, "false") == 0)
        return false;
    
    return true;
}

void CommonVillage::_getCropState(objectState *pState)
{
    switch (*pState)
    {
        case NETWORK_OBJECT_WAITTING: *pState = CROP_STATE_INVAILD; break;
        case NETWORK_OBJECT_FAIL:     *pState = CROP_STATE_FAIL;    break;
        case NETWORK_OBJECT_WORKING:  *pState = CROP_STATE_GROW_1;  break;
        case NETWORK_OBJECT_DONE:     *pState = CROP_STATE_DONE;    break;
    }
}

void CommonVillage::_getObjectState(objectState *pState)
{
    switch (*pState)
    {
        case NETWORK_OBJECT_CONSTRUCTION:
            *pState = BUILDING_STATE_UNDER_CONSTRUCTION_1;
            break;
        case NETWORK_OBJECT_FRIEND_WORKING:
            *pState = BUILDING_STATE_OTEHR_WORKING;
            break;
            
        case NETWORK_OBJECT_DONE:
        case NETWORK_OBJECT_FRIEND_DONE:
            *pState = BUILDING_STATE_DONE;
            break;
            
        case NETWORK_OBJECT_WORKING:
            *pState = BUILDING_STATE_WORKING;
            break;
    }
}

std::vector< std::pair<ObjectInMap, double> > CommonVillage::_parseBuildingInVillage(const char* pContent)
{
    vector< pair<ObjectInMap, double> > v;
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(const_cast<char*>(pContent));
    
    xml_node<char> *pRoot = xmlDoc.first_node()->first_node();
    
    int count = atoi(pRoot->value());
    pRoot = pRoot->next_sibling();
    
    DateInfo serverDate;
    if( _getServerTime(&serverDate) == false )
    {
        printf("%s <- Error\n", __FUNCTION__);
        return v;
    }
    
    for(int i=0; i<count; ++i, pRoot = pRoot->next_sibling())
    {
        xml_node<char> *pNode = pRoot->first_node();
        
        int index = atoi(pNode->value());
        pNode = pNode->next_sibling();
        int id = atoi(pNode->value());
        pNode = pNode->next_sibling();
        
        OBJECT_TYPE type;
        objectState state = atoi(pNode->value());
        SIZE<int> size;
        
        if( index >= 1000 )
        {
            _getCropState(&state);
            
            type = OBJECT_TYPE_FIELD;
            size = SIZE<int>(1,1);
        }
        else
        {
            _getObjectState(&state);

            type = OBJECT_TYPE_BUILDING;
                        
            BuildingInfo *pInfo;
            m_pInfoMgr->searchInfo(id, &pInfo);
            size = pInfo->GetSize();
        }
        pNode = pNode->next_sibling();

        int posValue = atoi(pNode->value());
        POINT<int> pos;
        pos.x = GETWORD_X(posValue);
        pos.y = GETWORD_Y(posValue);
        pNode = pNode->next_sibling();

        OBJECT_DIRECTION dir = static_cast<OBJECT_DIRECTION>(atoi(pNode->value()));
        pNode = pNode->next_sibling();

        const char *pDate = pNode->value();
        DateInfo date;
        date.UpdateDate(pDate);
        
        double deltaTime = date.GetTimeValue(serverDate);
        
        ObjectInMap obj = ObjectInMap(state, pos, size, dir, id, index);
        obj.SetType(type);
        
        pair<ObjectInMap, double> value(obj, deltaTime);
        v.push_back(value);
    }
    
    return v;
}

vector< pair<int, int> > CommonVillage::_parseCropInVillage(const char* pContent)
{
    vector< pair<int, int> > v;
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(const_cast<char*>(pContent));
    
    xml_node<char> *pRoot = xmlDoc.first_node()->first_node();

    int count = atoi(pRoot->value());
    pRoot = pRoot->next_sibling();
    
    for(int i=0; i<count; ++i, pRoot = pRoot->next_sibling())
    {
        xml_node<char> *pNode = pRoot->first_node();
        
        int index = atoi(pNode->value());
        pNode = pNode->next_sibling();
        int cropID = atoi(pNode->value());
        
        pair<int, int> value(index, cropID);
        v.push_back(value);
    }
    
    return v;
}

bool CommonVillage::_getServerTime(DateInfo *pInfo)
{
    CURL_DATA data;
    if(m_pNetwork->connectHttp("http://swmaestros-sng.appspot.com/timeprint", &data) != CURLE_OK)
    {
        printf("%s <- Error Get Server Time\n", __FUNCTION__);
        return false;
    }
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(data.pContent);
    
    xml_node<char> *pNode = xmlDoc.first_node()->first_node();
    pInfo->UpdateDate(pNode->value());
    
    return true;
}

int CommonVillage::_findFieldTime(int index, std::vector< std::pair<int, int> > *pvData)
{
    if(pvData == NULL) return -1;
    
    vector< pair<int, int> >::iterator iter;
    
    for(iter = pvData->begin(); iter != pvData->end(); ++iter)
    {
        if((*iter).first == index)
            return (*iter).second;
    }
    
    return -1;
}