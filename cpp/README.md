# Protocol Buffers C++ Example

This is a simple example demonstrating the use of Protocol Buffers in C++.

## Prerequisites

- [Protocol Buffers Compiler (`protoc`)](https://developers.google.com/protocol-buffers) installed.
- C++ compiler (e.g., g++).

## Compilation

```bash
protoc --cpp_out=. news.proto
g++ -std=c++11 -o httpserver httpserver.cpp ./proto/news.pb.cc `pkg-config --cflags --libs protobuf`
```

## websocket Compilation

g++ -std=c++11 -o websocket_server continious_send_server.cpp -lwebsocketpp -lpthread

protoc example.proto -o example.desc

## 更新 cpp 代码并打包
cd cpp/socket/ && .. rm -rf build/ && mkdir build && cd build && cmake .. && make && ./server
