
import sys
sys.path.insert(1, 'build/lib')

import va_cpp

# random_rotation = volume_augmentations.RandomRotation(((-20, 20), (-20, 20), (-20, 20)))
augmentations = va_cpp.Compose([
    va_cpp.RandomRotation(((-20, 20), (-20, 20), (-20, 20)))
    for _ in range(100)
])

augmentations.print()
