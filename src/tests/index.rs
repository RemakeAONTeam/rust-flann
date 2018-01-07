use super::*;

#[test]
fn builds_and_adds() {
    let mut index = Index::<f32, typenum::U3>::new(
        &[
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
        ],
        Parameters::default(),
    ).unwrap();
    assert_eq!(index.len(), 5);
    index.add(&Default::default(), None);
    assert_eq!(index.len(), 6);
    index.add_multiple(&[], None);
    assert_eq!(index.len(), 6);
    index.add_multiple(
        &[
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
        ],
        None,
    );
    assert_eq!(index.len(), 10);
}

#[test]
fn get_accesses_right_item() {
    let mut index = Index::<f32, typenum::U3>::new(
        &[
            arr![f32; 1, 2, 3],
            arr![f32; 4, 5, 6],
            arr![f32; 7, 8, 9],
            arr![f32; 10, 11, 12],
            arr![f32; 13, 14, 15],
        ],
        Parameters::default(),
    ).unwrap();

    index.add(&arr![f32; 16, 17, 18], None);

    index.add_multiple(&[], None);

    index.add_multiple(
        &[
            arr![f32; 19, 20, 21],
            arr![f32; 22, 23, 24],
            arr![f32; 25, 26, 27],
            arr![f32; 28, 29, 30],
        ],
        None,
    );

    assert_eq!(index.get(0), Some(arr![f32; 1, 2, 3]));
    assert_eq!(index.get(1), Some(arr![f32; 4, 5, 6]));
    assert_eq!(index.get(2), Some(arr![f32; 7, 8, 9]));
    assert_eq!(index.get(3), Some(arr![f32; 10, 11, 12]));
    assert_eq!(index.get(4), Some(arr![f32; 13, 14, 15]));
    assert_eq!(index.get(5), Some(arr![f32; 16, 17, 18]));
    assert_eq!(index.get(6), Some(arr![f32; 19, 20, 21]));
    assert_eq!(index.get(7), Some(arr![f32; 22, 23, 24]));
    assert_eq!(index.get(8), Some(arr![f32; 25, 26, 27]));
    assert_eq!(index.get(9), Some(arr![f32; 28, 29, 30]));
    assert_eq!(index.get(10), None);
}