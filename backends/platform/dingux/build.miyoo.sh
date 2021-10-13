#!/bin/bash

export PATH=/opt/miyoo/usr/bin/:$PATH


 ../../../configure --host=miyoo --with-mad-prefix=/opt/miyoo/usr/bin --enable-plugins --default-dynamic --disable-all-unstable-engines --disable-flac --disable-debug --disable-highres --enable-release --disable-hq-scalers --disable-mt32emu --disable-lua --disable-nuked-opl && make && make miyoo-dist
