FROM ubuntu:xenial

SHELL ["/bin/bash", "-c"]
ENV PYVER=3.5
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential ccache cmake git libffi-dev libgit2-dev pkg-config \
        software-properties-common unzip && \
    apt-get clean

RUN add-apt-repository -y ppa:deadsnakes/ppa && \
    apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        libpython3.5-dev python3.5-venv \
        libpython3.6-dev python3.6-venv && \
    apt-get clean

RUN python3.5 -m venv ~/venv3.5 && \
    . ~/venv3.5/bin/activate && \
    pip install wheel && \
    pip install google-cloud google-resumable-media matplotlib pygit2==0.24.2 pytest requests && \
    python3.6 -m venv ~/venv3.6 && \
    . ~/venv3.6/bin/activate && \
    pip install wheel && \
    pip install google-cloud google-resumable-media pytest requests && \
    rm -rf ~/.cache && \
    echo "source /root/venv\$PYVER/bin/activate" > ~/.bash_profile

ENTRYPOINT ["bash", "-lc", "exec $0 $@"]
