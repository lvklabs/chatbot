#/bin/bash

cat `dirname $0`/../../src/chatbot/common/version.h | grep APP_VERSION_STR | sed -e 's/.*"\(.*\)"/\1/'
