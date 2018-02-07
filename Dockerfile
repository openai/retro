FROM ubuntu:xenial

SHELL ["/bin/bash", "-c"]
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential ccache cmake git libffi-dev libgit2-dev libpython3-dev \
        libzip-dev pkg-config python3-venv && \
    apt-get clean && \
    python3 -m venv ~/venv && \
    . ~/venv/bin/activate && \
    pip install wheel && \
    pip install google-cloud google-resumable-media matplotlib pygit2==0.24.2 pytest requests && \
    rm -r ~/.cache && \
    echo "source /root/venv/bin/activate" > ~/.bash_profile

ENTRYPOINT ["bash", "-lc", "exec $0 $@"]
