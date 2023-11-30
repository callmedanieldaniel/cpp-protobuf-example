const protobuf = require("protobufjs");
const axios = require("axios");
const serverUrl = "http://127.0.0.1:8081/"; // Replace with your actual server URL
const path = require('path');

// Load the compiled Protocol Buffers schema
// const sendRoot = protobuf.loadSync("./message.proto");
// const MyMessage = sendRoot.lookupType("MyMessage");
// const myMessage = MyMessage.create({ data: "Hello from Node.js!" });
// const binaryData = MyMessage.encode(myMessage).finish();

const protoPath = path.join(__dirname, 'proto/news.proto');
const receiveRoot = protobuf.loadSync(protoPath);
const receivedMessage = receiveRoot.lookupType("NewsStruct");


// Send a POST request to the server with the binary data
axios.get(serverUrl, {
  headers: {
    "Content-Type": "application/octet-stream",
  },
})
  .then(response => {
    const decodedMessage = receivedMessage.decode(response.data);
    console.log("receivedMessage:", decodedMessage.data);
  })
  .catch(error => {
    console.error("Error:", error.message);
  });