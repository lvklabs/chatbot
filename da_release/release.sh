#!/bin/bash

function show_usage
{
  echo "Usage:" `basename $0` "[ --force ]"
}

if uname | grep -qi linux; then
  target_os=linux32
  pkg_format=tar.gz
elif uname | grep -qi cygwin; then
  target_os=win32
  pkg_format=zip
else
  echo "ERROR: Unsupported OS type: " `uname` 
  exit 1
fi

if [ -z "$target_os" ] || ! [ -d "$target_os" ]; then
  show_usage $*
  exit 1
fi

if [ -f export_crypto_keys ]; then
  . export_crypto_keys
else
  echo "Error: Cryptographic keys not found."
  read -p "Press Enter to continue or Ctrl-C to abort"
fi

. common/release-config

if [ -z "$pkg_version" ]; then
  echo "Error: Package version not set"
  exit 1
fi

if [ -z "$pkg_name" ]; then
  echo "Error: Package name not set"
  exit 1
fi

echo "*************************"
echo "Releasing chatbot v$pkg_version"
echo "*************************"

pkg_filename=$pkg_name.$pkg_format

echo "Verifying if $pkg_filename already exists at $lvk_host/$pkg_host_path..."
if ssh $lvk_user@$lvk_host "test -f $pkg_host_path/$pkg_filename"; then
  if [ "$1" != "--force" ]; then
    echo "Error: $pkg_filename already exists. To overwrite use --force"
    exit 1
  else
    echo "$pkg_filename already exists but overwriting"
  fi
fi

pushd $target_os

./build.sh && \
./package.sh $pkg_version && \
./upload.sh $pkg_filename && \
git tag -f $pkg_version && \
echo "*** Everything went OK! Package uploaded and repository tagged ***"

popd
