/**
*	network.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/network.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_network::_network - Constructor of _network class.
*/
_network::_network(void)
{
	this->sock = socket(AF_INET, SOCK_DGRAM, 0);

	this->sddr_r.sdr_in.sin_addr.s_addr = INADDR_ANY;
	this->sddr_r.sdr_in.sin_port = htons(PORT);
	this->sddr_r.sdr_in.sin_family = AF_INET;

	if (this->sock == 0) {
		cout << "[E]: Can't start socket.\n";
		exit(1);
	}

	this->sddr_r.sdr = reinterpret_cast<struct sockaddr *>(
		&this->sddr_r.sdr_in);
	this->set_options(this->sock);
}
/**
*	_network::~_network - Distructor of _network class.
*/
_network::~_network(void)
{
	this->stop = true;

	if (this->sock != 0) {
		shutdown(this->sock, SHUT_RDWR);
		close(this->sock);
	}
}
/**
*	_network::set_options - Setting options for opend
*	socket.
*
*	@stream - Reference to socket.
*/
void _network::set_options(int &stream)
{
	int sol = SOL_SOCKET, opt;
	char *t_opt;

	if (stream == 0) {
		cout << "[E] _socket::set_opts.\n";
		return;
	}

	t_opt = reinterpret_cast<char *>(&this->timeout);
	this->timeout.tv_sec = TIMEOUT;
	opt = sizeof(this->timeout);
	this->timeout.tv_usec = 0;

	setsockopt(stream, sol, SO_RCVTIMEO, t_opt, opt);
	setsockopt(stream, sol, SO_SNDTIMEO, t_opt, opt);
}
/**
*	_network::set_options - Starting threads of the
*	module.
*/
bool _network::start(void)
{
	if (this->stop == true) {
		cout << "[E]: Can't start threads without setted"
			<< " node.\n";
		return false;
	}

	this->th_s = thread(&_network::thread_send, this);
	this->th_r = thread(&_network::thread_resv, this);

#ifdef TGN_DEBUG
	cout << "[I] Threads started.\n";
#endif

	return th_s.joinable() && th_r.joinable();
}
/**
*	_network::wait_threads - Waiting for ending of threads
*	working.
*/
void _network::wait_threads(void)
{
	if (this->th_s.joinable()) {
		this->th_s.join();
	}

	if (this->th_r.joinable()) {
		this->th_r.join();
	}
}
/**
*	_network::set_node - Setting new node of the network.
*
*	@node - Struct of new node.
*/
void _network::set_node(struct tgn_node node)
{
	size_t sz = sizeof(struct sockaddr_in);
	string ip = node.ip;
	this->stop = false;

	memcpy(&this->sddr_s.sdr_in, &this->sddr_r.sdr_in, sz);
	sddr_s.sdr_in.sin_addr.s_addr = inet_addr(ip.c_str());

	sddr_s.sdr = reinterpret_cast<struct sockaddr *>(
		&sddr_s.sdr_in);

	if (!this->start()) {
		cout << "[E]: Can't start threads.\n";
		exit(1);
	}
}
/**
*	_network::thread_send - Module thread for sending
*	packages to other members of the network.
*/
void _network::thread_send(void)
{
	socklen_t sz = sizeof(struct sockaddr_in);
	struct sockaddr *st = this->sddr_s.sdr;
	auto ctime = system_clock::now();
	struct tgn_pack pack;

	while (!this->stop) {
		if (ctime - tgnpacks.first_ping() > 10s) {
			tgnpacks.ping_packages();
			this->switching_node();
		}

		if (tgnpacks.size() == 0) {
			tgnpacks.system_request(REQUEST_PING);
			cout << "Ping pack.\n";
			sleep(2);
		}

		if (ctime - tgnnodes.last_request() > 1200s 
			&& tgnnodes.factual_size() < 20) {
			tgnpacks.system_request(REQUEST_NODES);
			tgnnodes.ping();
		}

		ctime = system_clock::now();
		pack = tgnpacks.get_one();
		sendto(sock, pack.buff, pack.length, 0x100, st,sz);
	}
}
/**
*	_network::thread_resv - Module thread for receiving
*	packages from other members of the network.
*/
void _network::thread_resv(void)
{
	size_t l = 0, fulls = FULLSIZE;
	unsigned char buff[fulls];
	struct sockaddr_in server;
	struct sockaddr *st;
	socklen_t sz;

	st = reinterpret_cast<struct sockaddr *>(&server);
	sz = sizeof(struct sockaddr_in);
	bind(sock, this->sddr_r.sdr, sz);
	memset(buff, 0x00, fulls);

	while (!this->stop) {
		l = recvfrom(sock, buff, fulls, 0x100, st, &sz);

		if (!this->current_node(server)) {
			memset(buff, 0x00, fulls);
			continue;
		}

		if (l >= HEADERSIZE) {
			this->process_request(buff);
			memset(buff, 0x00, fulls);
			continue;
		}

		this->switching_node();
	}
}
/**
*	_network::process_request - Processing request which was
*	received from connected node.
*
*	@buffer - Byte array of package.
*/
void _network::process_request(unsigned char *buffer)
{
	tgnmsg message(buffer);
	enum tgn_htype type = message.header_type();

	if (type == REQUEST_DOS || type > RESPONSE_GARLIC) {
		this->switching_node();
		return;
	}

	if (type == INDEFINITE_MESSAGE) {
		tgnnodes.remove_sddr(this->sddr_r.sdr_in);
		this->switching_node();
		return;
	}

	if (message.is_garlic()) {
		received_messages << message;
		return;
	}

	if (type % 2 != 0) {
		cout << "[W]: Received indefinite msg.\n";
		return;
	}

	tgnpacks.resp_msg(message);
}
/**
*	_network::process_request - Check was request from current
*	node or not.
*
*	@cr - Struct sockaddr_in.
*/
bool _network::current_node(struct sockaddr_in &cr)
{
	struct tgn_ipport c_node = ipport_get(sddr_s.sdr_in);
	struct tgn_ipport r_node = ipport_get(cr);

	return c_node.ip == r_node.ip;
}

void _network::switching_node(void)
{
	struct tgn_node node = tgnnodes.get_one();

	cout << "[I]: Switching to another node.\n";
	tgnnodes.used_sddr(this->sddr_s.sdr_in);

	if (tgnnodes.size() == 0) {
		cout << "[E]: There isn't node in db.\n";
		this->stop = true;
		return;
	}

	this->set_node(node);
}