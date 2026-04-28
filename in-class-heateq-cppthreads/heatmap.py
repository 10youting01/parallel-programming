#!/bin/python

import argparse
import numpy as np
import matplotlib.pyplot as plt

def init_argparse() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        usage="%(prog)s [OPTION] [FILE]...",
        description="Print or check SHA1 (160-bit) checksums."
    )
    parser.add_argument('filename')
    parser.add_argument(
        "-v", "--version", action="version",
        version = f"{parser.prog} version 1.0"
    )
    parser.add_argument('animate')
    return parser

parser = init_argparse()
args = parser.parse_args()

meta = np.loadtxt(args.filename + ".meta", dtype='double')
print(meta)
grid_size = int(meta[0])
steps = int(meta[3])
rec_rate = int(meta[4])
print(grid_size)
print(steps)
print(rec_rate)

raw = np.fromfile(args.filename, dtype='double')
u = raw.reshape([steps // rec_rate, grid_size])

im = plt.imshow(np.transpose(u), aspect='auto', extent=(0., float(steps), 0., float(grid_size)))
plt.colorbar(im)
plt.ylabel('x')
plt.xlabel('step')
plt.show()
