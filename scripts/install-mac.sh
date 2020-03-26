#! /bin/bash
brew install openssl libuv cmake zlib

mkdir tmp
wget -P tmp/ https://github.com/uNetworking/uWebSockets/archive/v0.13.0.tar.gz
tar -xvzf tmp/v0.13.0.tar.gz -C tmp/
cd tmp/uWebSockets-0.13.0
make
sudo make install
cd ../..
if [ ! -f "/usr/lib/libuWS.so" ]; then
    sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
fi
rm -r tmp

bash scripts/install-cppdeps.sh
