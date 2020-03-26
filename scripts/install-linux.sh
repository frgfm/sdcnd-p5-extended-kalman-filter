#!/bin/bash
source /etc/os-release || echo 'Warning: /etc/os-release was not found'

if [[ " $ID_LIKE " == *' archlinux '* ]]; then
  sudo pacman -S wget libuv openssl gcc cmake make
else
  if [[ ! " $ID_LIKE " == *' debian '* ]]; then
    echo 'Warning: unidentified Linux distribution, assuming Debian-like'
  fi

  sudo apt-get update
  sudo apt-get install wget libuv1-dev libssl-dev gcc g++ cmake make
fi

mkdir tmp
wget -P tmp/ https://github.com/uNetworking/uWebSockets/archive/v0.14.0.tar.gz
tar -xvzf tmp/v0.14.0.tar.gz -C tmp/
cd tmp/uWebSockets-0.14.0
sudo make install
cd ../..
sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
rm -r tmp

bash scripts/install-cppdeps.sh
