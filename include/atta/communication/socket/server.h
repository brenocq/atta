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
#include <thread>
#include <functional>

namespace atta
{
	class Server
	{
		public:
			enum Domain {
				IPV4,
				IPV6
			};
			enum Protocol {
				TCP,
				UDP
			};
			struct CreateInfo {
				unsigned maxClients=10;
				unsigned port=4114;
				Domain domain = IPV4;
				Protocol protocol = TCP;
				bool createClientThreads = false;
			};

			Server(CreateInfo info);
			~Server();

			//std::function<void(int clientId, std::vector<uint8_t> buffer)> received()

		private:
			void createMainThread();
			void mainThreadLoop();

			void checkMasterSocket();
			void checkClientSockets();

			unsigned _maxClients;
			unsigned _port;
			Domain _domain;
			Protocol _protocol;

			// Main thread + optional client threads
			std::vector<std::thread> _threads;
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
