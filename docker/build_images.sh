#!/bin/sh

cd "$(dirname $0)/../"
./docker/linux/build_scripts/prefetch.sh OPENSSL CURL
docker build . --tag openai/retro-build:linux-v2 --file docker/linux/Dockerfile
docker push openai/retro-build:linux-v2
docker build . --tag openai/retro-build:win64-v2 --file docker/windows/Dockerfile
docker push openai/retro-build:win64-v2