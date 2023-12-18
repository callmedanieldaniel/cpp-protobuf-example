const WebSocket = require("ws");
const crypto = require("crypto");

// Create a WebSocket server on port 3000
const wss = new WebSocket.Server({ port: 3000 });
const BLOCK_SIZE_MB = 100;
const binaryData = crypto.randomBytes(BLOCK_SIZE_MB * 1024 * 1024);

// When a client connects
wss.on("connection", (ws) => {
  console.log("Client connected");
  let cnt = 0;
  const sendData = () => {
    const timestamp = Buffer.alloc(8);
    timestamp.writeBigInt64LE(BigInt(Date.now()));
    const dataToSend = Buffer.concat([timestamp, binaryData]);
    const before = Date.now();
    ws.send(dataToSend, { binary: true }, (error) => {
      const diff = Date.now() - before;
      console.log("sendFinish: ", cnt, diff, (BLOCK_SIZE_MB / diff) * 1000);
      cnt++;
      if (error) {
        console.error("WebSocket send error:", error);
      } else {
        // Send additional data when the last data has been sent
        sendData();
      }
    });
  };
  sendData();
  // Close the connection after sending the data (optional)
  // ws.close();
});

console.log("WebSocket server is listening on port 3000");
