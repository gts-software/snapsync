#!/bin/bash

BINARY="$1"
LIBRARY="$2"

while read LINE;
do
  if [[ "$LINE" =~ ^"$LIBRARY"\.so[\.0-9]*[[:space:]]+=\>[[:space:]]+(.*?)[[:space:]]+\(0x[0-9a-f]+\)$ ]];
  then
    FILE="${BASH_REMATCH[1]}"
    while [ -L "$FILE" ];
    do
      echo "$FILE"
      NEXT_FILE=$(readlink "$FILE")
      PARENT_DIR=$(dirname "$FILE")
      FILE="$PARENT_DIR/$NEXT_FILE"
    done
    echo "$FILE"
  fi
done < <(ldd "$BINARY")
