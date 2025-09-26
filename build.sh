#!/bin/zsh

make clean && make && 3dslink -a $IP_3DS target/Rangen_3ds.3dsx
