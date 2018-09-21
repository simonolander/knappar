let component = ReasonReact.statelessComponent("Button");

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
      | (false, false) => "#359f7d"
      | (false, true) => "#86d1b9"
      | (true, false) => "#9f3535"
      | (true, true) => "#de8888"
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
        ("#aaaaaa", 5.);
      } else {
        ("#aaaaaa", 0.);
      };
    <rect
      x=(Utils.str(actual_x))
      y=(Utils.str(actual_y))
      width=(Utils.str(actual_width))
      height=(Utils.str(actual_height))
      onClick
      onMouseOver
      onMouseOut
      fill
      stroke
      strokeWidth=(Utils.str(strokeWidth))
    />;
  },
};