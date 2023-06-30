#!/bin/bash

cd src
STRUCT_FILE="struct.txt"
INCLUDE_FILE="include.h"

find | grep '\.h' > $STRUCT_FILE

echo '' > $INCLUDE_FILE

for l in $(cat $STRUCT_FILE); do
    if [ $l != "./$INCLUDE_FILE" ] ; then
    echo "#include \"$(echo $l | cut -c 3- )\"" >> $INCLUDE_FILE
    fi
done

cd ../
less src/$INCLUDE_FILE
