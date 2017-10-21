#!/bin/bash

set -e
ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# initiate build container
cd "$ROOTDIR"
docker build -f build.dockerfile .
