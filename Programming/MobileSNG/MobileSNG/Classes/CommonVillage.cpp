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

CommonVillage::CommonVillage(Network *pNetwork)
{
    m_pNetwork = pNetwork;
    m_pInfoMgr = new ObjectInfoMgr;
}

CommonVillage::~CommonVillage()
{
    SAFE_DELETE(m_pInfoMgr);
}

bool CommonVillage::_networkNormalResult(rapidxml::xml_document<char> *pXMLDoc)
{
    const char *value = pXMLDoc->first_node()->first_node()->value();
    
    if(strcmp(value, "false") == 0)
        return false;
    
    return true;
}

std::vector< std::pair<ObjectInMap, long long int> > CommonVillage::_parseObjectInVillage(const char* pContent)
{
    vector< pair<ObjectInMap, long long int> > v;
    
    xml_document<char> xmlDoc;
    xmlDoc.parse<0>(const_cast<char*>(pContent));
    
    printf("%s\n", pContent);
    
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
            type = OBJECT_TYPE_CROP;
            
            if(state == NETWORK_OBJECT_DONE)
                state = CROP_STATE_DONE;
            else if( state == NETWORK_OBJECT_FAIL)
                state = CROP_STATE_FAIL;
            else state = CROP_STATE_GROW_1;
        }
        else
        {
            if( id != -1 )
            {
                type = OBJECT_TYPE_BUILDING;
                
                switch (state) {
                    case NETWORK_OBJECT_CONSTRUCTION:   state = BUILDING_STATE_UNDER_CONSTRUCTION_1;    break;
                    case NETWORK_OBJECT_WORKING:        state = BUILDING_STATE_WORKING;                 break;
                    case NETWORK_OBJECT_DONE:           state = BUILDING_STATE_DONE;                    break;
                    case NETWORK_OBJECT_OTHER_WATTING:  state = BUILDING_STATE_OTEHR_WORKING;           break;
                    default:break;
                }
                
                BuildingInfo *pInfo;
                m_pInfoMgr->searchInfo(id, &pInfo);
                size = pInfo->GetSize();
            }
            else
            {
                type = OBJECT_TYPE_FIELD;
                state = 0;
                size = SIZE<int>(1,1);
            }
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
        
        long long int deltaTime = date.GetTimeValue(serverDate);
        
        ObjectInMap obj = ObjectInMap(state, pos, size, dir, id, index);
        obj.SetType(type);
        
        pair<ObjectInMap, long long int> value(obj, deltaTime);
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