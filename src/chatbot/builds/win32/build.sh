#!/bin/bash

shadow_build_dir=.build
project_file=../../../chatbot.pro

if [ -z "$1" ]; then 
  rm -rf $shadow_build_dir
elif [ "$1" != "--fast" ]; then
  echo "Usage: " `basename $0` "[ --fast ]"
  exit 1
fi

echo "--------"
echo "Building"
echo "--------"

mkdir -p $shadow_build_dir
( cd $shadow_build_dir && qmake $project_file "CONFIG+=release" && make )
