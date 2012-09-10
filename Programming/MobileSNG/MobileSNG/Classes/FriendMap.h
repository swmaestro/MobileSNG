//
//  FriendMap.h
//  MobileSNG
//
//  Created by 건우 강 on 12. 9. 9..
//
//

#ifndef __MobileSNG__FriendMap__
#define __MobileSNG__FriendMap__

#include "Map.h"
#include "FriendVillage.h"

class FriendMap : public Map
{
public:
    FriendMap(int & width, FriendVillage * vill);
};

#endif /* defined(__MobileSNG__FriendMap__) */