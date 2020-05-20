################################################################################
FROM node:6-stretch as build

# install required build tools and libraries

RUN  apt-get update \
  && apt-get install -y --no-install-recommends \
        cmake \
        libboost-all-dev \
        diffstat \
  && rm -rf /var/lib/apt/lists/*

# copy workspace
COPY . /workspace

# run bash per default
CMD [ "/bin/bash" ]
