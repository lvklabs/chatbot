#!/bin/bash

shadow_build_dir=.test-suite-shadow-build
project_dir=..
log_file=run.log

unit_tests="json-unit-test cipher-unit-test csv-document-unit-test conversation-rw-unit-test default-sanitizer-unit-test aiml-engine-unit-test hybrid-engine-unit-test secure-stats-file-unit-test stats-manager-test"
e2e_tests="end-to-end-test"

if [ "$1" == "-u" ]; then
  tests="$unit_tests"
elif [ "$1" == "-e" ]; then
  tests="$e2e_tests"
elif [ "$1" == "-a" ]; then
  tests="$unit_tests $e2e_tests"
elif [ -d "$1" ]; then
  tests="$1"
else
  echo "Syntax:"
  echo "   $0 -u        # Run unit and system tests"
  echo "   $0 -e        # Run end to end tests"
  echo "   $0 -a        # Run all tests"
  echo "   $0 test_name # Run only specified test"
  exit 1
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


