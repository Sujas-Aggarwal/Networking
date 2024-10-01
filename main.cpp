#include<iostream>
#define ASIO_STANDALONE
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>
using namespace std;
static vector<char> vBuffer(1024*8); // basically 1 KiB (78 of these are enough to send people on moon)

void DataStreaming(asio::ip::tcp::socket& socket) { // for continuous data streaming between client and socket in a non blocking way
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](error_code ec, size_t length)
		{
			if (!ec)
			{
				cout << "\n\n Length of Data Read :" << length << " bytes\n\n";
				for (size_t i = 0; i < length;i++) {
					cout << vBuffer[i];
				}
				DataStreaming(socket);
			}
		}
	);
}
int main() {
	asio::error_code ec;
	asio::io_context context;
	asio::io_context::work idleWork(context); //to ensure context is finished Executing before running in thread
	thread threadContext = thread([&]() {context.run();});
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
		DataStreaming(socket); // starting to read data as soon as the connection is established, tho no data is going to be fetched without any request
		string getRequest =
			"GET /index.html HTTP/1.1\r\n"
			"HOST: sujas.tech\r\n"
			"Connection: close\r\n\r\n";
		
		socket.write_some(asio::buffer(getRequest.data(), getRequest.size()), ec);

		if (!ec) {
			cout << "Request Sent Successfully" << endl;
		}
		else {
			cout << "Failed to Send Request:\n" << ec.message() << endl;
		}
	}
	using namespace chrono_literals;
	this_thread::sleep_for(10000ms);
	context.stop();
	if (threadContext.joinable()) threadContext.join();

	return 0;
}