import { addDoc, deleteDoc, doc, getDocs, setDoc } from "firebase/firestore";
import {
  db,
  machineDBRef,
  registrationsDBRef,
  studentsDBRef,
  historyDBRef,
} from "../firebase";

const getItemsDocs = async (DBRef) => {
  let items = [];
  try {
    const snapshot = await getDocs(DBRef);
    snapshot.docs.forEach((doc) => {
      items.push({ ...doc.data(), id: doc.id });
    });
  } catch {
    console.error("Cant Fetch from DB");
  }

  return items;
};

const addItemDoc = async (DBRef, newDoc) => {
  try {
    await addDoc(DBRef, newDoc);
  } catch {
    console.error("Failed to add doc");
  }
};

const addItemDocWithID = async (DBRefName, newDocId, newDocValue) => {
  try {
    const docRef = doc(db, DBRefName, newDocId);
    await setDoc(docRef, newDocValue);
  } catch {
    console.error("Failed to add doc");
  }
};

const deleteItemDoc = async (DBRefName, docID) => {
  try {
    const docRef = doc(db, DBRefName, docID);
    await deleteDoc(docRef);
  } catch {
    console.error("Failed to add doc");
  }
};

export const getMachinesDocs = () => getItemsDocs(machineDBRef);
export const getRegistrationDocs = () => getItemsDocs(registrationsDBRef);
export const getStudentsDocs = () => getItemsDocs(studentsDBRef);
export const getHistoryDocs = () => getItemsDocs(historyDBRef);

//export const addMachineDoc = (newDoc) => addItemDoc(machineDBRef, newDoc);
export const addMachineDoc = (newDocId, newDocValue) =>
  addItemDocWithID("Machines", newDocId, newDocValue);
export const addRegistrationDoc = (newDoc) =>  addItemDoc(registrationsDBRef, newDoc);
export const addStudentDoc = (newDocId, newDocValue) =>
  addItemDocWithID("Students", newDocId, newDocValue);
export const addHistoryDoc = (newDoc) => addItemDoc(historyDBRef, newDoc);


export const deleteMachineDoc = (docId) => deleteItemDoc("Machines", docId);
export const deleteRegistratioDoc = (docId) =>
  deleteItemDoc("Registration", docId);
export const deleteStudentDoc = (docId) => deleteItemDoc("Students", docId);
