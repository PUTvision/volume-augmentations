import numpy as np
from pytest_benchmark.fixture import BenchmarkFixture
from volume_augmentations import VolumeTransformer


def test_single_channel_empty_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((1, 80, 80, 80))
    volume = np.random.rand(1, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_multi_channel_empty_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((2, 80, 80, 80))
    volume = np.random.rand(2, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_single_channel_empty_linear_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((1, 80, 80, 80), interpolation='linear')
    volume = np.random.rand(1, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_multi_channel_empty_linear_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((2, 80, 80, 80), interpolation='linear')
    volume = np.random.rand(2, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_single_channel_rotation_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((1, 80, 80, 80), rotation=(15, 15, 15))
    volume = np.random.rand(1, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_multi_channel_rotation_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((2, 80, 80, 80), rotation=(15, 15, 15))
    volume = np.random.rand(2, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_single_channel_rotation_linear_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((1, 80, 80, 80), rotation=(15, 15, 15), interpolation='linear')
    volume = np.random.rand(1, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)


def test_multi_channel_rotation_linear_apply(benchmark: BenchmarkFixture):
    volume_transformer = VolumeTransformer((2, 80, 80, 80), rotation=(15, 15, 15), interpolation='linear')
    volume = np.random.rand(2, 80, 80, 80).astype(np.float32)
    benchmark(volume_transformer.apply, volume)
