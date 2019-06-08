#include "net_serial.h"
#include "libretro.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#endif

extern retro_log_printf_t log_cb;
//FILE* fpout;
//FILE* fpin;
NetSerial::NetSerial()
: is_stopped_(true)
, is_server_(false)
, port_(12345)
, hostname_()
, server_fd_(-1)
, sockfd_(-1)
, lastConnectAttempt_(0)
{
//	fpout = fopen("/tmp/serial_out.txt", "w");
//	fpin  = fopen("/tmp/serial_in.txt", "w");
}

NetSerial::~NetSerial()
{
	stop();
}

//int master_txn_cnt = 0;
//int slave_txn_cnt = 0;
bool NetSerial::start(bool is_server, int port, const std::string& hostname)
{
	stop();

	log_cb(RETRO_LOG_INFO, "Starting GameLink nework %s on %s:%d\n",
			is_server ? "server" : "client", hostname.c_str(), port);
	is_server_ = is_server;
	port_ = port;
	hostname_ = hostname;
	is_stopped_ = false;

	return checkAndRestoreConnection(false);
}
void NetSerial::stop()
{
	if (!is_stopped_) {
		log_cb(RETRO_LOG_INFO, "Stoping GameLink nework\n");
		is_stopped_ = true;
		if (sockfd_ >= 0) {
			close(sockfd_);
			sockfd_ = -1;
		}
		if (server_fd_ >= 0) {
			close(server_fd_);
			server_fd_ = -1;
		}
	}
}

bool NetSerial::checkAndRestoreConnection(bool throttle)
{
	if (is_stopped_) {
		return false;
	}
	if (sockfd_ < 0 && throttle) {
		clock_t now = clock();
		// Only attempt to establish the connection every 5 seconds
		if (((now - lastConnectAttempt_) / CLOCKS_PER_SEC) < 5) {
			return false;
		}
	}
	lastConnectAttempt_ = clock();
	if (is_server_) {
		if (!startServerSocket()) {
			return false;
		}
		if (!acceptClient()) {
			return false;
		}
	} else {
		if (!startClientSocket()) {
			return false;
		}
	}
	return true;
}
bool NetSerial::startServerSocket()
{
	int fd;
	struct sockaddr_in server_addr;

	if (server_fd_ < 0) {
		bzero((char *)&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port_);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0) {
			log_cb(RETRO_LOG_ERROR, "Error opening socket: %s\n", strerror(errno));
			return false;
		}

		if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			log_cb(RETRO_LOG_ERROR, "Error on binding: %s\n", strerror(errno));
			close(fd);
			return false;
		}

		if (listen(fd, 1) < 0) {
			log_cb(RETRO_LOG_ERROR, "Error listening: %s\n", strerror(errno));
			close(fd);
			return false;
		}
		server_fd_ = fd;
		log_cb(RETRO_LOG_INFO, "GameLink network server started!\n");
	}

	return true;
}
bool NetSerial::acceptClient()
{
	struct sockaddr_in client_addr;
	struct timeval tv;
	fd_set rfds;

	if (server_fd_ < 0) {
		return false;
	}
	if (sockfd_ < 0) {
		int retval;

		FD_ZERO(&rfds);
		FD_SET(server_fd_, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		if (select(server_fd_ + 1, &rfds, NULL, NULL, &tv) <= 0) {
			return false;
		}

		socklen_t client_len = sizeof(client_addr);
		sockfd_ = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);
		if (sockfd_ < 0) {
			log_cb(RETRO_LOG_ERROR, "Error on accept: %s\n", strerror(errno));
			return false;
		}
		log_cb(RETRO_LOG_INFO, "GameLink network server connected to client!\n");
	}
	return true;
}
bool NetSerial::startClientSocket()
{
	int fd;
	struct sockaddr_in server_addr;

	if (sockfd_ < 0) {
		bzero((char *)&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port_);

		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0) {
			log_cb(RETRO_LOG_ERROR, "Error opening socket: %s\n", strerror(errno));
			return false;
		}

		struct hostent* server_hostname = gethostbyname(hostname_.c_str());
		if (server_hostname == NULL) {
			log_cb(RETRO_LOG_ERROR, "Error, no such host: %s\n", hostname_.c_str());
			close(fd);
			return false;
		}

		bcopy((char*)server_hostname->h_addr, (char*)&server_addr.sin_addr.s_addr, server_hostname->h_length);
		if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
			log_cb(RETRO_LOG_ERROR, "Error connecting to server: %s\n", strerror(errno));
			close(fd);
			return false;
		}
		sockfd_ = fd;
		log_cb(RETRO_LOG_INFO, "GameLink network client connected to server!\n");
	}
	return true;
}

