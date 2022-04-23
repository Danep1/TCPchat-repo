#include <iostream>

#include <boost/asio.hpp>
#include "../Chat.hpp"

void run_client_chat(const std::string & raw_ip_address, int port)
{
	try
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(raw_ip_address), port);

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

		socket.connect(endpoint);

		Chat(socket).run();
	}
	catch (const boost::system::system_error& e)
	{
		std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		exit(e.code().value());
	}
	catch (...)
	{
		std::cerr << "Error occured! Unknown error!" << std::endl;
		terminate();
	}
}

// parsing arguments from cmd
std::string parse_raw_ip_address(int argc, char* argv[])
{
	if (argc == 2)
	{
		return std::string(argv[1]);
	}
	if (argc > 2)
	{
		std::cerr << "Too many parameters: use client <ip address>" << std::endl;
		exit(EXIT_FAILURE);
	}
	return "127.0.0.1";
}

int main(int argc, char* argv[])
{
	system("chcp 1251");

	std::cout << "Client launched" << std::endl;

	const int port = 15150;

	run_client_chat(parse_raw_ip_address(argc, argv), port);

	return EXIT_SUCCESS;
}