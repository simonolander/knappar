let component = ReasonReact.statelessComponent("Main");

let make = _children => {
  ...component,
  render: self => <div> <ButtonBoard width=4 height=4 /> </div>,
};