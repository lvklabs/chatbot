#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: " `basename $0` "<version>"
else
  echo "----------------"
  echo "Packaging v$1"
  echo "----------------"

  set -e
  
  pkg_dir=chatbot-linux32-$1
  mkdir -p $pkg_dir

  rm -f $pkg_dir/*

  cp -v runtime/* $pkg_dir
  cp -v .build/chatbot $pkg_dir
  cp -v ../../CHANGELOG.txt $pkg_dir/CHANGELOG

  tar -czvf sadosky-linux32-$1.tar.gz $pkg_dir
fi


