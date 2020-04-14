from volume_augmentations.va_cpp import RandomRotation
from volume_augmentations.va_cpp import RandomTranslation
from volume_augmentations.va_cpp import InterpolationType
from volume_augmentations.va_cpp import SetInterpolation
from volume_augmentations.va_cpp import Compose
from volume_augmentations.va_cpp import Range
from volume_augmentations.va_cpp import RotationRanges
from volume_augmentations.va_cpp import TranslationRanges


def main():
    #  rotation_ranges = (x: (min: -10, max: 10), y: (min: -10, max: 10), y: (min: -10,
    #  max: 10))
    rotation_ranges = RotationRanges(
        Range(-10, 10), Range(-10, 10), Range(-10, 10))
    translation_ranges = TranslationRanges(
        Range(-10, 10), Range(-10, 10), Range(-10, 10))

    transformations = Compose([
        SetInterpolation([InterpolationType.NearestNeighbour,
                          InterpolationType.Linear]),
        RandomRotation([rotation_ranges]),
        RandomTranslation([translation_ranges])
    ])
    transformations()


if __name__ == '__main__':
    main()
