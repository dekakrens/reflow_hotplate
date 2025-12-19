import { useEffect, useRef, useState } from "react";
import type { WSPayload } from "../types/websocket";

export const useWebSocket = (url: string) => {
  const wsRef = useRef<WebSocket | null>(null);
  const [data, setData] = useState<WSPayload | null>(null);
  const [connected, setConnected] = useState<boolean>(false);

  useEffect(() => {
    const ws = new WebSocket(url);
    wsRef.current = ws;

    ws.onopen = () => {
      setConnected(true);
    };

    ws.onmessage = (event) => {
      try {
        const payload = JSON.parse(event.data);
        setData(payload);
      } catch (error) {
        console.log("Invalid payload", error);
      }
    };

    ws.onerror = (err) => {
      console.error("WebSocket error", err);
    };

    ws.onclose = () => {
      setConnected(false);
      console.log("WebSocket disconnected");
    };

    return () => {
      ws.close();
    };
  }, [url]);

  const send = (payload: WSPayload) => {
    if (wsRef.current?.readyState === WebSocket.OPEN) {
      wsRef.current.send(JSON.stringify(payload));
    }
  };

  return { data, connected, send };
};
