import {
  Typography,
  Box,
  IconButton,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogContentText,
  TextField,
  Button,
  DialogActions,
} from "@mui/material";
import DeleteIcon from "@mui/icons-material/Delete";
import AddCircleOutlineIcon from "@mui/icons-material/AddCircleOutline";
import React, { useState } from "react";
import { addStudentDoc, deleteRegistratioDoc } from "../../firebase/dbRef";

export interface RegistrationItemProps {
  regDocId: string;
  cardId: string;
  date: string;
  refetch: () => void;
}

export const RegistrationItem: React.FC<RegistrationItemProps> = ({
  regDocId,
  cardId,
  date,
  refetch,
}) => {
  const [modalIsOpen, setModalIsOpen] = useState<boolean>(false);
  const [studentName, setStudentName] = useState<string>("");
  const [studentPerm, setStudentPerm] = useState<string>("");

  const handleChangeName = (event: React.ChangeEvent<HTMLInputElement>) => {
    setStudentName(event.target.value);
  };

  const handleChangePrem = (event: React.ChangeEvent<HTMLInputElement>) => {
    setStudentPerm(event.target.value);
  };

  const handleRemove = () => {
    deleteRegistratioDoc(regDocId);
    refetch();
  };

  const submitStudent = () => {
    if (studentName === "" || studentPerm === "") {
      alert("You didnt enter any name or perm");
    } else {
      addStudentDoc(cardId, {
        name: studentName,
        perm_level: studentPerm,
      });
      deleteRegistratioDoc(regDocId);
    }
    setModalIsOpen(false);
    refetch();
  };

  return (
    <>
      <Box
        sx={{
          display: "flex",
          flexDirection: "row",
          justifyContent: "space-around",
          alignItems: "center",
          margin: "8px",
          width: 700,
          height: 70,
          border: "1px solid #0277bd",
          borderRadius: "20px",
          backgroundColor: "#e3f2fd",
          "&:hover": {
            backgroundColor: "#80d8ff",
            opacity: [0.9, 0.8, 0.7],
          },
        }}
      >
        <Typography> {cardId} </Typography>
        <Typography> {date} </Typography>
        <IconButton onClick={handleRemove}>
          <DeleteIcon />
        </IconButton>
        <IconButton onClick={() => setModalIsOpen(true)}>
          <AddCircleOutlineIcon />
        </IconButton>
      </Box>
      <Dialog open={modalIsOpen} onClose={() => setModalIsOpen(false)}>
        <DialogTitle>Subscribe</DialogTitle>
        <DialogContent>
          <DialogContentText>
            To conect this card id to student name, just enter the student name
            here :)
          </DialogContentText>
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Enter Student Name"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangeName}
          />
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Enter Student Permissions"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangePrem}
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={() => setModalIsOpen(false)}> Cancel </Button>
          <Button onClick={submitStudent}> Submit </Button>
        </DialogActions>
      </Dialog>
    </>
  );
};
