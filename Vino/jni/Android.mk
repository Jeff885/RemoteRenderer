LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := quicklz
LOCAL_C_INCLUDE := quick
LOCAL_SRC_FILES := libquicklz.a

LOCAL_ARM_MODE := arm

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := osgutilize
LOCAL_C_INCLUDE := osg
LOCAL_SRC_FILES := libosgutilize.a

LOCAL_ARM_MODE := arm

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := jpeg-turbo
LOCAL_C_INCLUDE := libjpeg
LOCAL_SRC_FILES := libjpeg.a

LOCAL_ARM_MODE := arm

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_ARM_NEON := true
endif

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := nativeRender
LOCAL_SRC_FILES := VinoNativeRender.cpp VinoRender.cpp \
					Manipulator.cpp classUtility.cpp \
					GlobalInfo.cpp DepthImageBase.cpp \
					CFirstPersonManipulator.cpp CameraOperator.cpp \
					
LOCAL_STATIC_LIBRARIES := osgutilize jpeg-turbo quicklz
LOCAL_LDLIBS    := -llog -lGLESv2

LOCAL_ARM_MODE := arm

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_ARM_NEON := true
endif

include $(BUILD_SHARED_LIBRARY)