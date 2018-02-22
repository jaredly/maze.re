type state = {
  visited: array(int),
  edges: Generator.PairSet.t,
  frontier: list(int),
  next: list(int),
  step: int
};

let init = (start, size) => {
  visited: Array.make(size, 0),
  edges: Generator.PairSet.empty,
  frontier: [start],
  next: [],
  step: 0
};

let edges = (state) => state.edges;

let visited = (state) => state.visited;

let max_age = (state) => state.step;

let sortpair = (a, b) => a > b ? (b, a) : (a, b);

let add_edges = (~distance, adjacents, state, src) =>
  List.fold_left(
    ((next, edges, step), dest) =>
      if (state.visited[dest] > 0) {
        if (distance && state.visited[dest] > state.visited[src] + 1) {
          state.visited[dest] = state.visited[src] + 1;
          ([dest, ...next], Generator.PairSet.add(sortpair(src, dest), edges), step + 1)
        } else {
          (next, edges, step)
        }
      } else {
        state.visited[dest] = distance ? state.visited[src] + 1 : step + 1;
        ([dest, ...next], Generator.PairSet.add(sortpair(src, dest), edges), step + 1)
      },
    (state.next, state.edges, state.step),
    adjacents
  );

let step = (~distance, get_adjacent, state) =>
  switch state.frontier {
  | [] => state
  | [src] =>
    let (frontier, edges, step) = add_edges(~distance, get_adjacent(src), state, src);
    /* TODO add option to shuffle or not */
    {...state, frontier: distance ? frontier : Utils.shuffle(frontier), next: [], edges, step}
  | [src, ...rest] =>
    let (next, edges, step) = add_edges(~distance, get_adjacent(src), state, src);
    {...state, frontier: rest, next, edges, step}
  };

let finished = (state) => state.frontier === [];

/* hmm these can be shared */
let rec loop_to_end = (~distance, get_adjacent, state) =>
  if (! finished(state)) {
    loop_to_end(~distance, get_adjacent, step(~distance, get_adjacent, state))
  } else {
    state
  };

let runWithoutShuffling = (size, get_adjacent, start) => loop_to_end(~distance=true, get_adjacent, init(start, size));

/** To match Generator.T */
let init = size => init(Random.int(size), size);
let run = (size, get_adjacent) => loop_to_end(~distance=false, get_adjacent, init(size));
let step = step(~distance=false);
let loop_to_end = loop_to_end(~distance=false);