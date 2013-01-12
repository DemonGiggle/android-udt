LOCAL_PATH := $(call my-dir)
LOCAL_CPP_EXTENSION:=.cpp 

include $(CLEAR_VARS)
LOCAL_MODULE    := udt
LOCAL_SRC_FILES := lib/md5.cpp lib/common.cpp lib/window.cpp lib/list.cpp lib/buffer.cpp lib/packet.cpp lib/channel.cpp lib/queue.cpp lib/ccc.cpp lib/cache.cpp lib/core.cpp lib/epoll.cpp lib/api.cpp 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fexceptions
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := appclient
LOCAL_SRC_FILES := app/appclient.cpp 
LOCAL_CFLAGS += -I$(LOCAL_PATH)/lib 
LOCAL_SHARED_LIBRARIES := udt
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := appserver
LOCAL_SRC_FILES := app/appserver.cpp
LOCAL_CFLAGS += -I$(LOCAL_PATH)/lib 
LOCAL_SHARED_LIBRARIES := udt
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := recvfile
LOCAL_SRC_FILES := app/recvfile.cpp
LOCAL_CFLAGS += -I$(LOCAL_PATH)/lib 
LOCAL_SHARED_LIBRARIES := udt
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := sendfile
LOCAL_SRC_FILES := app/sendfile.cpp
LOCAL_CFLAGS += -I$(LOCAL_PATH)/lib 
LOCAL_SHARED_LIBRARIES := udt
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := app/test.cpp
LOCAL_CFLAGS += -I$(LOCAL_PATH)/lib 
LOCAL_SHARED_LIBRARIES := udt
include $(BUILD_EXECUTABLE)
