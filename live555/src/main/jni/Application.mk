# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information

ifeq (,$(PROJECT_PATH))

# 项目路径(绝对路径): jni 上一级目录路径(使用 AS 则为 module 路径)
PROJECT_PATH := $(call find-project-dir, ., .)
# SVN 路径
LINUX_WORKS = $(PROJECT_PATH)/../library/android-libs

else

# PROJECT_PATH 由 build.bat 脚本传递: jni 目录
LINUX_WORKS = $(PROJECT_PATH)/../../../library/android-libs

endif

# APP_ABI := all
# APP_ABI := arm64-v8a armeabi armeabi-v7a mips mips64 x86 x86_64
APP_ABI := armeabi-v7a

APP_PLATFORM := android-21

APP_STL := c++_static