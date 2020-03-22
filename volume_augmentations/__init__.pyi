from typing import Optional, Tuple, List

import numpy as np


class VolumeTransformer:
    def __init__(self, volume_shape: Tuple[int, int, int], rotation: Optional[Tuple[float, float, float]] = None,
                 translation: Optional[Tuple[float, float, float]] = None,
                 scaling: Optional[Tuple[float, float, float]] = None,
                 channels_value_multipliers: Optional[List[float]] = None,
                 channels_value_addends: Optional[List[float]] = None, interpolation: str = 'nearest'): ...

    def apply(self, volume: np.ndarray) -> np.ndarray: ...


def augment(volume: np.ndarray, rotation: Optional[Tuple[float, float, float]] = None,
            translation: Optional[Tuple[float, float, float]] = None,
            scaling: Optional[Tuple[float, float, float]] = None,
            channels_value_multipliers: Optional[List[float]] = None,
            channels_value_addends: Optional[List[float]] = None, interpolation: str = 'nearest') -> np.ndarray: ...
