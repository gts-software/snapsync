################################################################################
FROM node:12-alpine as build

# install required build tools and libraries
RUN echo "@testing http://dl-3.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories
RUN apk add --no-cache bash make cmake gcc g++ boost boost-dev perl diffstat@testing

# copy workspace
COPY . /workspace

# run bash per default
CMD [ "/bin/bash" ]
