################################################################################
FROM node:8-alpine as build

# install required build tools and libraries
RUN apk add --no-cache bash make cmake gcc g++ boost boost-dev perl

# prepare working directory
RUN mkdir -p /build
WORKDIR /build

# copy and build dependencies
COPY ./librsync/      /build/librsync/
COPY ./cryptopp/      /build/cryptopp/
COPY ./builddeps.sh   /build/builddeps.sh

RUN /build/builddeps.sh

# copy and build libraries
COPY ./src/           /build/src/
COPY ./node/          /build/node/
COPY ./buildlibs.sh   /build/buildlibs.sh
COPY ./Makefile       /build/Makefile

RUN /build/buildlibs.sh
