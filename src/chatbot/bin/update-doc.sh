#!/bin/bash

base_dir=`dirname $0`
doc_dir=$base_dir/../doc

if [ -d $doc_dir ]; then
  cd $doc_dir
  doxygen Doxyfile
  cd -
else
  echo "Error: Directory doc/ not found!"
  exit 1
fi

