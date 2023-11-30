#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>

// Define a WebSocket++ server type
using server_t = websocketpp::server<websocketpp::config::asio>;

// Define a message handler callback
void on_message(server_t* s, websocketpp::connection_hdl hdl, server_t::message_ptr msg) {
    try {
        std::cout << "Received message: " << msg->get_payload() << std::endl;

        // Echo the received message back to the client
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (const websocketpp::lib::error_code& e) {
        std::cout << "Echo failed because: " << e.message() << std::endl;
    }
}

int main() {
    // Create a WebSocket++ server
    server_t server;

    // Set logging level (optional)
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Set up the message handler callback
    server.set_message_handler(bind(&on_message, &server, ::_1, ::_2));

    // Listen on port 9002
    server.listen(9002);

    // Start the server accept loop
    server.start_accept();

    // Start the ASIO io_service run loop
    server.run();

    return 0;
}