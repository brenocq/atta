//--------------------------------------------------
// Atta Project
// server.cpp
// Date: 2021-04-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/communication/socket/server.h>
#include <atta/helpers/log.h>

namespace atta
{
	Server::Server(CreateInfo info):
		_maxClients(info.maxClients), _port(info.port), _domain(info.domain), _protocol(info.protocol),
		_shouldFinish(false), _createClientThreads(info.createClientThreads), _clientReadCallback(info.clientReadCallback)
	{
		_clientSockets.resize(_maxClients);

		int domain, type;

		// Select protocol family (communication domain)
		if(_domain == IPV4)
			domain = AF_INET;
		else if(_domain == IPV6)
			domain = AF_INET6;
	
		// Select protocol family type
		if(_protocol == TCP)
			type = SOCK_STREAM;
		else if(_protocol == UDP)
			type = SOCK_DGRAM;

		_serverFd = socket(domain, type, 0);
		if(_serverFd == 0)
		{
			Log::error("Server", "Failed to create socket!");
			exit(1);
		}

		// Allow address reuse (good for multiple connections)
		int opt = 1;
		if(setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		{ 
			Log::error("Server", "Failed to set socket options!");
			exit(1); 
		}

		// Populate socket address
		_address.sin_family = domain;
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);

		// Bind the socket to port
		if(bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address))<0)
		{
			Log::error("Server", "Failed to bind socket!");
			exit(1);
		}
		Log::info("Server", "Listening on port $0", _port);

		// Set socket fd as a passive and specify maximum number of pending connections
		if(listen(_serverFd, 3) < 0)
		{
			Log::error("Server", "Failed to set listen!");
			exit(1);
		}

		createMainThread();
	}

	Server::~Server()
	{
		_shouldFinish = true;
		for(std::thread &t : _threads)
			t.join();
	}

	void Server::createMainThread()
	{
		_threads.push_back(std::thread([&](){mainThreadLoop();}));
	}

	void Server::mainThreadLoop()
	{
		Log::debug("Server", "Main Thread Loop");
		while(!_shouldFinish)
		{
			// Initialize file descriptor set with all bits as zero
			FD_ZERO(&_fdSet); 

			//---------- Add sockets to set ----------//
			// Add master socket to set (enable bit)
			FD_SET(_serverFd, &_fdSet); 
			
			// Maximum file descriptor number (used when searching for client sockets to write/read)
			int maxFd = _serverFd;

			// Add client file descriptors to set
			for(ClientInfo ci : _clientSockets) 
			{ 
				int fd = ci.fd;
				// If valid file descriptor, then add it to read list 
				if(fd > 0) 
					FD_SET(fd, &_fdSet); 
					
				// Highest file descriptor number, required for the select function 
				if(fd > maxFd) 
					maxFd = fd; 
			} 

			//---------- Wait ----------//
			// Wait for a read activity on one of the sockets
			// Timeout of 100ms (needed to step the thread if atta is closed)
			struct timeval timeout = {
				.tv_sec = 0,
				.tv_usec = 100000
			};

			int numReady = 0;// Number of file descriptors ready to read
			if((numReady=select(maxFd+1, &_fdSet, NULL, NULL, &timeout))<0)
			{
				Log::error("Server", "Failed to select file descriptor");
				exit(1);
			}

			//---------- Check sockets ----------//
			if(numReady>0)
			{
				checkMasterSocket();
				if(!_createClientThreads)
					checkClientSockets();
			}
		}
	}

	void Server::checkMasterSocket()
	{
		// Check if master file descriptor is ready to read (clients connected or disconnected)
		if(FD_ISSET(_serverFd, &_fdSet)) 
		{ 
			if(_protocol == TCP)
			{
				int newSocketFd;
				std::string newSocketIp;
				unsigned newSocketPort;

				struct sockaddr_in clientAddress;
				int addrLen = sizeof(clientAddress);
				// Extract first connectin request from the queue of pending connections
				if((newSocketFd = accept(_serverFd, (struct sockaddr *)&clientAddress, (socklen_t*)&addrLen))<0) 
				{ 
					Log::error("Server", "Failed to accept connection"); 
					exit(1); 
				}
				newSocketIp = std::string(inet_ntoa(clientAddress.sin_addr));
				newSocketPort = ntohs(clientAddress.sin_port);

				Log::info("Server", "New client connection -> fd:$0 ip:$1:$2", 
						newSocketFd, newSocketIp, newSocketPort);
					
				// Add new socket to vector of sockets 
				int newSocketId = -1;
				for(unsigned i=0; i<_clientSockets.size(); i++) 
				{ 
					// If position is empty or same client
					if(_clientSockets[i].fd == -1 || _clientSockets[i].ip == newSocketIp) 
					{ 
						_clientSockets[i].fd = newSocketFd; 
						_clientSockets[i].ip = newSocketIp; 
						_clientSockets[i].port = newSocketPort; 
						newSocketId = i;
						Log::info("Server", "Adding to list of client sockets at $0", i); 
						break; 
					} 
				} 

				if(newSocketId == -1)
					Log::warning("Server", "Could not accept new client, max number of clients reached"); 
				else
				{
					// Send connection message
					std::string connectionMessage = "Hi from the server! You are the client "+std::to_string(newSocketId); 
					if(send(newSocketFd, connectionMessage.c_str(), connectionMessage.size(), 0) != (int)connectionMessage.size())
						Log::error("Server", "Failed to send welcome message"); 
				}
			}
		} 
	}

	void Server::checkClientSockets()
	{
		char buffer[1024];
		int valread;

		// Check if data was received from clients
		for(size_t i=0;i<_clientSockets.size();i++) 
		{ 
			int fd = _clientSockets[i].fd;
			// Check if there is something to read from this client
			if(FD_ISSET(fd, &_fdSet)) 
			{ 
				// Check if it was for closing, and also read the incoming message 
				if((valread = read(fd, buffer, 1024)) <= 0)
				{ 
					if(valread != 0)
						Log::warning("Server", "Error when reading: $0", valread);
					if(valread == 0)
					{
						// Client disconnected, get his details and print 
						int addrLen = sizeof(_address);
						getpeername(fd, (struct sockaddr*)&_address, (socklen_t*)&addrLen); 
						Log::info("Server", "Client disconnected -> fd:$0 ip:$1:$2", fd, 
								std::string(inet_ntoa(_address.sin_addr)),
								std::to_string(ntohs(_address.sin_port))
								);
							
						// Close the socket and mark as 0 in list for reuse 
						close(fd); 
						_clientSockets[i].fd = -1;
					}
				} 
				else
				{ 
					if(_clientReadCallback)
						_clientReadCallback(i, std::vector<uint8_t>(buffer, buffer+valread));
					//Log::info("Server", "Received from client -> fd:$0 size:$1 buffer:$2", fd, valread, buffer);
				} 
			} 
		} 
	}
}
