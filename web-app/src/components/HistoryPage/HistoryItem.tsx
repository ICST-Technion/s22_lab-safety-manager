import {
    Typography,
    Box,
  } from "@mui/material";
  import React, { useState } from "react";
  
export interface HistoryItemProps {
    cardId: string;
    machineId: string;
    date: string;
    refetch: () => void;
}

export const HistoryItem: React.FC<HistoryItemProps> = ({
    cardId,
    machineId,
    date,
    refetch,
}) => {


    return (
        <Box
            sx={{
            display: "flex",
            flexDirection: "row",
            justifyContent: "space-around",
            alignItems: "center",
            margin: "8px",
            width: 700,
            height: 70,
            border: "1px solid #0277bd",
            borderRadius: "20px",
            backgroundColor: "#e3f2fd",
            "&:hover": {
                backgroundColor: "#80d8ff",
                opacity: [0.9, 0.8, 0.7],
            },
            }}
        >
            <Typography> {cardId} </Typography>
            <Typography> {machineId} </Typography>
            <Typography> {date} </Typography>
        </Box>
    );
};
