#!/bin/bash

main_project_pro="chatbot.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  echo "       Run this script from src/chatbot directory."
  exit 1
fi

rm -rf ../test-suite/.test-suite-shadow-build/
rm -rf builds/linux32/.build
rm -rf builds/win32/.build

find . -iname "*.h" -or -iname "*.cpp" -exec cccc {} \+

echo -n "Chatbot lines of code with comments       : "
cat `find . -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$" | wc -l
echo -n "Chatbot lines of code without comments    : "
cat `find . -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l
echo -n "Test suite lines of code with comments    : "
cat `find ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$" | wc -l
echo -n "Test suite lines of code without comments : "
cat `find ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l
echo -n "Total lines of code with comments         : "
cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$" | wc -l
echo -n "Total lines of code without comments      : "
cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$\|\\s*[{}]\\s*$\|^\\s*\(//\|/\\*\|\\*\|\\*/\).*$" | wc -l

