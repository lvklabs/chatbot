#!/bin/bash

shadow_build_dir=.build
project_file=../../../src/chatbot/chatbot.pro

if [ -z "$1" ]; then 
  rm -rf $shadow_build_dir
elif [ "$1" != "--fast" ]; then
  echo "Usage: " `basename $0` "[ --fast ]"
  exit 1
fi

echo "--------"
echo "Building"
echo "--------"

if [ -z "$QTDIR" ]; then
  qmake_path=qmake
else
  qmake_path=$QTDIR/bin/qmake
fi

mkdir -p $shadow_build_dir
( cd $shadow_build_dir && $qmake_path $project_file "CONFIG+=release da_contest" && make )


