type button = {
  index: int,
  on: bool,
  connectedButtonIds: list(int),
};

type state = {
  buttons: array(button),
  hoveredButtonIndex: option(int),
};

type action =
  | Click(int)
  | Hover(option(int));

let component = ReasonReact.reducerComponent("ButtonBoard");

let index_to_column = (width, index) => index mod width;

let index_to_row = (width, index) => index / width;

let row_col_to_index = (row, column, numberOfColumns) =>
  row * numberOfColumns + column;

let toggle_button = button => {...button, on: ! button.on};

let list_contains = element => List.exists((==)(element));

let neighbours = (index, numberOfColumns, numberOfRows) => {
  let (row, column) = (
    index_to_row(numberOfColumns, index),
    index_to_column(numberOfColumns, index),
  );
  let prepend_if = (bool, value, list) =>
    if (bool) {
      [value, ...list];
    } else {
      list;
    };

  prepend_if(row > 0, row_col_to_index(row - 1, column, numberOfColumns), [])
  |> prepend_if(
       column > 0,
       row_col_to_index(row, column - 1, numberOfColumns),
     )
  |> prepend_if(
       row < numberOfRows - 1,
       row_col_to_index(row + 1, column, numberOfColumns),
     )
  |> prepend_if(
       column < numberOfColumns - 1,
       row_col_to_index(row, column + 1, numberOfColumns),
     );
};

let array_filter = (func, array) =>
  Array.to_list(array) |> List.filter(func) |> Array.of_list;

let array_pick_random = array =>
  if (Array.length(array) == 0) {
    None;
  } else {
    Some(array[Random.int(Array.length(array))]);
  };

let neighbourhood = (index, remaining, numberOfColumns, numberOfRows) => {
  let rec neighbourhood' = (remaining, used, available) =>
    if (remaining > 0) {
      let actually_available = value => ! list_contains(value, used);
      let maybePicked =
        array_pick_random(array_filter(actually_available, available));
      switch (maybePicked) {
      | None => used
      | Some(picked) =>
        neighbourhood'(
          remaining - 1,
          [picked, ...used],
          Array.concat([
            available,
            Array.of_list(neighbours(picked, numberOfColumns, numberOfRows)),
          ]),
        )
      };
    } else {
      used;
    };
  neighbourhood'(
    remaining - 1,
    [index],
    Array.of_list(neighbours(index, numberOfColumns, numberOfRows)),
  );
};

let updateStateOnButtonClick = (state, index) => {
  let connectedButtonIds = state.buttons[index].connectedButtonIds;
  let updateButton = button =>
    if (list_contains(button.index, connectedButtonIds)) {
      {...button, on: ! button.on};
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
          connectedButtonIds: neighbourhood(index, 4, width, height),
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
    let cleanState = {buttons, hoveredButtonIndex: None};
    scramble(cleanState, Random.int(numberOfButtons / 2));
  },
  reducer: (action, state) =>
    switch (action) {
    | Click(index) =>
      ReasonReact.Update(updateStateOnButtonClick(state, index))
    | Hover(option) =>
      switch (option) {
      | Some(index) =>
        ReasonReact.Update({...state, hoveredButtonIndex: Some(index)})
      | None => ReasonReact.Update({...state, hoveredButtonIndex: None})
      }
    },
  render: self => {
    let x = index =>
      float_of_int(index_to_column(width, index)) *. 110. +. 5.;
    let y = index => float_of_int(index_to_row(width, index)) *. 110. +. 5.;
    let connectedToHoveredButtonIndexs =
      self.state.hoveredButtonIndex
      |> Option.map(Array.get(self.state.buttons))
      |> Option.map(button => button.connectedButtonIds)
      |> Option.default([]);
    let (button_width, button_height) = (100., 100.);
    let (svg_width, svg_height) = (
      button_width *. float_of_int(width) +. 10. *. float_of_int(width),
      button_height *. float_of_int(height) +. 10. *. float_of_int(height),
    );

    let buttons =
      Array.map(
        button =>
          <Button
            x=(x(button.index))
            y=(y(button.index))
            width=button_width
            height=button_height
            on=button.on
            indicated=(
              list_contains(button.index, connectedToHoveredButtonIndexs)
            )
            hovered=(
              self.state.hoveredButtonIndex
              |> Option.map((==)(button.index))
              |> Option.default(false)
            )
            onClick=(_ => self.send(Click(button.index)))
            onMouseOver=(_ => self.send(Hover(Some(button.index))))
            onMouseOut=(_ => self.send(Hover(None)))
          />,
        self.state.buttons,
      );
    <svg width=(Utils.str(svg_width)) height=(Utils.str(svg_height))>
      ...buttons
    </svg>;
  },
};