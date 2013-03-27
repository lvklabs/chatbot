#!/bin/bash
#
# TODO use common/release-config

if [ -z "$1" ]; then
  echo "Usage: " `basename $0` "<version>"
  exit 1
else
  echo "----------------"
  echo "Packaging v$1"
  echo "----------------"

  set -e
  
  pkg_dir=chatbot-win32-$1

  rm -rf $pkg_dir

  mkdir -p $pkg_dir
  mkdir -p $pkg_dir/lang  

  lupdate.exe ../../src/chatbot/chatbot.pro 

  cp -vR runtime/* $pkg_dir
  cp -v .build/release/chatbot.exe $pkg_dir
#  cp -v ../../src/chatbot/CHANGELOG $pkg_dir/CHANGELOG.txt
  cp -v ../../src/chatbot/lang/*.qm $pkg_dir/lang
  cp -v ../../src/chatbot/lang/chatbot2_xx_XX.ts $pkg_dir/lang

  rm -f chatbot-win32-$1.zip
  bin/7z.exe a chatbot-win32-$1.zip $pkg_dir/
fi

