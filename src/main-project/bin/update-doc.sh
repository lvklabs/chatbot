#!/bin/bash

if [ -d doc ]; then
  cd doc/
  doxygen Doxyfile
  cd ..
else
  echo "Error: Directory doc/ not found!"
  echo "       Run this script from src/main-project/ directory."
  exit 1
fi

