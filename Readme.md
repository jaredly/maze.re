# Maze.re

A reason library for generating mazes!

Cross-platform ready (can compile to js via bucklescript, or to native).

### Maze shapes supported

- [x] rectangular (normal)
- [x] rectangle w/ 45° triangles (split squares)
- [x] rectangle where each square is split into 4 45° triangles
- [x] circle
- [x] equilateral triangles
- [x] hexagons in a hex shape
- [x] hexagons in a square
- [x] hexagons in a triangle shape

### Maze generating algorithms

- [x] Bredth first search (quite boring)
- [x] Bredth first w/ random frontier choices (somewhat less boring)
- [x] Depth first search w/ random pruning (this is the one that will give you interesting mazes)

### TODO

- [x] make a "distance from starting point" function so that the goal is a predictable distance from the start
- [ ] make a more complicated algorithm that is "number of branch choices from the starting point"
      because the current one counts a single path of 5 blocks as "5 farther away", but it's not "5 harder"

### API

```
/* Pick a board type */
let module Board = Mazere.FourSquare;
/* Pick an Algorithm */
let module Alg = Mazere.NewDepth.F(Mazere.NewDepth.RandomConfig({}));

/* Put them together! */
let module Maze = Mazere.Manager.F(Board, Alg);

let (width, height) = (800., 800.);
let size_hint = 15; /* The desired "maze size" (generally "number of tiles across") */
let state = Maze.init((width, height), size_hint);
/* generate the full maze! If you're animating, you'll just want to run Maze.step(state) */
let state = Maze.loop_to_end(state);

let walls = Maze.all_walls(state);
/* These walls are of this type (See Border.re)
type t =
  | Arc((float, float, float, float, float))
  | Line(((float, float), (float, float)));
*/

let goal: (float, float) = Maze.randomCoord(state);
```