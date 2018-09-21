let component = ReasonReact.statelessComponent("Main");

let make = _children => {
  ...component,
  render: self =>
    <div
      style=(
        ReactDOMRe.Style.make(
          ~height="100%",
          ~display="flex",
          ~alignItems="center",
          ~justifyContent="center",
          (),
        )
      )>
      <ButtonBoard width=4 height=4 />
    </div>,
};