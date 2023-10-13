FROM tylern4/root:6.26.04


ENV HIPO_TOOLS=/usr/local/hipo_tools
ENV PATH=$PATH:$HIPO_TOOLS/bin
ENV LD_LIBRARY_PATH=$HIPO_TOOLS:/opt/conda/lib
# ENV PYTHONPATH=$HIPO_TOOLS:/usr/local/lib/root

RUN mamba install ninja cmake

COPY . /source
WORKDIR /source
RUN export CC=$(root-config --cc) && \
	export CXX=$(root-config --cxx) && \
	mkdir build && cd build && \
	cmake -G Ninja .. -DCMAKE_INSTALL_PREFIX=${HIPO_TOOLS} && \
	ninja -j 8 install

