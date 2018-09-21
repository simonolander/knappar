let str = float => {
  let string = string_of_float(float);
  let lastChar = string.[String.length(string) - 1];
  if (lastChar == '.') {
    string ++ "0";
  } else {
    string;
  };
};