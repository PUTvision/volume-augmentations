import importlib
import pytest


@pytest.fixture(scope="module",
                params=["volume_augmentations.va_cpp",
                        "volume_augmentations.va_rs"])
def va_impl(request):
    return importlib.import_module(request.param)
