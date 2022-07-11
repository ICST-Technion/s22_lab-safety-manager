import { Box, CircularProgress, Typography } from "@mui/material";
import { useEffect, useState } from "react";
import { getRegistrationDocs } from "../../firebase/dbRef";
import { RegistrationItem } from "./RegistrationItem";

export const RegistrationPage = () => {
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
      const res = await getRegistrationDocs();
      if (!active) {
        return;
      }
      setResult(res);
    }
  }, [refetch]);

  return (
    <Box>
      <Typography variant="h4"> Registration </Typography>
      {!result ? (
        <CircularProgress />
      ) : (
        result.map((item: any) => (
          <RegistrationItem
            refetch={handleRefetch}
            key={item?.id ?? ""}
            regDocId={item?.id ?? ""}
            date={item?.time ?? ""}
            cardId={item.card_id ?? ""}
          />
        ))
      )}
    </Box>
  );
};
