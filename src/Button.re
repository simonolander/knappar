let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~x: float,
      ~y: float,
      ~width: float,
      ~height: float,
      ~on: bool,
      ~onClick,
      _children,
    ) => {
  ...component,
  render: self =>
    <rect
      x={string_of_float(x)}
      y={string_of_float(y)}
      width={string_of_float(width)}
      height={string_of_float(height)}
      onClick
      fill={on ? "red" : "blue"}
    />,
};