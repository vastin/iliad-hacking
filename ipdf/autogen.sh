#!/bin/sh
export PATH=$PATH:/usr/local/arm/oe/arm-linux/bin
export PKG_CONFIG_PATH=/usr/local/arm/oe/arm-linux/lib/pkgconfig

autoreconf -v --install || exit 1
./configure --host=arm-linux --build=i386 --prefix=/usr