unsigned char NetSerial::send(unsigned char data, bool fastCgb)
{
	unsigned char buffer[2];

	if (is_stopped_) {
		return 0xFF;
	}
	if (sockfd_ < 0) {
		if (!checkAndRestoreConnection(true)) {
			return 0xFF;
		}
	}
//	master_txn_cnt++;

//	std::cout << "(" << master_txn_cnt << "," << slave_txn_cnt << ") Master send: " << (int)data << " at " << fastCgb << std::endl;

	buffer[0] = data;
	buffer[1] = fastCgb;
#ifdef _WIN32
   if (::send(sockfd_, (char*) buffer, 2, 0) <= 0)
#else
	if (write(sockfd_, buffer, 2) <= 0)
#endif
   {
		log_cb(RETRO_LOG_ERROR, "Error writing to socket: %s\n", strerror(errno));
		close(sockfd_);
		sockfd_ = -1;
		return 0xFF;
	}

//	std::cout <<  "(" << master_txn_cnt << "," << slave_txn_cnt << ") Master waiting for response... " << std::endl;
#ifdef _WIN32
	if (recv(sockfd_, (char*) buffer, 2, 0) <= 0) 
#else
   if (read(sockfd_, buffer, 2) <= 0) 
#endif
   {
		log_cb(RETRO_LOG_ERROR, "Error reading from socket: %s\n", strerror(errno));
		close(sockfd_);
		sockfd_ = -1;
		return 0xFF;
	}

//	std::cout << "(" << master_txn_cnt << "," << slave_txn_cnt << ") Master received: " << (int)buffer[0] << " (speed " << (int)buffer[1] << ")" << std::endl;

//	fwrite(&data, 1, 1, fpout);
//	fwrite(buffer, 1, 1, fpin);
//	fflush(fpout);
//	fflush(fpin);

	return buffer[0];
}

bool NetSerial::check(unsigned char out, unsigned char& in, bool& fastCgb)
{
	unsigned char buffer[2];
#ifdef _WIN32
	u_long bytes_avail = 0;
#else
	int bytes_avail = 0;
#endif
	if (is_stopped_) {
		return false;
	}
	if (sockfd_ < 0) {
		if (!checkAndRestoreConnection(true)) {
			return false;
		}
	}
#ifdef _WIN32
   if (ioctlsocket(sockfd_, FIONREAD, &bytes_avail) < 0)
#else
	if (ioctl(sockfd_, FIONREAD, &bytes_avail) < 0)
#endif
   {
		log_cb(RETRO_LOG_ERROR, "IOCTL Failed: %s\n", strerror(errno));
		return false;
	}

	// No data available yet
	if (bytes_avail < 2) {
		return false;
	}

#ifdef _WIN32
	if (recv(sockfd_, (char*) buffer, 2, 0) <= 0) 
#else
   if (read(sockfd_, buffer, 2) <= 0) 
#endif
   {
		log_cb(RETRO_LOG_ERROR, "Error reading from socket: %s\n", strerror(errno));
		close(sockfd_);
		sockfd_ = -1;
		return false;
	}

//	slave_txn_cnt++;

	in = buffer[0];
	fastCgb = buffer[1];
//	std::cout << "(" << master_txn_cnt << "," << slave_txn_cnt << ") Slave read: " << (int)in << " at " << fastCgb << ", responding with " << (int)out << std::endl;

	buffer[0] = out;
	buffer[1] = 128;
   #ifdef _WIN32
      if (::send(sockfd_, (char*) buffer, 2, 0) <= 0)
   #else
   	if (write(sockfd_, buffer, 2) <= 0)
   #endif
   {
		log_cb(RETRO_LOG_ERROR, "Error writing to socket: %s\n", strerror(errno));
		close(sockfd_);
		sockfd_ = -1;
		return false;
	}

//	fwrite(&out, 1, 1, fpout);
//	fwrite(&in, 1, 1, fpin);
//	fflush(fpout);
//	fflush(fpin);

	return true;
}
