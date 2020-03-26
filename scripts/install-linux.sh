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
wget -P tmp/ https://github.com/uNetworking/uWebSockets/archive/v0.13.0.tar.gz
tar -xvzf tmp/v0.13.0.tar.gz -C tmp/
cd tmp/uWebSockets-0.13.0
mkdir build && cd build
cmake .. && make
sudo make install
cd ../..
if [ ! -f "/usr/lib/libuWS.so" ]; then
    sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
fi
rm -r tmp

bash scripts/install-cppdeps.sh
