include SimpleBoard.FromTile(AlternatingEquilateralTriangle);

/** width, height */
type shape = (int, int);

let coordinates = ((width, height): shape) => {
  let v = ref([]);
  for (y in 0 to height - 1) {
    for (x in 0 to width - 1) {
      v := [(x, y), ...v^]
    }
  };
  v^
};

let hsq3 = sqrt(3.0) /. 2.0;

let fi = float_of_int;
let iof = int_of_float;

let auto_size = ((cwidth, cheight), hint_num) => {
  /* let size = cwidth /. float_of_int(hint_num);
  (hint_num, size, (cwidth, cwidth *. sqrt(3.0) /. 2.0)) */

  /** TODO fix this, width is not equal to height */
  let dim = min(cwidth, cheight);
  let size = dim /. fi(hint_num);
  let width = iof(cwidth /. size);
  let height = iof(cheight /. size /. hsq3);
  ((width * 2, height), size, (fi(width) *. size, fi(height) *. size))
};

let offset = (shape, scale, (x, y)) => {
  open Utils.Float;
  let (fx, fy) = (fi(x), fi(y));
  ((0.5 +. fx / 2.0) * scale, (fy + 0.5) * hsq3 * scale)
};

let offcenter = 0.5 -. 0.5 /. sqrt(3.0);

let tile_center = (shape, scale, (x, y)) => {
  let (ax, ay) = offset(shape, scale, (x, y));
  let dy =
    if (AlternatingEquilateralTriangle.is_flipped((x, y))) {
      -. scale
      *. offcenter
      /. 1.5
      /*-.scale *. hsq3 /. 4.0*/
    } else {
      scale
      *. offcenter
      /. 1.5
      /*scale *. hsq3 /. 4.0*/
    };
  (ax, ay +. dy)
};

let from_point = ((width, height), scale, (x, y)) => {
  open Utils.Float;
  /** hsq3 * scale is the height of the triangle, where scale is the width (length of a side) */
  let fy = y / hsq3 / scale;
  let fx = (x / scale -. 0.5) * 2.0;
   /* -. (int_of_float(fy) mod 2 == 0 ? 0. : 1.); */
  let ix = int_of_float(fx);
  let remy = fy -. floor(fy);
  let remx = fx -. floor(fx);
  let iy = int_of_float(fy);
  let ix = if (ix mod 2 == iy mod 2) {
    (remy > remx) ? ix : Pervasives.(ix + 1)
  } else {
    (remx + remy < 1.0) ? ix : Pervasives.(ix + 1)
  };
  (ix, iy)
};


let adjacent_coord_simple = (direction) =>
  switch direction {
  | EquilateralTriangle.Up => (0, (-1))
  | Down => (0, 1)
  | Left => ((-1), 0)
  | Right => (1, 0)
  };

let adjacent_coord = (_, (x, y), direction) => {
  let (dx, dy) = adjacent_coord_simple(direction);
  (x + dx, y + dy)
};