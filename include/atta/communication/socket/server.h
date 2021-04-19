//--------------------------------------------------
// Atta Project
// server.h
// Date: 2021-04-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMMUNICATION_SOCKET_SERVER_H
#define ATTA_COMMUNICATION_SOCKET_SERVER_H
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>

namespace atta
{
	class Server
	{
		public:
			Server(unsigned maxClients=10, unsigned port=4114);
			~Server();

			void loop();

		private:
			void checkMasterSocket();
			void checkClientSockets();

			unsigned _maxClients;
			unsigned _port;
			bool _shouldFinish;

			// Server info
			int _serverFd;
			struct sockaddr_in _address;
			fd_set _fdSet;// File descriptor set

			// Clients info
			std::vector<int> _clientSockets;// Client sockets file descriptors
	};
}

#endif// ATTA_COMMUNICATION_SOCKET_SERVER_H
