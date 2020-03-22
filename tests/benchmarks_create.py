from pytest_benchmark.fixture import BenchmarkFixture
from volume_augmentations import VolumeTransformer


def test_single_channel_empty_creation(benchmark: BenchmarkFixture):
    benchmark(VolumeTransformer, (1, 80, 80, 80))


def test_multi_channel_empty_creation(benchmark: BenchmarkFixture):
    benchmark(VolumeTransformer, (2, 80, 80, 80))


def test_single_channel_rotation_creation(benchmark: BenchmarkFixture):
    benchmark(VolumeTransformer, (1, 80, 80, 80), rotation=(15, 15, 15))


def test_multi_channel_rotation_creation(benchmark: BenchmarkFixture):
    benchmark(VolumeTransformer, (2, 80, 80, 80), rotation=(15, 15, 15))
