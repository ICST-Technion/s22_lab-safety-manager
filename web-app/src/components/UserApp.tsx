import { Stack, Typography } from "@mui/material";
import { useState } from "react";
import { Page } from "../types";
import { Header } from "./Header";
import { MachinePages } from "./MachinesPage/MachinePage";
import { RegistrationPage } from "./RegistrationPage/RegistrationPage";
import { StudentsPage } from "./StudentsPage/StudentsPage";
import { HistoryPage } from "./HistoryPage/HistoryPage";

interface UserAppProps {
  user: any;
}

export const UserApp: React.FC<UserAppProps> = ({ user }) => {
  const [page, setPage] = useState<Page>("Home");

  const renderCurrentPage = (currentPage: Page) => {
    switch (currentPage) {
      case "Home":
        return; //<Typography>  </Typography>;
      case "Machines":
        return <MachinePages />;
      case "Students":
        return <StudentsPage />;
      case "Registration":
        return <RegistrationPage />;
      case "History":
        return <HistoryPage />;
      }
  };

  console.log(user);

  return (
    <Stack alignItems="center" spacing={5} sx={{ marginTop: "30px" }}>
      <Header setPage={setPage} user={user} />
      {renderCurrentPage(page)}
    </Stack>
  );
};
