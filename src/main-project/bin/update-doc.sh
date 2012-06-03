#!/bin/bash

if [ -d doc ]; then
  cd doc/
  doxygen Doxyfile
  cd ..
else
  echo "Error: Directory doc/ not found!"
fi

