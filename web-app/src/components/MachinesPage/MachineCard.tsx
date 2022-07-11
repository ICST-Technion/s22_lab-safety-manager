import {
  Card,
  CardContent,
  Typography,
  CardActions,
  Button,
} from "@mui/material";
import React from "react";
import { deleteMachineDoc } from "../../firebase/dbRef";

export interface MachineCardProps {
  machineName: string;
  duration: string;
  perm_level: number;
  machineId: string;
  refetch: () => void;
}

export const MachineCard: React.FC<MachineCardProps> = ({
  machineName,
  duration,
  perm_level,
  machineId,
  refetch,
}) => {
  const handleRemoveMachine = () => {
    deleteMachineDoc(machineId);
    refetch();
  };

  return (
    <Card
      sx={{
        boxShadow: "none",
        border: "1px solid #0277bd",
        borderRadius: "20px",
        backgroundColor: "#e3f2fd",
        "&:hover": {
          backgroundColor: "#80d8ff",
          opacity: [0.9, 0.8, 0.7],
        },
      }}
    >
      <CardContent>
        <Typography gutterBottom variant="h5" component="div">
          {machineName}
        </Typography>
        <Typography variant="body2" color="text.secondary">
          Machine ID: {machineId}
        </Typography>
        <Typography variant="body2" color="text.secondary">
          Duartion: {duration} minutes
        </Typography>
        <Typography variant="body2" color="text.secondary">
          Permission: {perm_level}
        </Typography>
      </CardContent>
      <CardActions>
        <Button size="medium" color="secondary" onClick={handleRemoveMachine}>
          Delete
        </Button>
      </CardActions>
    </Card>
  );
};
