FROM condaforge/mambaforge:4.14.0-0
USER root

ENV HIPO_TOOLS /usr/local/hipo_tools
ENV PATH $PATH:$HIPO_TOOLS/bin
ENV LD_LIBRARY_PATH $HIPO_TOOLS/lib:/usr/local/lib/root
ENV PYTHONPATH $HIPO_TOOLS/lib:/usr/local/lib/root

RUN mamba install -y -c conda-forge root numpy cython cmake ninja

