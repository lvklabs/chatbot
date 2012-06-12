#!/bin/bash

shadow_build_dir=.build
project_file=../../../main-project.pro

rm -rf $shadow_build_dir
mkdir -p $shadow_build_dir
( cd $shadow_build_dir && qmake $project_file "CONFIG+=release" && make )



