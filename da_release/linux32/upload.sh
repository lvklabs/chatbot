#!/bin/bash

. ../common/release-config

if [ -z "$1" ]; then
  echo "Usage:" `basename $0` "<package_file>"
else

  set -e

  echo "----------------------"
  echo "Uploading $1"
  echo "----------------------"

  cp ../../src/chatbot/CHANGELOG.txt CHANGELOG
  scp CHANGELOG $1 $lvk_user@$lvk_host:$pkg_host_path
  rm -f CHANGELOG
fi
