if [ -z "$1" ]; then
  echo "Usage:" `basename $0` "<package_file>"
else

  if [ -f passwd ]; then cat passwd; fi

  set -e

  cp ../../CHANGELOG.txt CHANGELOG
  scp CHANGELOG $1 lavandaink@lavandaink.com.ar:lavandaink.com.ar/builds/sadosky
  rm -f CHANGELOG
fi
