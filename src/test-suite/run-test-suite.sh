#!/bin/bash

shadow_build_dir=.test-suite-shadow-build
project_dir=..
log_file=run.log

unit_tests="json-unit-test cipher-unit-test csv-document-unit-test conversation-rw-unit-test default-sanitizer-unit-test secure-stats-file-unit-test updater-unit-test cb2-engine-unit-test"
sys_tests="user-auth-test cb2-engine-full-test stats-manager-test"

show_usage()
{
  echo "Syntax:"
  echo "   $0 -u        # Run unit tests"
  echo "   $0 -s        # Run system tests"
  echo "   $0 -a        # Run all tests"
  echo "   $0 test_name # Run only specified tests"
}

if [ "$1" == "-u" ]; then
  tests="$unit_tests"
elif [ "$1" == "-s" ]; then
  tests="$sys_tests"
elif [ "$1" == "-a" ]; then
  tests="$unit_tests $sys_tests"
elif [ "$1" == "" ]; then
  show_usage
  exit 1
else
  for d in $@ ; do
    if [ -d "$d" ]; then
      tests="$tests $d"
    else
      echo "Error: Directory $d not found"
      echo
      show_usage
      exit 1
    fi
  done
fi

set -e

rm -f $log_file
mkdir -p $shadow_build_dir
cd $shadow_build_dir
qmake $project_dir "SUBDIRS=$tests"
make

for t in $tests; do
  ( cd $t && find . -name "*Test" -executable -exec ./{} \; | tee -a ../$log_file )
done


echo
echo
echo "==== TEST SUITE EXECUTION SUMMARY ===="
echo
cat $log_file | grep "********* Start\|Totals" | sed -e 's/ Start testing of/-/g' | tr -d "*"
rm -f $log_file
echo 
echo "======================================"
echo

cd $project_dir


