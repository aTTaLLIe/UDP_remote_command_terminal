#include "UDP.h"

UDP::UDP(bool type_dev) : type_dev(type_dev)
{
	if (type_dev)
	{
		send_str.resize(SIZE_CLIENT_DATA);
		recive_str.resize(SIZE_SERVER_DATA);
	}
	else
	{
		send_str.resize(SIZE_SERVER_DATA);
		recive_str.resize(SIZE_CLIENT_DATA);
	}
}

UDP::~UDP()
{

}

int UDP::open_socket(std::string ip_source, int port_source)
{
	try
	{
		socket = std::unique_ptr<ip::udp::socket>(new ip::udp::socket{ service, ip::udp::endpoint(ip::address::from_string(ip_source), port_source) });
		if (socket->is_open())
			return 0;
		else
			return 1;
	}
	catch (const boost::system::system_error& ex)
	{
		return -1;
	}
	return 0;
}

int UDP::close()
{
	try
	{
		socket->close();
	}
	catch (const boost::system::system_error& ex)
	{
		return -1;
	}
	return 0;
}

int UDP::set_receiver(std::string ip_destination, int port_destination)
{
	try
	{
		receiver_ep = ip::udp::endpoint(boost::asio::ip::address::from_string(ip_destination), port_destination);
	}
	catch (const boost::system::system_error& ex)
	{
		return -1;
	}
	return 0;
}

int UDP::send(const std::string& data)
{
	if (type_dev)
	{
		if (data.size() != SIZE_CLIENT_DATA)	return 1;
		send_str = data;
	}
	else
	{
		if (data.size() != SIZE_SERVER_DATA)	return 1;
		send_str = data;
	}

	try { socket->send_to(buffer(send_str), receiver_ep); }
	catch (const boost::system::system_error& ex) { return -1; }
	return 0;
}

int UDP::recive(std::string& recive_data)
{
	auto size_get = socket->receive_from(buffer(recive_str), receiver_ep);
	if (size_get != sizeof(recive_str))
		return -1;
	recive_data = recive_str;
	return 0;
}

int UDP::get_size_packet_self()
{
	return type_dev == 1 ? SIZE_CLIENT_DATA : SIZE_SERVER_DATA;
}

int UDP::get_size_packet_other()
{
	return type_dev == 1 ? SIZE_SERVER_DATA : SIZE_CLIENT_DATA;
}