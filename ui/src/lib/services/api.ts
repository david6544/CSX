import { decode, encode } from "@msgpack/msgpack";
import type { Order } from "$lib/types/stock";
import { browser } from "$app/environment";

// Message type constants
const MessageTypes = {
  ORDER: 0x01,
  PRICE_REQUEST: 0x02,
  MARKET_DATA: 0x03,
};

let webSocket: WebSocket | null = null;

// Helper function to create a message with binary header
function createMessage(type: number, data: any): Uint8Array {
    // Encode the payload with MessagePack
    const payload = encode(data);
    console.log(payload)
    
    // Create message with just a 1-byte header + payload
    const message = new Uint8Array(1 + payload.byteLength);
    message[0] = type;  // Message type byte
    message.set(payload, 1);  // Payload starts at index 1
    
    console.log(message);
    return message;
}

export function sendOrder(order: Order) {
  const ws = webSocket || initWebSocket();

  if (!ws || ws.readyState !== WebSocket.OPEN) {
      console.error("Websocket not connected");
      return false;
  }

  const orderArray = [
      Math.floor(order.price),
      Math.floor(order.quantity)
  ];
  
  // Create message with ORDER message type and order data
  const message = createMessage(MessageTypes.ORDER, orderArray);
  
  // Send the binary message
  ws.send(message);
  return true;
}

export function requestPrice(symbol: string) {
  const ws = webSocket || initWebSocket();

  if (!ws || ws.readyState !== WebSocket.OPEN) {
      console.error("Websocket not connected");
      return false;
  }
  
  // Create message with PRICE_REQUEST type and symbol data
  const message = createMessage(MessageTypes.PRICE_REQUEST, { symbol });
  
  // Send the binary message
  ws.send(message);
  return true;
}

export function initWebSocket() {
  if (browser && !webSocket) {
    webSocket = new WebSocket('ws://127.0.0.1:8081');

    webSocket.onmessage = (event: MessageEvent<any>) => {
      if (event.data instanceof Blob) {
        event.data.arrayBuffer().then(buffer => {
          const order = decode(new Uint8Array(buffer));
          console.log('Recieved order: ', order);
        });
      } else {
        console.log('Recieved Text message: ', event.data);
      }
    }
    
    webSocket.onopen = () => {
      console.log("WebSocket connected");
    };
    
    webSocket.onerror = (error) => {
      console.error("WebSocket error:", error);
    };
    
    webSocket.onclose = () => {
      webSocket = null;
      console.log("WebSocket closed");
    };
  }

  return webSocket;
}

