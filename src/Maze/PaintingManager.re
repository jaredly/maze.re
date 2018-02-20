module State = {
  type t('shape, 'coord, 'map) = {
    shape: 'shape,
    scale: float,
    outsize: (float, float),
    coords: array('coord),
    enabled: 'map
  };
};

module F = (Board: SimpleBoard.T, Gen: Generator.T) => {
  include Manager.F(Board, Gen);
  let create_enabled_map = (coords) =>
    Array.fold_left((map, c) => CoordMap.add(c, false, map), CoordMap.empty, coords);
  let paint_init = ((width, height), hint_size) => {
    let (shape, scale, outsize) = Board.auto_size((width, height), hint_size);
    let coords = Board.coordinates(shape) |> Array.of_list;
    let enabled = create_enabled_map(coords);
    State.{shape, scale, outsize, coords, enabled}
  };
  let toggle_all = (state, coord) => {
    open State;
    let enabled =
      Board.adjacents(state.shape, coord)
      |> List.map(Board.adjacent_coord(state.shape, coord))
      |> List.fold_left(
           (enabled, coord) =>
             CoordMap.mem(coord, enabled) ? CoordMap.add(coord, true, enabled) : enabled,
           state.State.enabled
         );
    {...state, State.enabled}
  };
  let toggle_point = (state, (x, y)) => {
    open State;
    let {shape, scale, enabled} = state;
    let coord = Board.from_point(shape, scale, (x, y));
    if (CoordMap.mem(coord, enabled)) {
      let state = toggle_all(state, coord);
      let enabled = CoordMap.add(coord, true, state.enabled);
      {...state, State.enabled}
    } else {
      state
    }
  };
  let realize_state = ({State.shape, scale, outsize, coords, enabled}) => {
    let coords =
      List.filter((coord) => CoordMap.find(coord, enabled), Array.to_list(coords)) |> Array.of_list;
    switch coords {
    | [||] => None
    | coords =>
      let coord_map = create_index_map(coords);
      let count = Array.length(coords);
      let gen_state = Gen.init(count);
      let get_adjacent = get_adjacent(shape, coords, coord_map);
      Some(
        Manager.State.{count, shape, scale, outsize, gen_state, get_adjacent, coords, coord_map}
      )
    }
  };
  let paint_walls = (state) =>
    switch (realize_state(state)) {
    | None => []
    | Some(mstate) => all_walls(mstate)
    };
  let paint_shapes = ({State.coords, scale, shape, enabled}) =>
    /*all_shapes (realize_state state)*/
    /*List.map*/
    List.filter((coord) => CoordMap.find(coord, enabled), Array.to_list(coords))
    |> List.map(
         (coord) => {
           let offset = Board.offset(shape, scale, coord);
           let shape = Board.tile_at_coord(shape, coord);
           /*let visited = Array.get (Gen.visited gen_state) i;*/
           (Shape.transform(offset, scale, shape), CoordMap.find(coord, enabled) ? 8 : 2)
         }
       )
    |> Array.of_list;
};
