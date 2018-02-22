include EquilateralTriangle;

let adjacent_coord_simple = (direction) =>
  switch direction {
  | Up => (0, (-1))
  | Down => (0, 1)
  | Left => ((-1), 0)
  | Right => (1, 0)
  };

let is_flipped = ((x, y)) => x mod 2 !== y mod 2;

let adjacents = (pos) => adjacents_simple(is_flipped(pos));

let adjacent_coord = ((x, y), direction) => {
  let (dx, dy) = adjacent_coord_simple(direction);
  (x + dx, y + dy)
};

let direction_to_border = (pos) => direction_to_border_simple(is_flipped(pos));

let to_shape = (pos) => to_shape_simple(is_flipped(pos));
