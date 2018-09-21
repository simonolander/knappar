let component = ReasonReact.statelessComponent("Button");

let str = float => {
  let string = string_of_float(float);
  let lastChar = string.[String.length(string) - 1];
  if (lastChar == '.') {
    string ++ "0";
  } else {
    string;
  };
};

let shrinkage = 0.85;

let make =
    (
      ~x: float,
      ~y: float,
      ~width: float,
      ~height: float,
      ~on: bool,
      ~hovered: bool,
      ~indicated: bool,
      ~onClick,
      ~onMouseOver,
      ~onMouseOut,
      _children,
    ) => {
  ...component,
  render: _self => {
    let fill =
      switch (on, indicated) {
      | (false, false) => "#00f"
      | (false, true) => "#44f"
      | (true, false) => "#f00"
      | (true, true) => "#f44"
      };
    let (actual_width, actual_height) =
      if (indicated) {
        (width, height);
      } else {
        (width *. shrinkage, height *. shrinkage);
      };
    let (actual_x, actual_y) = (
      x +. (width -. actual_width) /. 2.,
      y +. (height -. actual_height) /. 2.,
    );
    let (stroke, strokeWidth) =
      if (indicated) {
        ("black", 5.);
      } else {
        ("black", 0.);
      };
    <rect
      x=(str(actual_x))
      y=(str(actual_y))
      width=(str(actual_width))
      height=(str(actual_height))
      onClick
      onMouseOver
      onMouseOut
      fill
      stroke
      strokeWidth=(str(strokeWidth))
    />;
  },
};