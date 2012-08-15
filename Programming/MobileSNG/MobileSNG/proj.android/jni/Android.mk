LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := helloworld/main.cpp \
		   ../../Classes/Allocator.cpp \
		   ../../Classes/AppDelegate.cpp \
		   ../../Classes/Building.cpp \
		   ../../Classes/Crop.cpp \
                   ../../Classes/Field.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/GameSystem.cpp \
                   ../../Classes/Map.cpp \
                   ../../Classes/MapMgr.cpp \
                   ../../Classes/Network.cpp \
                   ../../Classes/ObjectInfoMgr.cpp \
                   ../../Classes/ObjectInMap.cpp \
                   ../../Classes/Ornament.cpp \
                   ../../Classes/Shop.cpp \
                   ../../Classes/Sqlite3Base.cpp \
                   ../../Classes/Timer.cpp \
                   ../../Classes/UIMgr.cpp 
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    ../../Classes \
                    /Users/code_athei/Documents/Dev/Project/MobileSNG/Programming/MobileSNG/MobileSNG/curl/header/ \
                    /Users/code_athei/Documents/Dev/Project/MobileSNG/Programming/MobileSNG/MobileSNG/libs/cocos2dx/platform/ \
                    /Users/code_athei/Documents/Dev/Project/MobileSNG/Programming/MobileSNG/MobileSNG/libs/cocos2dx/platform/third_party/android \

LOCAL_LDLIBS := -L$(call host-path, /Users/code_athei/Documents/Dev/Project/MobileSNG/Programming/MobileSNG/MobileSNG/libs/cocos2dx/platform/third_party/android/libraries/$(TARGET_ARCH_ABI)) -lcurl

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) $(call import-module,cocos2dx)
