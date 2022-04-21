#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>


class Chat
{
private:    
    using string_t = std::string;
    using container_t = std::vector < string_t >;
    using socket_t = boost::asio::ip::tcp::socket;
    using flag_t = std::atomic< bool >;

private:

    socket_t & socket_;
    container_t messages_;
    flag_t exit_flag_;
    string_t user_name_;

private:

public:
    explicit Chat(socket_t & socket) : socket_(socket), exit_flag_(false)
    {
        std::cout << "Enter your name: ";
        std::cin >> user_name_;
        std::cout << "To close chat enter '!exit'" << std::endl;
    }

    void run()
    {
        auto reader = std::thread(&Chat::read, this);

        write();

        //trying cancel read_until, that stucks 
        //but it calls abort()
        socket_.cancel();
        
        reader.join();

        send_message(user_name_ + "left the chat!\n");
    }

private:
    string_t parse_message(const string_t & text) const
    {
        auto t = std::time(nullptr);
        auto timenow = std::tm(*std::localtime(&t));
        std::stringstream format;
        format << '[' << user_name_ << "] (" << std::put_time(&timenow, "%T") << ") : " << text << '\n';
        return format.str();
    }

    void read()
    {
        send_message(user_name_ + " joined the chat\n");

    	boost::asio::streambuf buffer;
        while (!exit_flag_)
        {
            boost::asio::read_until(socket_, buffer, '\n');
            
            if (buffer.size())
            {
                std::string message;
                std::istream input_stream(&buffer);
                std::getline(input_stream, message, '\n');
                std::cout << message << std::endl;
            }
        }
    }

    void send_message(const string_t & text)
    {
        messages_.push_back(text);
        boost::asio::write(socket_, boost::asio::buffer(text));
    }

    void write()
    {
        string_t text;

        while (!exit_flag_)
        {
            std::getline(std::cin, text);

            if (text == "!exit")
                exit_flag_ = true;
            else if (text != "")
                send_message(parse_message(text));
        }
    }
};
