#!/bin/sh

umount xfs_test

LD_PRELOAD=/usr/lib/gcc/x86_64-pc-linux-gnu/11.1.0/libasan.so \
	./xfs_undelete \
	-d /dev/vg0/xfs_test \
	-o recovered/

