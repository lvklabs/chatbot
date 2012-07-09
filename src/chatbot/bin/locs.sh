#!/bin/bash

main_project_pro="chatbot.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  echo "       Run this script from src/chatbot directory."
  exit 1
fi

cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$" | wc -l
