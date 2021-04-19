//--------------------------------------------------
// Atta Project
// client.h
// Date: 2021-04-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMMUNICATION_SOCKET_CLIENT_H
#define ATTA_COMMUNICATION_SOCKET_CLIENT_H
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

namespace atta
{
	class Client
	{
		public:
			Client(std::string serverIp="127.0.0.1", unsigned serverPort=1441);
			~Client();

			void connectToServer();
			void sendToServer(std::string data);
			std::string readFromServer(unsigned maxSize=1024);

			bool getConnected() const { return _connected; }

		private:
			bool _connected;
			unsigned _clientId;

			// Server info
			std::string _serverIp;
			unsigned _serverPort;

			// Socket connection
			unsigned _clientFd;
			struct sockaddr_in _serverAddr;
	};
}

#endif// ATTA_COMMUNICATION_SOCKET_CLIENT_H
