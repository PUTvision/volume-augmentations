
import sys
sys.path.insert(1, 'build/lib')

import vacpp

# random_rotation = volume_augmentations.RandomRotation(((-20, 20), (-20, 20), (-20, 20)))
augmentations = vacpp.Compose([
    vacpp.RandomRotation(((-20, 20), (-20, 20), (-20, 20)))
    for _ in range(100)
])

augmentations.print()
