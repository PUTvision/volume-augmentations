import numpy as np

from volume_augmentations import VolumeTransformer


def test_single_channel_multiplication():
    volume = np.ones((1, 48, 48, 48), dtype=np.float32)
    transformer = VolumeTransformer((1, 48, 48, 48), channels_value_multipliers=(1.1,))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume != 1.1) == 0


def test_multi_channel_multiplication():
    volume = np.ones((3, 48, 48, 48), dtype=np.float32)
    transformer = VolumeTransformer((3, 48, 48, 48), channels_value_multipliers=(1.1, 1.2, 0.9))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume[0] != 1.1) == 0
    assert np.count_nonzero(rotated_volume[1] != 1.2) == 0
    assert np.count_nonzero(rotated_volume[2] != 0.9) == 0


def test_single_channel_addition():
    volume = np.ones((1, 48, 48, 48), dtype=np.float32)
    transformer = VolumeTransformer((1, 48, 48, 48), channels_value_addends=(0.1,))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume != 1.1) == 0


def test_multi_channel_addition():
    volume = np.ones((3, 48, 48, 48), dtype=np.float32)
    transformer = VolumeTransformer((3, 48, 48, 48), channels_value_addends=(0.1, 0.2, -0.1))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume[0] != 1.1) == 0
    assert np.count_nonzero(rotated_volume[1] != 1.2) == 0
    assert np.count_nonzero(rotated_volume[2] != 0.9) == 0
