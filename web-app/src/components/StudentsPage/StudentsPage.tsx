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
      let res = await getStudentsDocs();
      if (!active) {
        return;
      }
      const sortName = (a: any, b: any) => {
        if (a.name >= b. name) return 1;
        else return -1;
      }

      res = res.sort(sortName)
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
