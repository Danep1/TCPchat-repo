#include <iostream>

#include <boost/asio.hpp>
#include "../Chat.hpp"

class ClientChat
{
public:
	explicit ClientChat(const std::string & raw_ip_address, int port)
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
};

int main(int argc, char* argv[])
{
	system("chcp 1251");

	std::cout << "Client launched\n";

	const int port = 15150;

	std::string raw_ip_address = "127.0.0.1";

	// parsing arguments from cmd
	if (argc == 2)
	{
		raw_ip_address = std::string(argv[1]);
	}
	else if (argc > 2)
	{
		std::cerr << "Too many parameters" << std::endl;
	}

	ClientChat(raw_ip_address, port);

	return EXIT_SUCCESS;
}