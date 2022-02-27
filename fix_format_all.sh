#!/bin/bash

THIS_PATH="$(realpath "$0")"
THIS_DIR="$(dirname "$THIS_PATH")"

for dir in " "
do
    FILE_LIST="$(find "$THIS_DIR/$dir"| grep -E ".*(\.cpp|\.c|\.h|\.hpp)$")"
    clang-format -i $FILE_LIST
    for file in $FILE_LIST
    do
        [ -n "$(tail -c1 $file)" ] && echo >> $file
    done
done
