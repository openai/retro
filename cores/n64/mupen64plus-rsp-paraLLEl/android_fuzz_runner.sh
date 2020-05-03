#!/bin/bash

RSP_FUZZ="$1"

adb push "$RSP_FUZZ" /data/local/tmp/rsp-vu-fuzzer >/dev/null || exit 1
adb shell chmod +x /data/local/tmp/rsp-vu-fuzzer >/dev/null || exit 1
adb shell /data/local/tmp/rsp-vu-fuzzer || exit 1
adb shell rm /data/local/tmp/rsp-vu-fuzzer || exit 1
