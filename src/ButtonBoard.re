type button = {
  index: int,
  on: bool,
  connectedButtonIds: list(int),
};

type state = {
  buttons: array(button),
  hoveredButtonId: option(int),
};

type action =
  | Click(int)
  | Hover(option(int));

let component = ReasonReact.reducerComponent("ButtonBoard");

let index_to_column = (width, index) => index mod width;

let index_to_row = (width, index) => index / width;

let toggle_button = button => {...button, on: !button.on};

let list_contains = element => List.exists((==)(element));

let updateStateOnButtonClick = (state, index) => {
  let connectedButtonIds = state.buttons[index].connectedButtonIds;
  let updateButton = button =>
    if (List.exists((==)(button.index), connectedButtonIds)) {
      {...button, on: !button.on};
    } else {
      button;
    };
  let buttons = state.buttons |> Array.map(updateButton);
  {...state, buttons};
};

let make = (~width: int, ~height: int, _children) => {
  ...component,
  initialState: () => {
    Random.self_init();
    let numberOfButtons = width * height;
    let buttons =
      Array.init(numberOfButtons, index =>
        {
          index,
          on: false,
          connectedButtonIds: [
            index,
            Random.int(numberOfButtons),
            Random.int(numberOfButtons),
          ],
        }
      );
    let rec scramble = (state, numberOfScrambles) =>
      if (numberOfScrambles > 0) {
        scramble(
          updateStateOnButtonClick(state, Random.int(numberOfButtons)),
          numberOfScrambles - 1,
        );
      } else {
        state;
      };
    let cleanState = {buttons, hoveredButtonId: None};
    scramble(cleanState, Random.int(numberOfButtons / 2));
  },
  reducer: (action, state) =>
    switch (action) {
    | Click(index) =>
      ReasonReact.Update(updateStateOnButtonClick(state, index))
    | Hover(option) =>
      switch (option) {
      | Some(index) =>
        ReasonReact.Update({...state, hoveredButtonId: Some(index)})
      | None => ReasonReact.Update({...state, hoveredButtonId: None})
      }
    },
  render: self => {
    let x = index => float_of_int(index_to_column(width, index)) *. 110.;
    let y = index => float_of_int(index_to_row(width, index)) *. 110.;
    let connectedToHoveredButtonIds =
      self.state.hoveredButtonId
      |> Option.map(Array.get(self.state.buttons))
      |> Option.map(button => button.connectedButtonIds)
      |> Option.default([]);

    let buttons =
      Array.map(
        button =>
          <Button
            x={x(button.index)}
            y={y(button.index)}
            width=100.
            height=100.
            on={button.on}
            indicated={
              list_contains(button.index, connectedToHoveredButtonIds)
            }
            hovered={
              switch (self.state.hoveredButtonId) {
              | Some(index) => button.index == index
              | None => false
              }
            }
            onClick={_ => self.send(Click(button.index))}
            onMouseOver={_ => self.send(Hover(Some(button.index)))}
            onMouseOut={_ => self.send(Hover(None))}
          />,
        self.state.buttons,
      );
    <svg width="1000" height="1000"> ...buttons </svg>;
  },
};