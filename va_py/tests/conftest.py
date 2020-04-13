import importlib
import pytest


@pytest.fixture(scope="module",
                params=["volume_augmentations.va_cpp"])
def va_impl(request):
    return importlib.import_module(request.param)
