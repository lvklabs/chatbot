#!/bin/bash

. ../common/release-config

if [ -z "$1" ]; then
  echo "Usage:" `basename $0` "<package_file>"
else

  set -e

  echo "----------------------"
  echo "Uploading $1"
  echo "----------------------"

  scp ../../src/chatbot/CHANGELOG $1 $lvk_user@$lvk_host:$pkg_host_path
fi
