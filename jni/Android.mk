LOCAL_PATH := $(call my-dir)
LOCAL_CPP_EXTENSION:=.cpp 

include $(CLEAR_VARS)

LOCAL_MODULE    := udt
LOCAL_SRC_FILES := md5.cpp common.cpp window.cpp list.cpp buffer.cpp packet.cpp channel.cpp queue.cpp ccc.cpp cache.cpp core.cpp epoll.cpp api.cpp 

#LOCAL_CFLAGS += -I/usr/include/c++/4.7.1 
#LOCAL_CFLAGS += -I/usr/include/4.7.1/

#LOCAL_C_INCLUDES += /usr/include/c++/4.7.1
#LOCAL_C_INCLUDES += /usr/include/c++/4.7.1/x86_64-unknown-linux-gnu

LOCAL_CFLAGS += -fexceptions

#LOCAL_C_INCLUDES += /opt2/adt-bundle-linux-x86_64/android-ndk-r8d/sources/cxx-stl/gnu-libstdc++/4.7/include
#LOCAL_C_INCLUDES += /opt2/adt-bundle-linux-x86_64/android-ndk-r8d/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi/include/

include $(BUILD_SHARED_LIBRARY)
