import { initializeApp } from "firebase/app";
import { getFirestore, collection } from "firebase/firestore";
import { getAuth } from "firebase/auth";
const firebaseConfig = {
  apiKey: "AIzaSyDVqDgltLIhBE6_KHj6jp337cB84uQmmbM",
  authDomain: "safetymanager-52fc3.firebaseapp.com",
  databaseURL:
    "https://safetymanager-52fc3-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "safetymanager-52fc3",
  storageBucket: "safetymanager-52fc3.appspot.com",
  messagingSenderId: "227208185310",
  appId: "1:227208185310:web:ba80848a3df49296e7094f",
  measurementId: "G-TVRSW0GTSF",
};

// Use this to initialize the firebase App
initializeApp(firebaseConfig);

// Use these for db & auth
export const db = getFirestore();
const machineDBRef = collection(db, "Machines");
const studentsDBRef = collection(db, "Students");
const registrationsDBRef = collection(db, "Registration");
const historyDBRef = collection(db, "History");


const auth = getAuth();

export { machineDBRef, registrationsDBRef, historyDBRef, studentsDBRef, auth };
