#!/bin/env python3

from os     import listdir,remove
from random import shuffle

if __name__ == '__main__':
    target_dir = 'xfs_test'
    files = listdir(target_dir)
    shuffle(files)
    for file in files[::2]:
        path = target_dir + '/' + file
        remove(path)
        print('removed ' + path)

