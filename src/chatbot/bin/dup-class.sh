#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
   echo "Syntax: " `basename $0` "<class_name> <new_class_name>"
   exit 1
fi

classname=$1
classname_upper=`echo $classname | tr [a-z] [A-Z]`
classname_lower=`echo $classname | tr [A-Z] [a-z]`
new_classname=$2
new_classname_upper=`echo $new_classname | tr [a-z] [A-Z]`
new_classname_lower=`echo $new_classname | tr [A-Z] [a-z]`

if ! [ -f "$classname_lower.h" ]; then
   echo "Error: $classname_lower.h not found!"
   exit 1
fi

if ! [ -f "$classname_lower.cpp" ]; then
   echo "Warning: $classname_lower.cpp not found!"
fi

cp $classname_lower.h $new_classname_lower.h
if [ -f "$classname_lower.cpp" ]; then cp $classname_lower.cpp $new_classname_lower.cpp; fi

sed -i -e "s/$classname/$new_classname/g" $new_classname_lower.*
sed -i -e "s/$classname_lower/$new_classname_lower/g" $new_classname_lower.*
sed -i -e "s/$classname_upper/$new_classname_upper/g" $new_classname_lower.*

