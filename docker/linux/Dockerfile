FROM jeromerobert/centos5-gcc5

ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8
ENV PATH /usr/local/bin:/opt/rh/devtoolset-2/root/usr/bin:$PATH
ENV LD_LIBRARY_PATH /opt//opt/rh/devtoolset-2/root/usr/lib64:/opt/rh/devtoolset-2/root/usr/lib:/usr/local/lib64:/usr/local/lib
ENV PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

WORKDIR /
COPY docker/linux/build_scripts /build_scripts
COPY docker/linux/sources /
RUN bash build_scripts/build.sh && rm -r build_scripts
RUN yum install -y zlib-devel

RUN /opt/python/cp35-cp35m/bin/python3.5 -m venv ~/venv3.5 && \
    . ~/venv3.5/bin/activate && \
    pip install wheel && \
    pip install google-auth google-cloud-storage pytest requests && \
    /opt/python/cp36-cp36m/bin/python3.6 -m venv ~/venv3.6 && \
    . ~/venv3.6/bin/activate && \
    pip install wheel && \
    pip install google-auth google-cloud-storage pytest requests && \
    rm -rf ~/.cache && \
    ln -s ~/venv3.5 ~/venv && \
    echo "source /root/venv\$PYVER/bin/activate" > ~/.bash_profile

WORKDIR /root
COPY docker/scripts scripts
COPY docker/cmake cmake

COPY third-party/libzip libzip
RUN ROOT=/usr ./scripts/build_libzip.sh && \
    rm -rf libzip

ENV SSL_CERT_FILE=/opt/_internal/certs.pem
ENV PYVER=3.5

ENTRYPOINT ["/bin/bash", "-lc", "exec $0 $@"]
