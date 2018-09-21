type button = {
  index: int,
  on: bool,
  connectedButtonIds: list(int),
};

type state = {buttons: array(button)};

type action =
  | Click(int);

let component = ReasonReact.reducerComponent("ButtonBoard");

let index_to_column = (width, index) => index mod width;

let index_to_row = (width, index) => index / width;

let toggle_button = button => {...button, on: !button.on};

let make = (~width: int, ~height: int, _children) => {
  ...component,
  initialState: () => {
    let numberOfButtons = width * height;
    let buttons =
      Array.init(numberOfButtons, index =>
        {index, on: false, connectedButtonIds: SI.singleton(index)}
      );
    {buttons: buttons};
  },
  reducer: (action, state) =>
    switch (action) {
    | Click(index) =>
      let button = state.buttons[index];
      let newButton = {...button, on: !button.on};
      let newButtons = Array.copy(state.buttons);
      newButtons[index] = newButton;
      ReasonReact.Update({...state, buttons: newButtons});
    },
  render: self => {
    let x = index => float_of_int(index_to_column(width, index)) *. 110.;
    let y = index => float_of_int(index_to_row(width, index)) *. 110.;
    let buttons =
      Array.map(
        button =>
          <Button
            x={x(button.index)}
            y={y(button.index)}
            width=100.
            height=100.
            on={button.on}
            onClick={_ => self.send(Click(button.index))}
          />,
        self.state.buttons,
      );
    <svg width="1000" height="1000"> ...buttons </svg>;
  },
};