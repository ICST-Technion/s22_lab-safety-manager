import {
  Box,
  Button,
  CircularProgress,
  Dialog,
  DialogActions,
  DialogContent,
  Typography,
  TextField,
  DialogTitle,
  DialogContentText,
} from "@mui/material";
import React, { useEffect, useState } from "react";
import { addMachineDoc, getMachinesDocs } from "../../firebase/dbRef";
import { MachineCard } from "./MachineCard";

export const MachinePages = () => {
  const [result, setResult] = useState<any>();
  const [refetch, setRefetch] = useState<boolean>(false);

  const [modalIsOpen, setModalIsOpen] = useState<boolean>(false);
  const [machineId, setMachineId] = useState<string>("");
  const [machineName, setMachineName] = useState<string>("");
  const [machineDuration, setMachineDuration] = useState<string>("");
  const [machinePrem, setMachinePrem] = useState<string>("");

  useEffect(() => {
    setRefetch(false);
    let active = true;
    load();
    return () => {
      active = false;
    };

    async function load() {
      setResult(undefined); // this is optional
      const res = await getMachinesDocs();
      if (!active) {
        return;
      }
      setResult(res);
    }
  }, [refetch]);

  const handleChangeName = (event: React.ChangeEvent<HTMLInputElement>) => {
    setMachineName(event.target.value);
  };

  const handleChangeID = (event: React.ChangeEvent<HTMLInputElement>) => {
    setMachineId(event.target.value);
  };

  const handleChangeDuration = (event: React.ChangeEvent<HTMLInputElement>) => {
    setMachineDuration(event.target.value);
  };

  const handleChangePremission = (event: React.ChangeEvent<HTMLInputElement>) => {
    setMachinePrem(event.target.value);
  };

  const handleRefetch = () => {
    setRefetch(true);
  };

  const submitMachine = () => {
    if (machineId === "" || machineName === "" || machineDuration === "" || machinePrem === "") {
      alert("You didnt enter valid parametrs");
    } else {
      addMachineDoc(machineId, {
        name: machineName,
        duration: machineDuration,
        perm_level: machinePrem,
      });
    }
    setModalIsOpen(false);
    handleRefetch();
  };

  return (
    <>
      <Box>
        <Box
          sx={{
            display: "flex",
            justifyContent: "space-between",
            marginBottom: "10px",
          }}
        >
          <Typography variant="h4"> Machines </Typography>
          <Button
            variant="contained"
            color="secondary"
            onClick={() => setModalIsOpen(true)}
          >
            Add a new Machine
          </Button>
        </Box>
        {!result ? (
          <CircularProgress />
        ) : (
          <Box
            sx={{
              display: "flex",
              gap: "8px",
              maxWidth: "700px",
              flexWrap: "wrap",
            }}
          >
            {result &&
              result.map((item: any) => (
                <MachineCard
                  key={item?.id ?? ""}
                  machineId={item?.id ?? ""}
                  machineName={item?.name ?? ""}
                  duration={item?.duration ?? ""}
                  perm_level={item?.perm_level ?? ""}
                  refetch={handleRefetch}
                />
              ))}
          </Box>
        )}
      </Box>

      <Dialog open={modalIsOpen} onClose={() => setModalIsOpen(false)}>
        <DialogTitle>Subscribe</DialogTitle>
        <DialogContent>
          <DialogContentText>Add a new Machine</DialogContentText>
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Name"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangeName}
          />
          <TextField
            autoFocus
            margin="dense"
            id="name" 
            label="Machine ID"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangeID}
          />
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Duration (in minutes)"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangeDuration}
          />
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Permission level"
            type="text"
            fullWidth
            variant="standard"
            onChange={handleChangePremission}
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={() => setModalIsOpen(false)}> Cancel </Button>
          <Button onClick={submitMachine}> Submit </Button>
        </DialogActions>
      </Dialog>
    </>
  );
};
