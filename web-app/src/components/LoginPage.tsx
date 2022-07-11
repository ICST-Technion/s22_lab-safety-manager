import { Button, Stack, Typography } from "@mui/material";
import React from "react";
import SendIcon from "@mui/icons-material/Send";
import logo from './logo.png';

export interface LoginPageProps {
  loginHandler: () => void;
}
export const LoginPage: React.FC<LoginPageProps> = ({ loginHandler }) => {
  return (
    <Stack alignItems="center" spacing={5} sx={{ marginTop: "40px" }}>
      <img src={logo} alt="Logo" />
      {/* <Typography variant="h3"> Welcome to Safety Manager </Typography> */}
      <Button onClick={loginHandler} variant="contained" endIcon={<SendIcon />}>
        Login With Google
      </Button>
    </Stack>
  );
};
