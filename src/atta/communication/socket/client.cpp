//--------------------------------------------------
// Atta Project
// client.cpp
// Date: 2021-04-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/communication/socket/client.h>
#include <atta/helpers/log.h>

namespace atta
{
	Client::Client(std::string serverIp, unsigned serverPort):
		_connected(false),
		_serverIp(serverIp), _serverPort(serverPort)
	{

	}

	Client::~Client()
	{

	}

	void Client::connectToServer()
	{
		// Create TCP socket(SOCK_STREAM) using ipv4(AF_INET)
		if((_clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			Log::error("Client", "Socket creation error");
			exit(1);
		}

		// Populate server address
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_port = htons(_serverPort);

		// Convert IPv4 and IPv6 addresses from text to binary form
		if(inet_pton(AF_INET, _serverIp.c_str(), &_serverAddr.sin_addr)<=0)
		{
			Log::error("Client", "Invalid address / Address not supported.");
			exit(1);
		}

		// Connect to server
		if(connect(_clientFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
		{
			Log::warning("Client", "Failed to connect to the server!");
			_connected = false;
		}
		else
		{
			Log::success("Client", "Connected to the server");
			_connected = true;
		}
	}

	void Client::sendToServer(std::string data)
	{
		send(_clientFd, data.c_str(), data.size(), 0); 
	}

	std::string Client::readFromServer(unsigned maxSize)
	{
		char buffer[maxSize];
		int size = read(_clientFd, buffer, maxSize);
		buffer[size] = '\0';
		
		if(size <= 0)
		{
			_connected = false;
			Log::warning("Client", "Server offline!");
		}

		return std::string(buffer);
	}
}
