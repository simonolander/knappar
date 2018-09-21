let map = (func, arg) =>
  switch (arg) {
  | Some(a) => Some(func(a))
  | None => None
  };

let flatMap = (func, arg) =>
  switch (arg) {
  | Some(a) => func(a)
  | None => None
  };

let filter = (func, arg) =>
  switch (arg) {
  | Some(a) =>
    if (func(a)) {
      Some(a);
    } else {
      None;
    }
  | None => None
  };

let default = (default, arg) =>
  switch (arg) {
  | Some(a) => a
  | None => default
  };