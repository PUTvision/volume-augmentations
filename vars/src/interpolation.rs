use ndarray::ArrayView3;

pub trait Interpolation {
    fn interpolate(
        source_point: (f32, f32, f32),
        nearest_source_point: (usize, usize, usize),
        source_value: f32,
        source_volume: &ArrayView3<f32>,
        source_volume_shape: (usize, usize, usize),
    ) -> f32;
}

pub struct NearestNeighbour {}
impl Interpolation for NearestNeighbour {
    fn interpolate(
        _source_point: (f32, f32, f32),
        _nearest_source_point: (usize, usize, usize),
        source_value: f32,
        _source_volume: &ArrayView3<f32>,
        _source_volume_shape: (usize, usize, usize),
    ) -> f32 {
        return source_value;
    }
}

pub struct Linear {}
impl Interpolation for Linear {
    fn interpolate(
        source_point: (f32, f32, f32),
        nearest_source_point: (usize, usize, usize),
        source_value: f32,
        source_volume: &ArrayView3<f32>,
        source_volume_shape: (usize, usize, usize),
    ) -> f32 {
        let (nearest_x, nearest_y, nearest_z) = nearest_source_point;

        let x_difference = source_point.0 - nearest_x as f32;
        let y_difference = source_point.1 - nearest_y as f32;
        let z_difference = source_point.2 - nearest_z as f32;

        let mut first_neighbour = (nearest_x, nearest_y, nearest_z + 1);
        let mut second_neighbour = (nearest_x, nearest_y + 1, nearest_z);
        let mut third_neighbour = (nearest_x + 1, nearest_y, nearest_z);
        let mut fourth_neighbour = (nearest_x, nearest_y + 1, nearest_z + 1);
        let mut fifth_neighbour = (nearest_x + 1, nearest_y + 1, nearest_z);
        let mut sixth_neighbour = (nearest_x + 1, nearest_y, nearest_z + 1);
        let mut seventh_neighbour = (nearest_x + 1, nearest_y + 1, nearest_z + 1);

        let (x_size, y_size, z_size) = source_volume_shape;
        if nearest_x == x_size - 1 {
            third_neighbour = nearest_source_point;
            fifth_neighbour = second_neighbour;
            sixth_neighbour = first_neighbour;
            seventh_neighbour = fourth_neighbour;
        }
        if nearest_y == y_size - 1 {
            second_neighbour = nearest_source_point;
            fourth_neighbour = first_neighbour;
            fifth_neighbour = third_neighbour;
            seventh_neighbour = sixth_neighbour;
        }
        if nearest_z == z_size - 1 {
            first_neighbour = nearest_source_point;
            fourth_neighbour = second_neighbour;
            sixth_neighbour = third_neighbour;
            seventh_neighbour = fifth_neighbour;
        }

        return ((1f32 - x_difference)
            * (1f32 - y_difference)
            * (1f32 - z_difference)
            * source_value)
            + ((1f32 - x_difference)
                * (1f32 - y_difference)
                * z_difference
                * source_volume.get(first_neighbour).unwrap())
            + ((1f32 - x_difference)
                * y_difference
                * (1f32 - z_difference)
                * source_volume.get(second_neighbour).unwrap())
            + (x_difference
                * (1f32 - y_difference)
                * (1f32 - z_difference)
                * source_volume.get(third_neighbour).unwrap())
            + ((1f32 - x_difference)
                * y_difference
                * z_difference
                * source_volume.get(fourth_neighbour).unwrap())
            + (x_difference
                * y_difference
                * (1f32 - z_difference)
                * source_volume.get(fifth_neighbour).unwrap())
            + (x_difference
                * (1f32 - y_difference)
                * z_difference
                * source_volume.get(sixth_neighbour).unwrap())
            + (x_difference
                * y_difference
                * z_difference
                * source_volume.get(seventh_neighbour).unwrap());
    }
}
