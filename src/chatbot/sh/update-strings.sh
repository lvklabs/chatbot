#!/bin/bash

base_dir=`dirname $0`
project_dir=$base_dir/..

main_project_pro="$project_dir/chatbot.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  exit 1
fi

lupdate_bin=`type -p lupdate`
linguist_bin=`type -p linguist`
lrelease_bin=`type -p lrelease`

if [ -z "$lupdate_bin" ]; then
  lupdate_bin=`locate lupdate | grep -i qt | grep bin | head -n1`
  if [ -z "$lupdate_bin" ]; then
    echo "Error: Command 'lupdate' not found"
    exit 1
  fi
fi

if [ -z "$linguist_bin" ]; then
  linguist_bin=`locate linguist | grep -i qt | grep bin | head -n1`
  if [ -z "$linguist_bin" ]; then
    echo "Error: Command 'linguist' not found"
    exit 1
  fi
fi

if [ -z "$lrelease_bin" ]; then
  lrelease_bin=`locate lrelease | grep -i qt | grep bin | head -n1`
  if [ -z "$lrelease_bin" ]; then
    echo "Error: Command 'lrelease' not found"
    exit 1
  fi
fi

set -e

`echo $lupdate_bin` $main_project_pro
`echo $linguist_bin` $project_dir/lang/chatbot2_es_AR.ts
`echo $lrelease_bin` $project_dir/lang/chatbot2_es_AR.ts
rm -f $project_dir/../chatbot-build-desktop-*/qrc_resources.*


