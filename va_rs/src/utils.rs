pub fn angles_to_radians(angles: &(f32, f32, f32)) -> (f32, f32, f32) {
    (
        angles.0.to_radians(),
        angles.1.to_radians(),
        angles.2.to_radians(),
    )
}
