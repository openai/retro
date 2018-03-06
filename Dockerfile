FROM ubuntu:xenial

SHELL ["/bin/bash", "-c"]
ENV PYVER=3.5
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential ccache cmake curl git libffi-dev libz-mingw-w64-dev \
        mingw-w64 p7zip-full pkg-config software-properties-common unzip \
        zip && \
    apt-get clean

RUN apt-add-repository -y ppa:deadsnakes/ppa && \
    apt-add-repository -y ppa:tobydox/mingw-w64 && \
    apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        libpython3.5-dev python3.5-venv \
        libpython3.6-dev python3.6-venv && \
    apt-get clean

RUN python3.5 -m venv ~/venv3.5 && \
    . ~/venv3.5/bin/activate && \
    pip install wheel && \
    pip install google-cloud google-resumable-media pytest requests && \
    python3.6 -m venv ~/venv3.6 && \
    . ~/venv3.6/bin/activate && \
    pip install wheel && \
    pip install google-cloud google-resumable-media pytest requests && \
    rm -rf ~/.cache && \
    ln -s ~/venv3.5 ~/venv && \
    echo "source /root/venv\$PYVER/bin/activate" > ~/.bash_profile

WORKDIR /root
COPY scripts scripts
COPY cmake cmake
COPY third-party/libzip libzip
RUN CROSS=win64 ROOT=/usr/x86_64-w64-mingw32 ./scripts/build_deps.sh

ENTRYPOINT ["bash", "-lc", "exec $0 $@"]
