#include <iostream>

#include <boost/asio.hpp>
#include "../Chat.hpp"

void write_data(boost::asio::ip::tcp::socket& socket)
{
	std::string data;

	std::getline(std::cin, data);

	boost::asio::write(socket, boost::asio::buffer(data));
}

int main(int argc, char* argv[])
{
	system("chcp 1251");

	std::cout << "Client launched\n";

	std::string raw_ip_address = "127.0.0.1";

	// parsing arguments from cmd
	if (argc == 2)
	{
		raw_ip_address = std::string(argv[1]);
	}
	else if (argc > 2)
	{
		std::cerr << "Too many parametrs" << std::endl;
	}

	constexpr auto port = 15150U;

	try
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(raw_ip_address), port);

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

		socket.connect(endpoint);

		Chat(socket).run();
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}

	return EXIT_SUCCESS;
}