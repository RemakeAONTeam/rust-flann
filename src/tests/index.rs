use super::*;

#[test]
fn builds_and_adds() {
    let mut index = Index::<f32, typenum::U3>::new(
        vec![
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
        ],
        Parameters::default(),
    ).unwrap();
    assert_eq!(index.count(), 5);
    index.add(Default::default(), None);
    assert_eq!(index.count(), 6);
    index.add_multiple(vec![], None);
    assert_eq!(index.count(), 6);
    index.add_multiple(
        vec![
            Default::default(),
            Default::default(),
            Default::default(),
            Default::default(),
        ],
        None,
    );
    assert_eq!(index.count(), 10);
}

#[test]
fn get_accesses_right_item() {
    let mut index = Index::<f32, typenum::U3>::new(
        vec![
            arr![f32; 1, 2, 3],
            arr![f32; 4, 5, 6],
            arr![f32; 7, 8, 9],
            arr![f32; 10, 11, 12],
            arr![f32; 13, 14, 15],
        ],
        Parameters::default(),
    ).unwrap();

    index.add(arr![f32; 16, 17, 18], None);

    index.add_multiple(vec![], None);

    index.add_multiple(
        vec![
            arr![f32; 19, 20, 21],
            arr![f32; 22, 23, 24],
            arr![f32; 25, 26, 27],
            arr![f32; 28, 29, 30],
        ],
        None,
    );

    assert_eq!(index.get(0), Some(&arr![f32; 1.0, 2.0, 3.0]));
    assert_eq!(index.get(1), Some(&arr![f32; 4.0, 5.0, 6.0]));
    assert_eq!(index.get(2), Some(&arr![f32; 7.0, 8.0, 9.0]));
    assert_eq!(index.get(3), Some(&arr![f32; 10.0, 11.0, 12.0]));
    assert_eq!(index.get(4), Some(&arr![f32; 13.0, 14.0, 15.0]));
    assert_eq!(index.get(5), Some(&arr![f32; 16.0, 17.0, 18.0]));
    assert_eq!(index.get(6), Some(&arr![f32; 19.0, 20.0, 21.0]));
    assert_eq!(index.get(7), Some(&arr![f32; 22.0, 23.0, 24.0]));
    assert_eq!(index.get(8), Some(&arr![f32; 25.0, 26.0, 27.0]));
    assert_eq!(index.get(9), Some(&arr![f32; 28.0, 29.0, 30.0]));
    assert_eq!(index.get(10), None);
}

#[test]
fn nearest_neighbor_returns_correct_item() {
    let index = Index::<f32, typenum::U3>::new(
        vec![
            arr![f32; 0, 0, 0],
            arr![f32; 0, 0, 1],
            arr![f32; 0, 1, 0],
            arr![f32; 0, 1, 1],
            arr![f32; 1, 0, 0],
            arr![f32; 1, 0, 1],
            arr![f32; 1, 1, 0],
            arr![f32; 1, 1, 1],
        ],
        Parameters::default(),
    ).unwrap();

    assert_eq!(index.find_nearest_neighbor(&arr![f32; -1, -1, -1]).0, 0);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; -1, -1, 2]).0, 1);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; -1, 2, -1]).0, 2);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; -1, 2, 2]).0, 3);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; 2, -1, -1]).0, 4);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; 2, -1, 2]).0, 5);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; 2, 2, -1]).0, 6);
    assert_eq!(index.find_nearest_neighbor(&arr![f32; 2, 2, 2]).0, 7);
}

#[test]
fn nearest_neighbors_returns_correct_item() {
    type Point2 = Index::<f32, typenum::U2>;
    let data = vec![
            arr![f32; 413, 800],
            arr![f32; 256, 755],
            arr![f32; 843, 586],
            arr![f32; 922, 823],
            arr![f32; 724, 789],
            arr![f32; 252, 39],
            arr![f32; 350, 369],
            arr![f32; 339, 247],
            arr![f32; 212, 653],
            arr![f32; 881, 714], ];
    let mut index = Point2::new(
        vec![
            Default::default(),
        ],
        Parameters::default(),
    ).unwrap();
    for v in data.clone() {
        index.add(v, None);
    }
    let (res1, res2) = index.find_nearest_neighbors(&data,3);
    
    //[1, 2, 9, 2, 9, 1, 3, 10, 5, 4, 5, 10, 5, 4, 10, 6, 8, 0, 7, 8, 9, 8, 7, 6, 9, 2, 1, 10, 3, 5]
    //[0, 26674, 62010, 0, 12340, 26674, 0, 17828, 55370, 0, 27490, 46628, 0, 27490, 30274, 0, 50833, 65025, 0, 15005, 99700, 0, 15005, 50833, 0, 12340, 62010, 0, 17828, 30274]
    assert_eq!(res1[0], 1); assert_eq!(res1[1], 2); assert_eq!(res1[2], 9);
    assert_eq!(res1[3], 2); assert_eq!(res1[4], 9); assert_eq!(res1[5], 1);
    assert_eq!(res1[6], 3); assert_eq!(res1[7], 10); assert_eq!(res1[8], 5);

    assert_eq!(res2[0], 0f32);
    assert_eq!(res2[1], 26674f32);
    assert_eq!(res2[2], 62010f32);
}

#[test]
fn search_radius_returns_correct_item() {
    let index = Index::<f32, typenum::U3>::new(
        vec![
            arr![f32; 0, 0, 0],
            arr![f32; 0, 0, 1],
            arr![f32; 0, 1, 0],
            arr![f32; 0, 1, 1],
            arr![f32; 1, 0, 0],
            arr![f32; 1, 0, 1],
            arr![f32; 1, 1, 0],
            arr![f32; 1, 1, 1],
        ],
        Parameters::default(),
    ).unwrap();

    let mut indices = index
        .search_radius(&arr![f32; 0, 0, -1], 1.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![0]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 1.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![4]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 10.0, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![0, 1, 2, 3, 4, 5, 6, 7]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 10.0, 0)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 2.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![4, 5, 6]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 3.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![4, 5, 6, 7]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 4.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![0, 4, 5, 6, 7]);

    let mut indices = index
        .search_radius(&arr![f32; 2, 0, 0], 6.1, 10)
        .into_iter()
        .map(|v| v.0)
        .collect::<Vec<usize>>();
    indices.sort();
    assert_eq!(indices, vec![0, 1, 2, 3, 4, 5, 6, 7]);
}
