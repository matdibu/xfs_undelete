#!/bin/sh

umount xfs_test
lvremove /dev/vg0/xfs_test -y
