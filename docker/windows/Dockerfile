FROM ubuntu:xenial

SHELL ["/bin/bash", "-c"]
ARG ARCH=x86_64
ARG BITS=64
ENV PYVER=3.5
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential ccache cmake curl g++-mingw-w64-`echo $ARCH | tr _ -` \
        git libz-mingw-w64-dev p7zip-full pkg-config \
        software-properties-common unzip zip && \
    apt-get clean

RUN apt-add-repository -y ppa:deadsnakes/ppa && \
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
COPY docker/scripts scripts
COPY docker/cmake cmake

RUN CROSS=win${BITS} ROOT=/usr/${ARCH}-w64-mingw32 ./scripts/install_python.sh

COPY third-party/libzip libzip
RUN CROSS=win${BITS} ROOT=/usr/${ARCH}-w64-mingw32 ./scripts/build_libzip.sh && \
    rm -rf libzip

ENTRYPOINT ["bash", "-lc", "exec $0 $@"]
