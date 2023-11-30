const WebSocket = require("ws");

const ws = new WebSocket("ws://localhost:9002");

ws.on("open", () => {
  console.log("WebSocket connection opened.");
  let n = 0;
  setInterval(() => {
    ws.send("node--" + n);
    n++;
  }, 1000);

  let sizeSum = 0;
  let lastTs = Date.now();
  // Listen for incoming data from the server
  ws.on("message", (data) => {
    const now = Date.now();
    const MB = 1 * 1024 * 1024;
    const size = data.length / MB;
    const bt = data.slice(0, 8);
    const decodedNumberBigInt = bt.readBigInt64LE(0);
    const sendTs = Number(decodedNumberBigInt.toString());
    const diff = now - sendTs / 1000;
    sizeSum += size;

    // console.log(
    //   `BandWidth: ${(size / diff) * 1000}MB/s,time:${diff}, size:${size}MB`
    // );
  });

  setInterval(() => {
    const now = Date.now();
    const bw = (sizeSum / (now - lastTs)) * 1000;
    lastTs = now;
    sizeSum = 0;
    console.log("--- all messagesize bandwidth ", bw);
  }, 1000);
});

ws.on("error", (error) => {
  console.error("WebSocket error:", error);
});

ws.on("close", (code, reason) => {
  console.log(`WebSocket connection closed, code: ${code}, reason: ${reason}`);
});
