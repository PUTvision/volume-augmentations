import importlib
import pytest


@pytest.fixture(scope="module",
                params=["volume_augmentations"])
def va_impl(request):
    return importlib.import_module(request.param)
