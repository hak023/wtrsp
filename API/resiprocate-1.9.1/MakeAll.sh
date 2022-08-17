#!/bin/bash

###
autoreconf --install
./configure.sh

###
cd rutil
make install

###
cd ../resip/stack
make install
