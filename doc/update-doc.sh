#!/bin/bash

base_dir=`dirname $0`
doc_dir=$base_dir/

if [ -f $doc_dir/Doxyfile ]; then
  cd $doc_dir
  doxygen Doxyfile
  cd -
else
  echo "Error: Doxyfile not found!"
  exit 1
fi

