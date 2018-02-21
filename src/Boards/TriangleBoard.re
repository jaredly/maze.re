include SimpleBoard.FromTile(EquilateralTriangle);

/** The height of the triangle, in rows */
type shape = int;

let coordinates = (size: shape) => {
  let v = ref([]);
  for (y in 0 to size - 1) {
    for (x in 0 to y * 2) {
      v := [(x, y), ...v^]
    }
  };
  v^
};

let auto_size = ((cwidth, _cheight), hint_num) => {
  let size = cwidth /. float_of_int(hint_num);
  (hint_num, size, (cwidth, cwidth *. sqrt(3.0) /. 2.0))
};

let hsq3 = sqrt(3.0) /. 2.0;

let fi = float_of_int;

let offset = (shape, scale, (x, y)) => {
  open Utils.Float;
  let (fx, fy) = (fi(x), fi(y));
  ((fx - fy + fi(shape)) / 2.0 * scale, (fy + 0.5) * hsq3 * scale)
};

let offcenter = 0.5 -. 0.5 /. sqrt(3.0);

let tile_center = (shape, scale, (x, y)) => {
  let (ax, ay) = offset(shape, scale, (x, y));
  let dy =
    if (EquilateralTriangle.is_flipped((x, y))) {
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

let from_point = (shape, scale, (x, y)) => {
  let fshape = fi(shape);

  open Utils.Float;
  /** hsq3 * scale is the height of the triangle, where scale is the width (length of a side) */
  let fy = y / hsq3 / scale;
  let fx = x * 2.0 / scale - fshape + floor(fy);
  let ix = int_of_float(fx);
  let remy = fy -. floor(fy);
  let remx = fx -. floor(fx);
  let ix = if (ix mod 2 == 0) {
    (remy > remx) ? ix : Pervasives.(ix + 1)
  } else {
    (remx + remy < 1.0) ? ix : Pervasives.(ix + 1)
  };
  (ix, int_of_float(fy))
};
