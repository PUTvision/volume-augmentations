import numpy as np


def test_single_channel_rotation(va_impl):
    volume = np.ones((1, 48, 48, 48), dtype=np.float32)
    transformer = va_impl.VolumeTransformer(
        (1, 48, 48, 48), (15, 10, 5))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume == 0) == 13134


def test_multi_channel_rotation(va_impl):
    volume = np.ones((3, 48, 48, 48), dtype=np.float32)
    transformer = va_impl.VolumeTransformer(
        (3, 48, 48, 48), (15, 10, 5))
    rotated_volume = transformer.apply(volume)

    assert np.count_nonzero(rotated_volume[0] == 0) == 13134
    assert np.count_nonzero(rotated_volume[1] == 0) == 13134
    assert np.count_nonzero(rotated_volume[2] == 0) == 13134


def test_single_channel_margin(va_impl):
    max_rotation = 10
    volume = np.ones((1, 48, 48, 48), dtype=np.float32)

    for x_rotation in range(-max_rotation, max_rotation + 1, 5):
        for y_rotation in range(-max_rotation, max_rotation + 1, 5):
            for z_rotation in range(-max_rotation, max_rotation + 1, 5):
                for interpolation in ('nearest', 'linear'):
                    transformer = va_impl.VolumeTransformer((1, 48, 48, 48),
                                                            (x_rotation, y_rotation,
                                                             z_rotation),
                                                            interpolation=interpolation)
                    rotated_volume = transformer.apply(volume)

                    assert np.count_nonzero(
                        rotated_volume[:, 8:41, 8:41, 8:41] == 0) == 0


def test_multi_channel_margin(va_impl):
    max_rotation = 10
    volume = np.ones((3, 48, 48, 48), dtype=np.float32)

    for x_rotation in range(-max_rotation, max_rotation + 1, 5):
        for y_rotation in range(-max_rotation, max_rotation + 1, 5):
            for z_rotation in range(-max_rotation, max_rotation + 1, 5):
                for interpolation in ('nearest', 'linear'):
                    transformer = va_impl.VolumeTransformer((3, 48, 48, 48), (x_rotation, y_rotation, z_rotation),
                                                            interpolation=interpolation)
                    rotated_volume = transformer.apply(volume)

                    assert np.count_nonzero(
                        rotated_volume[0, 8:41, 8:41, 8:41] == 0) == 0
                    assert np.count_nonzero(
                        rotated_volume[1, 8:41, 8:41, 8:41] == 0) == 0
                    assert np.count_nonzero(
                        rotated_volume[2, 8:41, 8:41, 8:41] == 0) == 0
