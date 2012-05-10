#!/bin/bash

`locate lupdate | grep -i qt | grep bin | head -n1 ` main-project.pro && \
`locate linguist | grep -i qt | grep bin | head -n1 ` strings_es.ts && \
`locate lrelease | grep -i qt | grep bin | head -n1 ` strings_es.ts && \
rm -f ../main-project-build-desktop-Desktop_Qt_4_7_4_for_GCC__Qt_SDK__*/qrc_resources.*


