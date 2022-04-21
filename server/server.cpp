#include <iostream>

#include <boost/asio.hpp>
#include "../Chat.hpp"

std::string read_data(boost::asio::ip::tcp::socket& socket)
{
	const std::size_t length = 10;
	char buffer[length];
	boost::asio::read(socket, boost::asio::buffer(buffer, length));
	return std::string(buffer, length);
}

std::string read_data_until(boost::asio::ip::tcp::socket& socket)
{
	boost::asio::streambuf buffer;

	boost::asio::read_until(socket, buffer, '!');

	std::string message;

	// Because buffer 'buf' may contain some other data
	// after '\n' symbol, we have to parse the buffer and
	// extract only symbols before the delimiter.
	std::istream input_stream(&buffer);
	std::getline(input_stream, message, '!');

	return message;
}

int main(int argc, char* argv[])
{
	system("chcp 1251");

	std::cout << "Server launched\n";

	constexpr auto max_clients = 10ULL;

	constexpr auto port = 15150U;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	boost::asio::io_service io_service;

	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(max_clients);

		boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		Chat(socket).run();
	}
	catch (boost::system::system_error& e)
	{
		std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}
	catch (...)
	{
		std::cerr << "Error occured! Unknown error!" << std::endl;
		terminate();
	}
	return EXIT_SUCCESS;
}