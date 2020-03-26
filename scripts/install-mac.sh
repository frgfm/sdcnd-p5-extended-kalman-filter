#! /bin/bash
brew install openssl libuv cmake zlib

mkdir tmp
wget -P tmp/ https://github.com/uNetworking/uWebSockets/archive/v0.13.0.tar.gz
tar -xvzf tmp/v0.13.0.tar.gz -C tmp/
cd tmp/uWebSockets-0.13.0

mkdir build && cd build
export PKG_CONFIG_PATH=/usr/local/opt/openssl/lib/pkgconfig 
OPENSSL_VERSION=`brew list --versions openssl | cut -d' ' -f2`
cmake -DOPENSSL_ROOT_DIR=$(brew --cellar openssl)/$OPENSSL_VERSION -DOPENSSL_LIBRARIES=$(brew --cellar openssl)/$OPENSSL_VERSION/lib ..
make
sudo make install
cd ../..
if [ ! -f "/usr/lib/libuWS.so" ]; then
    sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
fi
rm -r tmp

bash scripts/install-cppdeps.sh
