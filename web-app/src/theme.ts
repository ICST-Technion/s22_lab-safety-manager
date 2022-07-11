import { createTheme, responsiveFontSizes } from "@mui/material";

export const getAppTheme = () => {
  let theme = createTheme({
    palette: {
      background: {
        default: "#bbdefb"
     },
      primary: {
        main: "#29b6f6",
        light: "#4fc3f7",
        dark: "#0288d1",
      },
      secondary: {
        main: "#29b6f6",
        light: "#4fc3f7",
        dark: "#0288d1",
      },
    },
    typography: {
      fontFamily: "Assistant",
    },
    direction: "rtl",
  });

  theme = responsiveFontSizes(theme);
  return theme;
};
