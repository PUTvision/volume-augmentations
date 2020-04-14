#include "volume_transformer.h"

#include <doctest/doctest.h>

TEST_CASE("Volume transformer2")
{
  RotationRanges<float> rotation_ranges{
      .x = {.min = -10, .max = 10},
      .y = {.min = -10, .max = 20},
      .z = {.min = -1, .max = 1},
  };
  TranslationRanges<float> translation_ranges{
      .x = {.min = -10, .max = 10},
      .y = {.min = -10, .max = 20},
      .z = {.min = -1, .max = 1},
  };

  auto transformations = Compose{
      SetInterpolation{InterpolationType::NearestNeighbour,
                       InterpolationType::Linear},
      RandomRotation{rotation_ranges},
      RandomTranslation{translation_ranges},
  };
  transformations.call();
}

