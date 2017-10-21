################################################################################
FROM node:8-alpine as build

# install required build tools and libraries
RUN apk add --no-cache bash make cmake gcc g++ boost boost-dev perl

# prepare workspace
RUN mkdir -p /workspace
WORKDIR /workspace

# run bash per default
CMD [ "/bin/bash" ]
