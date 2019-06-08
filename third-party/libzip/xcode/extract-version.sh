#!/bin/sh

# Replace the value for PLIST_KEY with the resolved definition from the header file that was passed in.

SOURCE_HEADER_FILE_PATH=$1
SOURCE_PLIST_PATH=$2

PLIST_KEY="CFBundleShortVersionString"

VERSION_KEY=`/usr/libexec/PlistBuddy -c "Print :${PLIST_KEY}" "${SOURCE_PLIST_PATH}"`

#echo "Key: ${VERSION_KEY}"

VERSION_NUM=`cat "${SOURCE_HEADER_FILE_PATH}" | sed -n "s|#define ${VERSION_KEY} \"\(.*\)\".*|\1|p"`

#echo "Value: ${VERSION_NUM}"

TARGET_PLIST_PATH="${TARGET_BUILD_DIR}/${INFOPLIST_PATH}"

/usr/libexec/PlistBuddy -c "Set :${PLIST_KEY} ${VERSION_NUM}" "${TARGET_PLIST_PATH}"
