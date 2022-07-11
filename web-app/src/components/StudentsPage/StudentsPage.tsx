import { Box, CircularProgress, Typography } from "@mui/material";
import { useEffect, useState } from "react";
import { getStudentsDocs } from "../../firebase/dbRef";
import { StudentsItem } from "./StudentItem";

export const StudentsPage = () => {
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
      const res = await getStudentsDocs();
      if (!active) {
        return;
      }
      setResult(res);
    }
  }, [refetch]);

  return (
    <Box>
      <Typography variant="h4"> Students </Typography>
      {!result ? (
        <CircularProgress />
      ) : (
        result.map((studentItem: any) => (
          <StudentsItem
            key={studentItem?.id ?? ""}
            refetch={handleRefetch}
            student={studentItem?.name ?? ""}
            perm_level={studentItem?.perm_level ?? ""}
            cardId={studentItem?.id ?? ""}
          />
        ))
      )}
    </Box>
  );
};
