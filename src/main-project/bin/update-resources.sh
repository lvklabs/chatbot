#!/bin/bash

main_project_pro="main-project.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  echo "       Run this script from src/main-project directory."
  exit 1
fi

rm -f ../main-project-build-desktop-Desktop_Qt_4_7_4_for_GCC__Qt_SDK__*/qrc_resources.*


