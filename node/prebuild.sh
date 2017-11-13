#!/bin/bash

set -e
ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."

# verify we have NODE_PRE_GYP_GITHUB_TOKEN
if [ -z "$NODE_PRE_GYP_GITHUB_TOKEN" ];
then
  echo "Please set NODE_PRE_GYP_GITHUB_TOKEN!"
  echo "See https://www.npmjs.com/package/node-pre-gyp-github for more."
  exit 1
fi

# check if we are running in an container
IN_CONTAINER=false
if cat /proc/1/cgroup | grep docker > /dev/null;
then
  IN_CONTAINER=true
fi

# prebuild process
if [ "$IN_CONTAINER" != "true" ];
then
  # clean project
  cd "$ROOTDIR"
  ./clean.sh

  for DOCKERFILE in ./node/targets/*.dockerfile;
  do
    echo "################################################################################"
    echo "Building with $DOCKERFILE"
    echo "################################################################################"

    # derive target name
    TARGET_NAME=`basename "$DOCKERFILE" .dockerfile`

    # build container
    docker build -f "$DOCKERFILE" -t "snapsync-node-build-target:$TARGET_NAME" .

    # run container
    docker run -it --rm -e NODE_PRE_GYP_GITHUB_TOKEN="$NODE_PRE_GYP_GITHUB_TOKEN" "snapsync-node-build-target:$TARGET_NAME" /workspace/node/prebuild.sh
  done
else
  # build dependencies, libraries and tools
  cd "$ROOTDIR"
  ./build.sh

  # publish prebuilt binaries
  cd "$ROOTDIR/node"
  npm run prebuild-publish
fi
