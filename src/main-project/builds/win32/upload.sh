if [ -z "$1" ]; then
  echo "$0 <file>"
  exit 1
fi

if [ -f passwd ]; then cat.exe passwd | tail -n1; fi
scp.exe $1 lavandaink@lavandaink.com.ar:lavandaink.com.ar/builds/sadosky
