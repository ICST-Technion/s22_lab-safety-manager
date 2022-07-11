import React, { useMemo, useState } from "react";
import { GoogleAuthProvider, signInWithPopup } from "firebase/auth";
import { auth } from "../firebase";
import { LoginPage } from "./LoginPage";
import { UserApp } from "./UserApp";
import { getAppTheme } from "../theme";
import { ThemeProvider } from "@mui/material";
import CssBaseline from '@mui/material/CssBaseline';

export const App = () => {
  const [user, setUser] = useState<any>();

  const theme = useMemo(() => getAppTheme(), []);

  const handleSignIn = async () => {
    const googleProvier = new GoogleAuthProvider();
    const signRes = await signInWithPopup(auth, googleProvier);
    setUser(signRes);
  };

  return (
    <>
      <ThemeProvider theme={theme}>
        <CssBaseline />
        {!!user ? (
          <UserApp user={user} />
        ) : (
          <LoginPage loginHandler={handleSignIn} />
        )}
      </ThemeProvider>
    </>
  );
};
