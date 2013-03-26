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
  
  pkg_dir=chatbot-linux32-$1

  rm -rf $pkg_dir

  mkdir -p $pkg_dir
  mkdir -p $pkg_dir/lang  

  cp -vR runtime/* $pkg_dir
  cp -v .build/chatbot $pkg_dir
#  cp -v ../../src/chatbot/CHANGELOG $pkg_dir/CHANGELOG
  cp -v ../../src/chatbot/lang/chatbot2_es_AR.qm $pkg_dir/lang

  strip $pkg_dir/chatbot

  tar -czvf chatbot-linux32-$1.tar.gz $pkg_dir
fi

