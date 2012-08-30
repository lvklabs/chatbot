#!/bin/bash

main_project_pro="chatbot.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  echo "       Run this script from src/chatbot directory."
  exit 1
fi

rm -rf ../test-suite/.test-suite-shadow-build/

echo -n "Chatbot lines of code with comments       : "
cat `find . -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$" | wc -l
echo -n "Chatbot lines of code without comments    : "
cat `find . -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l
echo -n "Test suite lines of code with comments    : "
cat `find ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$" | wc -l
echo -n "Test suite lines of code without comments : "
cat `find ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l
echo -n "Total lines of code with comments         : "
cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$" | wc -l
echo -n "Total lines of code without comments      : "
cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l
