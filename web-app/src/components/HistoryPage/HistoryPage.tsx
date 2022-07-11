import { Box, CircularProgress, Typography } from "@mui/material";
import { useEffect, useState } from "react";
import { getHistoryDocs } from "../../firebase/dbRef";
import { HistoryItem } from "./HistoryItem";
import Stack from '@mui/material/Stack';
import { styled } from '@mui/material/styles';
import Paper from '@mui/material/Paper';


const Item = styled(Paper)(({ theme }) => ({
    backgroundColor: '#64b5f6',
    ...theme.typography.body2,
    padding: theme.spacing(1),
    textAlign: 'center',
    color: theme.palette.text.primary,
  }));

export const HistoryPage = () => {
  const [result, setResult] = useState<any>();
  const [refetch, setRefetch] = useState<boolean>(false);

  const handleRefetch = () => {
    setRefetch(true);
  };

  useEffect(() => {
    setRefetch(false);

    let active = true;
    load();
    return () => {
      active = false;
    };

    async function load() {
      setResult(undefined); // this is optional
      let res = await getHistoryDocs();
      if (!active) {
        return;
      }

      const sortTimes = (a: any, b: any) => {
        if (a.time < b. time) return 1;
        else return -1;
      }

      res = res.sort(sortTimes)
      setResult(res);
    }
  }, [refetch]);


  return (
    <Box>
      <Typography variant="h4"> History </Typography>
      <br></br>
      <Box>
        <Stack direction="row"  justifyContent="center" spacing={20} > 
            <Typography variant="h5">CardID</Typography>
            <Typography variant="h5">MachineID</Typography>
            <Typography variant="h5">Date</Typography>
        </Stack>
      </Box>

      {!result ? (
        <CircularProgress />
      ) : (
        result.map((item: any) => (
          <HistoryItem
            key={item?.id ?? ""}
            refetch={handleRefetch}
            date={item?.time ?? ""}
            cardId={item?.card_id ?? ""}
            machineId={item?.machine_id ?? ""}
          />
        ))
      )}
    </Box>
  );
};
