#!/bin/bash

main_project_pro="chatbot.pro"

if ! [ -f $main_project_pro ]; then
  echo "Error: File $main_project_pro not found!"
  echo "       Run this script from src/chatbot directory."
  exit 1
fi

rm -f ../chatbot-build-desktop-Desktop_Qt_4_7_4_for_GCC__Qt_SDK__*/qrc_.*


