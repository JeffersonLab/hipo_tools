FROM condaforge/mambaforge:4.14.0-0
USER root

ENV HIPO_TOOLS /usr/local/hipo_tools
ENV PATH $PATH:$HIPO_TOOLS/bin
ENV LD_LIBRARY_PATH $HIPO_TOOLS/lib:/usr/local/lib/root
ENV PYTHONPATH $HIPO_TOOLS/lib:/usr/local/lib/root

RUN mamba install -y -c conda-forge root numpy cython cmake make

ADD . /tmp/hipo_tools/
RUN mkdir -p /tmp/hipo_tools/build && cd /tmp/hipo_tools/build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=$HIPO_TOOLS \
    && make -j$(nproc) \
    && make install \
    && rm -rf /tmp/*
