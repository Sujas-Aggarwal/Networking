#include<iostream>
#define ASIO_STANDALONE
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>
using namespace std::chrono_literals;
using namespace std;
int main() {
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 8000);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (!ec) {
		cout << "Connected" << endl;
	}
	else {
		cout << "Failed to Connect to Address:\n" << ec.message() << endl;
	}
	if (socket.is_open()) {
		string getRequest =
			"GET /cpp HTTP/1.1\r\n"
			"HOST: sujas.tech\r\n"
			"Connection: close\r\n\r\n";
		
		socket.write_some(asio::buffer(getRequest.data(), getRequest.size()), ec);

		if (!ec) {
			cout << "Request Sent Successfully" << endl;
		}
		else {
			cout << "Failed to Send Request:\n" << ec.message() << endl;
		}
		this_thread::sleep_for(100ms);
		size_t availableBytes = socket.available();
		if (availableBytes >= 0) {
			cout << "Available Bytes to Read: " << availableBytes << endl;
			vector<char> vBuffer(availableBytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()),ec);
			for (auto c : vBuffer) {
				cout << c;
			}
		}
		else {
			cout << "No Response Recieved" << endl;
		}
	}
	return 0;
}