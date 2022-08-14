#pragma once
#include<boost/asio.hpp>
#include<memory>
#include<sstream>
#include<iostream>
#include<vector>

using namespace boost::asio;
using namespace std;

class UDP
{
private:
	const int SIZE_CLIENT_DATA = 40;
	const int SIZE_SERVER_DATA = 40;

	bool type_dev; // 0 - server, 1 - client
	io_service service;
	std::unique_ptr<ip::udp::socket> socket;
	ip::udp::endpoint receiver_ep;
	std::string send_str;
	std::string recive_str;


public:
	UDP(bool type_dev);
	~UDP();
	int open_socket(std::string ip_source, int port_source);
	int set_receiver(std::string ip_destination, int port_destination);
	int send(const std::string& data);
	int recive(std::string& recive_data);
	int close();

	int get_size_packet_self();
	int get_size_packet_other();

};

