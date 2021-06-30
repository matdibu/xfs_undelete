#!/bin/env python3

import string
import random
from os import sync

if __name__ == '__main__':
    out_dir = 'xfs_test'
    for _ in range(100):
        file_name = ''.join(random.choice(string.ascii_lowercase) for i in range(16))
        file_text = 'this is the text for file ' + file_name
        with open(file_name, 'w') as f:
            f.write(file_text)
    sync()

