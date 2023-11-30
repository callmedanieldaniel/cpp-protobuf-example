#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <vector>
#include <chrono>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Struct to represent binary data with timestamp
struct BinaryDataWithTimestamp
{
    std::chrono::system_clock::time_point timestamp;
    std::vector<char> binaryData;
};

BinaryDataWithTimestamp sendData;

std::vector<char> generate_random_data(std::size_t size)
{
    std::vector<char> data(size);

    // 使用伪随机数生成器填充数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (std::size_t i = 0; i < size; ++i)
    {
        data[i] = static_cast<char>(dis(gen));
    }

    return data;
}

std::vector<char> binaryData = generate_random_data(40 * 1024 * 1024);

// Define a callback to handle incoming messages
void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    auto now = std::chrono::system_clock::now();
    // 转换为 time_t（自 1970 年 1 月 1 日以来的秒数）
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    sendData.binaryData = binaryData;
    sendData.timestamp = std::chrono::system_clock::now();

    size_t timestampSize = sizeof(sendData.timestamp);

    std::cout << "Time:" << currentTime;
    std::cout << "Size:" << timestampSize;
    std::cout << "on_message " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    std::vector<char> sendBuffer(timestampSize + sendData.binaryData.size());
    memcpy(sendBuffer.data(), &sendData.timestamp, timestampSize);
    memcpy(sendBuffer.data() + timestampSize, sendData.binaryData.data(), sendData.binaryData.size());

    try
    {
        s->send(hdl, sendBuffer.data(), sendBuffer.size(), websocketpp::frame::opcode::BINARY);
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

int main()
{
    // Create a server endpoint
    server echo_server;

    try
    {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "other exception" << std::endl;
    }
}