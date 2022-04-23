#include <iostream>

#include <boost/asio.hpp>
#include "../Chat.hpp"

class ServerChat
{
public:
	explicit ServerChat(int port, int max_connections)
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

		boost::asio::io_service io_service;

		try
		{
			boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

			acceptor.bind(endpoint);

			acceptor.listen(max_connections);

			boost::asio::ip::tcp::socket socket(io_service);

			acceptor.accept(socket);

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

	constexpr int port = 15150;

	constexpr int max_connections = 10;

	ServerChat(port, max_connections);
	
	return EXIT_SUCCESS;
}