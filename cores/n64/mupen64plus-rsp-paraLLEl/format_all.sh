#!/bin/bash

for file in arch/*/*/*.{cpp,h} *.{cpp,hpp,h} rsp/*.{cpp,h}
do
    echo "Formatting file: $file ..."
    clang-format -style=file -i $file
done
