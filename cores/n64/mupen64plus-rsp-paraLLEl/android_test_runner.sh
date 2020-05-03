#!/bin/bash

RSP_RUNNER="$1"
RSP_BINFOLDER="$2"
TEST_NAME="$3"

adb push "$RSP_RUNNER" /data/local/tmp/rsp-runner >/dev/null || exit 1
adb shell chmod +x /data/local/tmp/rsp-runner >/dev/null || exit 1
adb push "$RSP_BINFOLDER"/"$TEST_NAME".global.bin /data/local/tmp/ >/dev/null || exit 1
adb push "$RSP_BINFOLDER"/"$TEST_NAME".bin /data/local/tmp/ >/dev/null || exit 1
adb shell /data/local/tmp/rsp-runner /data/local/tmp/"$TEST_NAME".global.bin /data/local/tmp/"$TEST_NAME".bin || exit 1
adb shell rm /data/local/tmp/rsp-runner >/dev/null || exit 1
adb shell rm /data/local/tmp/"$TEST_NAME".global.bin /data/local/tmp/"$TEST_NAME".bin >/dev/null || exit 1
