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
	Server::Server(unsigned maxClients, unsigned port):
		_maxClients(maxClients), _port(port), _shouldFinish(false)
	{
		_clientSockets.resize(_maxClients);

		// Create TCP socket(SOCK_STREAM) using ipv4(AF_INET)
		if((_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);

		// Bind the socket to port
		if(bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address))<0)
		{
			Log::error("Server", "Failed to bind socket!");
			exit(1);
		}
		Log::info("Server", "Listening on port $0", _port);

		// Specify maximum number of pending connections
		if(listen(_serverFd, 3) < 0)
		{
			Log::error("Server", "Failed to set listen!");
			exit(1);
		}
	}

	Server::~Server()
	{

	}

	void Server::loop()
	{
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
			for(int fd : _clientSockets) 
			{ 
				// If valid file descriptor, then add it to read list 
				if(fd > 0) 
					FD_SET(fd, &_fdSet); 
					
				// Highest file descriptor number, need it for the select function 
				if(fd > maxFd) 
					maxFd = fd; 
			} 

			//---------- Wait ----------//
			// Wait for an activity on one of the sockets (read)
			// Timeout is NULL, so wait indefinitely 
			if(select(maxFd+1, &_fdSet, NULL, NULL, NULL)<0)
			{
				Log::error("Server", "Failed to select socket");
				exit(1);
			}

			//---------- Check sockets ----------//
			checkMasterSocket();
			checkClientSockets();
		}
	}

	void Server::checkMasterSocket()
	{
		if(FD_ISSET(_serverFd, &_fdSet)) 
		{ 
			int newSocketFd;
			int addrLen = sizeof(_address);
			if((newSocketFd = accept(_serverFd, (struct sockaddr *)&_address, (socklen_t*)&addrLen))<0) 
			{ 
				Log::error("Server", "Failed to accept connection"); 
				exit(1); 
			} 
			Log::info("Server", "New client connection -> fd:$0 ip:$1:$2", 
					newSocketFd,
					std::string(inet_ntoa(_address.sin_addr)),
					std::to_string(ntohs(_address.sin_port))
					);
				
			unsigned newSocketId = 0;
			// Add new socket to vector of sockets 
			for(unsigned i=0; i<_clientSockets.size(); i++) 
			{ 
				// If position is empty 
				if(_clientSockets[i] == 0) 
				{ 
					_clientSockets[i] = newSocketFd; 
					newSocketId = i;
					Log::info("Server", "Adding to list of client sockets at $0", i); 
					break; 
				} 
			} 

			// Send connection message
			std::string connectionMessage = "{\"id\":"+std::to_string(newSocketId)+"}"; 
			if(send(newSocketFd, connectionMessage.c_str(), connectionMessage.size(), 0) != (int)connectionMessage.size())
			{ 
				Log::error("Server", "Failed to send welcome message"); 
			} 
		} 
	}

	void Server::checkClientSockets()
	{
		char buffer[1024];
		int valread;

		// Check IO operation on client sockets
		for(unsigned i=0; i<(int)_clientSockets.size(); i++) 
		{ 
			int fd = _clientSockets[i]; 
				
			if(FD_ISSET(fd, &_fdSet)) 
			{ 
				// Check if it was for closing, and also read the incoming message 
				if((valread = read(fd, buffer, 1024)) <= 0)
				{ 
					// Somebody disconnected, get his details and print 
					int addrLen = sizeof(_address);
					getpeername(fd, (struct sockaddr*)&_address, (socklen_t*)&addrLen); 
					Log::info("Server", "Client disconnected -> fd:$0 ip:$1:$2", fd, 
							std::string(inet_ntoa(_address.sin_addr)),
							std::to_string(ntohs(_address.sin_port))
							);
						
					// Close the socket and mark as 0 in list for reuse 
					close(fd); 
					_clientSockets[i] = 0; 
				} 
				else
				{ 
					buffer[valread] = '\0';

					// Update client data
					Log::info("Server", "Received from client -> fd:$0 size:$1 buffer:$2", i, valread, std::string(buffer));
					//_clientInfos[i] = json::parse(std::string(buffer));

					// Merge all clients data to one json
					//json gameState;
					//for(int j=0; j<_maxClients; j++)
					//{
					//	if(_clientSockets[j]!=0)
					//	{
					//		gameState+=_clientInfos[j];
					//	}
					//}

					// Responds with data from all active clients
					//std::string message = gameState.dump();
					//send(sd, message.c_str(), message.size(), 0); 
				} 
			} 
		} 
	}
}
