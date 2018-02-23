include SimpleBoard.FromTile(HalfSquareTriangle);

type shape = (int, int);

let coordinates = ((w, h): shape) => {
  let v = ref([]);
  for (x in 0 to w - 1) {
    for (y in 0 to h - 1) {
      v := [(x, y), ...v^]
    }
  };
  v^
};

let fi = float_of_int;

let iof = int_of_float;

let auto_size = ((cwidth, cheight), hint_num) => {
  let dim = min(cwidth, cheight);
  let size = dim /. fi(hint_num) *. 1.2;
  let width = iof(cwidth /. size);
  let height = iof(cheight /. size);
  ((width * 2, height), size, (fi(width) *. size, fi(height) *. size))
};

let tau: float = 3.14159 *. 2.;
/* [%bs.raw "Math.PI * 2"]; */

let theta = tau /. 8.0 -. atan(0.5);

let n = tan(theta) *. sqrt(2.0) /. 2.0;

let offcenter = n /. sqrt(2.0);

let offset = (_shape, scale, (x, y)) => ((fi(x / 2) +. 0.5) *. scale, (fi(y) +. 0.5) *. scale);

let tile_center = (shape, scale, (x, y)) => {
  let (cx, cy) = offset(shape, scale, (x, y));
  let d = offcenter *. scale;
  if (HalfSquareTriangle.is_flipped((x, y))) {
    (cx +. d, cy +. d)
  } else {
    (cx -. d, cy -. d)
  }
};

let from_point = (shape, scale, (x, y)) => {
  let fy = y /. scale;
  let fx = x /. scale;
  let ix = iof(fx);
  let iy = iof(fy);
  let remx = fx -. floor(fx);
  let remy = fy -. floor(fy);
  (ix * 2 + (remx +. remy > 1.0 ? 1 : 0), iy)
};
