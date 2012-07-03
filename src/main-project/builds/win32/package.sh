#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: " `basename $0` "<version>"
  exit 1
else
  echo "----------------"
  echo "Packaging v$1"
  echo "----------------"

  set -e
  
  pkg_dir=chatbot-win32-$1
  mkdir -p $pkg_dir

  rm -f $pkg_dir/*

  cp -v runtime/* $pkg_dir
  cp -v .build/release/chatbot.exe $pkg_dir
  cp -v ../../CHANGELOG.txt $pkg_dir/CHANGELOG.txt
  cp -vR ../../lang $pkg_dir

  rm -f sadosky-win32-$1.zip
  bin/7z.exe a sadosky-win32-$1.zip $pkg_dir/
fi
