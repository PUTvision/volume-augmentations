use crate::interpolation::Interpolation;
use crate::utils::angles_to_radians;
use itertools::multizip;
use ndarray::{Array2, Array4, ArrayView4, Axis};
use ndarray_linalg::solve::Inverse;

pub fn get_transformation_matrix(
    volume_shape: &(usize, usize, usize, usize),
    rotation: Option<(f32, f32, f32)>,
    translation: Option<(f32, f32, f32)>,
    scaling: Option<(f32, f32, f32)>,
) -> Array2<f32> {
    let (_, x_size, y_size, z_size) = *volume_shape;

    let mut transformation: Array2<f32> = array![
        [1.0f32, 0.0f32, 0.0f32, x_size as f32 / 2.0f32],
        [0.0f32, 1.0f32, 0.0f32, y_size as f32 / 2.0f32],
        [0.0f32, 0.0f32, 1.0f32, z_size as f32 / 2.0f32],
        [0.0f32, 0.0f32, 0.0f32, 1.0f32]
    ];

    if let Some(scales) = scaling {
        let (x_scale, y_scale, z_scale) = scales;
        let scaling_matrix: Array2<f32> = array![
            [x_scale, 0.0f32, 0.0f32, 0.0f32],
            [0.0f32, y_scale, 0.0f32, 0.0f32],
            [0.0f32, 0.0f32, z_scale, 0.0f32],
            [0.0f32, 0.0f32, 0.0f32, 1.0f32]
        ];

        transformation = transformation.dot(&scaling_matrix);
    }

    if let Some(angles) = rotation {
        let (x_angle, y_angle, z_angle) = angles_to_radians(&angles);
        let (sin_x, sin_y, sin_z) = (f32::sin(x_angle), f32::sin(y_angle), f32::sin(z_angle));
        let (cos_x, cos_y, cos_z) = (f32::cos(x_angle), f32::cos(y_angle), f32::cos(z_angle));
        let x_rotation: Array2<f32> = array![
            [1.0f32, 0.0f32, 0.0f32, 0.0f32],
            [0.0f32, cos_x, -sin_x, 0.0f32],
            [0.0f32, sin_x, cos_x, 0.0f32],
            [0.0f32, 0.0f32, 0.0f32, 1.0f32]
        ];
        let y_rotation: Array2<f32> = array![
            [cos_y, 0.0f32, sin_y, 0.0f32],
            [0.0f32, 1.0f32, 0.0f32, 0.0f32],
            [-sin_y, 0.0f32, cos_y, 0.0f32],
            [0.0f32, 0.0f32, 0.0f32, 1.0f32]
        ];
        let z_rotation: Array2<f32> = array![
            [cos_z, -sin_z, 0.0f32, 0.0f32],
            [sin_z, cos_z, 0.0f32, 0.0f32],
            [0.0f32, 0.0f32, 1.0f32, 0.0f32],
            [0.0f32, 0.0f32, 0.0f32, 1.0f32]
        ];

        transformation = transformation
            .dot(&x_rotation)
            .dot(&y_rotation)
            .dot(&z_rotation);
    }

    if let Some(translations) = translation {
        let (x_translation, y_translation, z_translation) = translations;
        let translation_matrix: Array2<f32> = array![
            [1.0f32, 0.0f32, 0.0f32, x_translation],
            [0.0f32, 1.0f32, 0.0f32, y_translation],
            [0.0f32, 0.0f32, 1.0f32, z_translation],
            [0.0f32, 0.0f32, 0.0f32, 1.0f32]
        ];

        transformation = transformation.dot(&translation_matrix);
    }

    let inverse_centering: Array2<f32> = array![
        [1.0f32, 0.0f32, 0.0f32, -(x_size as f32 / 2.0f32)],
        [0.0f32, 1.0f32, 0.0f32, -(y_size as f32 / 2.0f32)],
        [0.0f32, 0.0f32, 1.0f32, -(z_size as f32 / 2.0f32)],
        [0.0f32, 0.0f32, 0.0f32, 1.0f32]
    ];
    return transformation.dot(&inverse_centering).inv().unwrap();
}

pub fn apply_transformations<T: Interpolation>(
    volume: &ArrayView4<f32>,
    volume_shape: (usize, usize, usize, usize),
    transformation: &Array2<f32>,
    channels_value_multipliers: &Vec<f32>,
    channels_value_addends: &Vec<f32>,
) -> Array4<f32> {
    let (_, x_size, y_size, z_size) = volume_shape;

    let mut output_volume: Array4<f32> = Array4::zeros(volume.raw_dim());

    multizip((
        output_volume.outer_iter_mut(),
        channels_value_multipliers,
        channels_value_addends,
    ))
    .enumerate()
    .for_each(
        |(channel, (mut channel_view, value_multiplier, value_addend))| {
            for ((x, y, z), output_value) in channel_view.indexed_iter_mut() {
                let source_x = transformation.get([0, 0]).unwrap() * x as f32
                    + transformation.get([0, 1]).unwrap() * y as f32
                    + transformation.get([0, 2]).unwrap() * z as f32
                    + transformation.get([0, 3]).unwrap();
                let source_y = transformation.get([1, 0]).unwrap() * x as f32
                    + transformation.get([1, 1]).unwrap() * y as f32
                    + transformation.get([1, 2]).unwrap() * z as f32
                    + transformation.get([1, 3]).unwrap();
                let source_z = transformation.get([2, 0]).unwrap() * x as f32
                    + transformation.get([2, 1]).unwrap() * y as f32
                    + transformation.get([2, 2]).unwrap() * z as f32
                    + transformation.get([2, 3]).unwrap();

                let (nearest_source_x, nearest_source_y, nearest_source_z) =
                    (source_x as usize, source_y as usize, source_z as usize);

                // TODO: is casting i32 to usize always safe? (especially for negative numbers)
                *output_value = volume
                    .get([
                        channel,
                        nearest_source_x,
                        nearest_source_y,
                        nearest_source_z,
                    ])
                    .map(|source_value| {
                        T::interpolate(
                            (source_x, source_y, source_z),
                            (nearest_source_x, nearest_source_y, nearest_source_z),
                            *source_value,
                            &volume.index_axis(Axis(0), channel),
                            (x_size, y_size, z_size),
                        ) * value_multiplier
                            + value_addend
                    })
                    .unwrap_or(0.0f32);
            }
        },
    );

    output_volume
}
