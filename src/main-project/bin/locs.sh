cat `find . ../test-suite -name "*.cpp" -or -name "*.h"` | grep -v "^\\s*$" | wc -l
