#!/bin/bash

common_src_dir=`dirname $0`/../common
versionrev_h_path=$common_src_dir/versionrev.h

macro_name="AUTO_APP_VERSION_REV"
input_pattern="#define\\s\\+$macro_name\\s\\+\"\\([a-f0-9]\\+\\)\""

# get git HEAD revision
git_head_rev=`git log -n1 --pretty=%H`

# get versionrev.h revision
version_h_rev=`cat $versionrev_h_path | grep $macro_name | sed -e "s/$input_pattern/\\1/"`

echo "git HEAD revision    : $git_head_rev"
echo "versionrev.h revision: $version_h_rev"

if [ "$git_head_rev" != "$version_h_rev" ]; then
   echo "New version found! Updating revision in versionrev.h..."
   output_pattern="#define $macro_name     \"$git_head_rev\""
   sed -i -e "s/$input_pattern/$output_pattern/" $versionrev_h_path
   echo "Done!"
fi

