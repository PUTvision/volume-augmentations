#[macro_use(array)]
extern crate ndarray;
extern crate blas_src;
extern crate ndarray_linalg;
#[cfg(target_os = "linux")]
extern crate openblas_src;
#[cfg(any(target_os = "macos", target_os = "windows"))]
extern crate intel_mkl_src;

mod interpolation;
mod transformations;
mod utils;

use pyo3::prelude::*;
use pyo3::wrap_pyfunction;

use numpy::{IntoPyArray, PyArray4};

use crate::interpolation::{Linear, NearestNeighbour};
use crate::transformations::*;
use ndarray::{Array2, Array4, ArrayView4};

#[pyclass]
struct VolumeTransformer {
    transformation: Array2<f32>,
    channels_value_multipliers: Vec<f32>,
    channels_value_addends: Vec<f32>,
    transforming_function: fn(
        volume: &ArrayView4<f32>,
        volume_shape: (usize, usize, usize, usize),
        transformation: &Array2<f32>,
        channels_value_multipliers: &Vec<f32>,
        channels_value_addends: &Vec<f32>,
    ) -> Array4<f32>,
}

const DEFAULT_INTERPOLATION: &str = "nearest";

#[cfg(not(test))]
#[pymethods]
impl VolumeTransformer {
    #[new]
    #[args(interpolation = "DEFAULT_INTERPOLATION")]
    fn new(
        volume_shape: (usize, usize, usize, usize),
        rotation: Option<(f32, f32, f32)>,
        translation: Option<(f32, f32, f32)>,
        scaling: Option<(f32, f32, f32)>,
        channels_value_multipliers: Option<Vec<f32>>,
        channels_value_addends: Option<Vec<f32>>,
        interpolation: &str,
    ) -> Self {
        let channels_value_multipliers =
            channels_value_multipliers.unwrap_or(vec![1.0f32; volume_shape.0]);
        assert_eq!(channels_value_multipliers.len(), volume_shape.0);
        let channels_value_addends = channels_value_addends.unwrap_or(vec![0.0f32; volume_shape.0]);
        assert_eq!(channels_value_addends.len(), volume_shape.0);

        let transformation =
            get_transformation_matrix(&volume_shape, rotation, translation, scaling);
        let transforming_function = match interpolation {
            "nearest" => apply_transformations::<NearestNeighbour>,
            "linear" => apply_transformations::<Linear>,
            _ => panic!("Unsupported interpolation: {}", interpolation),
        };

        VolumeTransformer {
            transformation,
            channels_value_multipliers,
            channels_value_addends,
            transforming_function,
        }
    }

    fn apply(&self, py: Python, volume: &PyArray4<f32>) -> Py<PyArray4<f32>> {
        let volume = volume.as_array();
        let volume_shape = (
            volume.shape()[0],
            volume.shape()[1],
            volume.shape()[2],
            volume.shape()[3],
        );
        let output_volume = (self.transforming_function)(
            &volume,
            volume_shape,
            &self.transformation,
            &self.channels_value_multipliers,
            &self.channels_value_addends,
        );
        output_volume.into_pyarray(py).to_owned()
    }
}

#[pyfunction]
pub fn augment(
    py: Python,
    volume: &PyArray4<f32>,
    rotation: Option<(f32, f32, f32)>,
    translation: Option<(f32, f32, f32)>,
    scaling: Option<(f32, f32, f32)>,
    channels_value_multipliers: Option<Vec<f32>>,
    channels_value_addends: Option<Vec<f32>>,
    interpolation: &str,
) -> Py<PyArray4<f32>> {
    let volume = volume.as_array();
    let volume_shape = (
        volume.shape()[0],
        volume.shape()[1],
        volume.shape()[2],
        volume.shape()[3],
    );
    let transformation = get_transformation_matrix(&volume_shape, rotation, translation, scaling);

    let channels_value_multipliers =
        channels_value_multipliers.unwrap_or(vec![1.0f32; volume_shape.0]);
    let channels_value_addends = channels_value_addends.unwrap_or(vec![0.0f32; volume_shape.0]);
    assert_eq!(channels_value_multipliers.len(), volume_shape.0);
    assert_eq!(channels_value_addends.len(), volume_shape.0);

    let output_volume = match interpolation {
        "nearest" => apply_transformations::<NearestNeighbour>(
            &volume,
            volume_shape,
            &transformation,
            &channels_value_multipliers,
            &channels_value_addends,
        ),
        "linear" => apply_transformations::<Linear>(
            &volume,
            volume_shape,
            &transformation,
            &channels_value_multipliers,
            &channels_value_addends,
        ),
        _ => panic!("Unsupported interpolation: {}", interpolation),
    };
    output_volume.into_pyarray(py).to_owned()
}

#[pymodule]
fn volume_augmentations(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add_wrapped(wrap_pyfunction!(augment))?;
    m.add_class::<VolumeTransformer>()?;

    Ok(())
}
