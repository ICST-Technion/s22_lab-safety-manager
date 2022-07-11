import {
  Box,
  Typography,
  Button,
  Avatar,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  IconButton,
} from "@mui/material";
import React, { useState } from "react";
import { Page } from "../types";
import { signOut } from "firebase/auth";
import { auth } from "../firebase";
import logo from './logo.png';

interface HeaderProps {
  setPage: (page: Page) => void;
  user: any;
}
export const Header: React.FC<HeaderProps> = ({ setPage, user }) => {
  const [modalIsOpen, setModalIsOpen] = useState<boolean>(false);

  const handleLogout = () => {
    signOut(auth);
    window.location.reload();
  };
  return (
    <>
      <img src={logo} alt="Logo" />
      <Box
        sx={{
          display: "flex",
          justifyContent: "space-between",
          alignItems: "center",
          // width: "800px",
        }}
      >
        {/* <Typography fontSize={"37px"}> Safety Manager </Typography> */}
        <Box
          sx={{
            display: "flex",
            justifyContent: "space-between",
            alignItems: "center",
            width: "420px",
          }} >
          <Button variant="contained" onClick={() => setPage("Registration")}>
            {" "}
            Registration{" "}
          </Button>
          <Button variant="contained" onClick={() => setPage("Students")}>
            {" "}
            Students{" "}
          </Button>
          <Button variant="contained" onClick={() => setPage("Machines")}>
            {" "}
            Machinces{" "}
          </Button>
          <Button variant="contained" onClick={() => setPage("History")}>
            {" "}
            History{" "}
          </Button>
        </Box>
        <IconButton onClick={() => setModalIsOpen(true)}>
          {user.user.photoURL && <Avatar src={user.user.photoURL}></Avatar>}
        </IconButton>
      </Box>
      <Dialog open={modalIsOpen} onClose={() => setModalIsOpen(false)}>
        <DialogTitle>Hi {user?.user?.displayName ?? ":)"}</DialogTitle>
        <DialogContent></DialogContent>
        <DialogActions>
          <Button onClick={() => setModalIsOpen(false)}> Cancel </Button>
          <Button onClick={handleLogout}> Logout </Button>
        </DialogActions>
      </Dialog>
    </>
  );
};
