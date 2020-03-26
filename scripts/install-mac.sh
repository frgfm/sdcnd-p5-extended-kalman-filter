#! /bin/bash
brew install openssl libuv cmake zlib

mkdir tmp
wget -P tmp/ https://github.com/uNetworking/uWebSockets/archive/v0.14.0.tar.gz
tar -xvzf tmp/v0.14.0.tar.gz -C tmp/
cd tmp/uWebSockets-0.14.0
make
sudo make install
cd ../..
sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
rm -r tmp

bash scripts/install-cppdeps.sh
