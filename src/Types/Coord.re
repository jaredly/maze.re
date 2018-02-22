module type T = {
  type t;
  let show: t => string;
  let offset: (t, t) => t; let compare: (t, t) => int;};
