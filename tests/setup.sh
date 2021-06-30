#!/bin/sh

lvcreate -y -L 40G vg0 -n xfs_test
mkfs.xfs /dev/vg0/xfs_test -f
mount /dev/vg0/xfs_test xfs_test -o user

