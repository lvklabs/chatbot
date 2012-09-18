#!/bin/bash

. ../common/release-config

if [ -z "$1" ]; then
  echo "$0 <file>"
  exit 1
fi


echo "----------------------"
echo "Uploading $1"
echo "----------------------"

scp.exe $1 $lvk_user@$lvk_host:$pkg_host_path

