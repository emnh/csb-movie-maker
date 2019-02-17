#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# vim: ft=python ts=4 sw=4 sts=4 et fenc=utf-8
# Original author: "Eivind Magnus Hvidevold" <hvidevold@gmail.com>
# License: GNU GPLv3 at http://www.gnu.org/licenses/gpl.html

'''
'''

import os
import sys
import re

def rl():
    return sys.stdin.readline()

def irl():
    return int(rl())

def main():
    'entry point'

    # Player 1
    laps = irl()
    print(laps)
    cps = irl()
    print(cps)
    for i in range(cps):
        print(rl(),end="")

    # Player 2
    laps = irl()
    cps = irl()
    for i in range(cps):
        rl()

    podCount = 4
    i = 0
    for line in sys.stdin:
        if int(i / 4) % 2 == 0:
            # Player 1
            print(line,end="")
        else:
            # Player 2
            pass
        i += 1

if __name__ == '__main__':
    main()

