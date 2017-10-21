################################################################################
FROM node:8-alpine as build

# install required build tools and libraries
RUN echo "@testing http://dl-3.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories
RUN apk add --no-cache bash make cmake gcc g++ boost boost-dev perl diffstat@testing

# prepare workspace
RUN mkdir -p /workspace
WORKDIR /workspace

# run bash per default
CMD [ "/bin/bash" ]
