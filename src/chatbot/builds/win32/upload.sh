#!/bin/bash

. ../common/release-config

if [ -z "$1" ]; then
  echo "$0 <file>"
  exit 1
fi


echo "----------------------"
echo "Uploading $1"
echo "----------------------"

if [ -f passwd ]; then cat.exe passwd | tail -n1; fi
scp.exe $1 $lvk_user@$lvk_host:$pkg_host_path

