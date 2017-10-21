#!/bin/bash

set -e
ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# check if we are runnin in an container
IN_CONTAINER=false
if cat /proc/1/cgroup | grep docker > /dev/null;
then
  IN_CONTAINER=true
fi

# build process
if [ "$1" == "--direct" ] || [ "$1" == "-d" ] || [ "$IN_CONTAINER" == "true" ];
then
  cd "$ROOTDIR"
  ./builddeps.sh
  ./buildlibs.sh
else
  # build container
  cd "$ROOTDIR"
  docker build -f build.dockerfile -t snapsync-build-environment:latest .

  # run container
  if [ "$1" == "--shell" ] || [ "$1" == "-s" ];
  then
    docker run -it --rm -v "$ROOTDIR":/workspace -u $(id -u):$(id -g) --net=host snapsync-build-environment:latest /bin/bash
  else
    docker run -it --rm -v "$ROOTDIR":/workspace -u $(id -u):$(id -g) --net=host snapsync-build-environment:latest /workspace/build.sh
  fi
fi
