import random

import cv2
import numpy as np

import sys
sys.path.insert(1, 'build/lib')

from va_rs import VolumeTransformer

original_cube = np.zeros((32, 32, 32), dtype=np.float32)
original_cube[12:20, 12:20, 12:20] = 1.0
original_cube = original_cube[None, ...]

linear_cube = original_cube.copy()
nearest_cube = original_cube.copy()

rotations = (20, 4, 1)
translations = tuple(np.random.rand(3) * 6 - 3)  # <-3, +3>
scaling = tuple(np.random.rand(3) * 0.2 + 0.9)  # <0.9; 1.1>
raw_cube_multipliers = (random.random() * 0.2 + 0.9,)  # (<0.9; 1.1>, 1.0) - don't multiply frangi data

raw_cube_transformer = VolumeTransformer(linear_cube.shape, rotations,
                                         interpolation='linear')
label_cube_transformer = VolumeTransformer(nearest_cube.shape, rotations,
                                           interpolation='nearest')

linear_cube = raw_cube_transformer.apply(linear_cube)
nearest_cube = label_cube_transformer.apply(nearest_cube)

for i in range(32):
    cv2.imshow('original', original_cube[0, i, ...])
    cv2.imshow('linear', linear_cube[0, i, ...])
    cv2.imshow('nearest', nearest_cube[0, i, ...])
    cv2.waitKey()
