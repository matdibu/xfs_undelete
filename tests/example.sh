#!/bin/sh

./teardown.sh
./setup.sh
# ./create_files.py
./mount.sh
./copy_thesis.sh
./umount.sh
./mount.sh
./delete_random.py
./umount.sh
./run.sh
./fix_owner.sh
