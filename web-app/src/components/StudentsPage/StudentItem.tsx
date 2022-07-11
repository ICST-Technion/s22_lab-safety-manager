import { Typography, Box, IconButton } from "@mui/material";
import DeleteIcon from "@mui/icons-material/Delete";
import React from "react";
import { deleteStudentDoc } from "../../firebase/dbRef";

export interface StudentsItemProps {
  student: string;
  cardId: string;
  perm_level: string;
  refetch: () => void;
}

export const StudentsItem: React.FC<StudentsItemProps> = ({
  student,
  cardId,
  perm_level,
  refetch,
}) => {
  const handleRemove = () => {
    deleteStudentDoc(cardId);
    refetch();
  };

  return (
    <Box
      sx={{
        display: "flex",
        flexDirection: "row",
        justifyContent: "space-evenly",
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
      <Typography> {student} </Typography>
      <Typography> {cardId} </Typography>
      <Typography> {perm_level} </Typography>
      <IconButton onClick={handleRemove}>
        <DeleteIcon />
      </IconButton>
    </Box>
  );
};
