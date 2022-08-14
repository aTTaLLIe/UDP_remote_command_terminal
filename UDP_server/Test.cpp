#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include "UDP.h"
#include "Main_Protocol.h"


//BOOST_AUTO_TEST_CASE(NTP_client_test)
//{
//	NTP_protocol ntp_client(1);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_open_socket("300.0.0.1", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_open_socket("200", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_open_socket("127.0.0.1", 5689), 0);
//
//	BOOST_CHECK_EQUAL(ntp_client.NTP_set_receiver("127.0.0.1", 5689), 0);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_set_receiver("500.0.0.1", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_set_receiver("192.168.0.2", 4444), 0);
//
//	std::vector<unsigned char> send_vect{};
//	BOOST_CHECK_EQUAL(ntp_client.NTP_send(send_vect), 1);
//	send_vect.reserve(20);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_send(send_vect), 1);
//	send_vect.resize(10);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_send(send_vect), 1);
//	send_vect.resize(17);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_send(send_vect), -1);
//	send_vect.resize(20);
//	BOOST_CHECK_EQUAL(ntp_client.NTP_send(send_vect), 1);
//}
//
//BOOST_AUTO_TEST_CASE(NTP_server_test)
//{
//	NTP_protocol ntp_server(0);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_open_socket("300.0.0.1", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_open_socket("200", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_open_socket("127.0.0.1", 5689), 0);
//
//	BOOST_CHECK_EQUAL(ntp_server.NTP_set_receiver("127.0.0.1", 5689), 0);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_set_receiver("xc", 5689), -1);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_set_receiver("192.168.0.2", 4444), 0);
//
//	std::vector<unsigned char> send_vect{};
//	BOOST_CHECK_EQUAL(ntp_server.NTP_send(send_vect), 1);
//	send_vect.reserve(40);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_send(send_vect), 1);
//	send_vect.resize(17);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_send(send_vect), 1);
//	send_vect.resize(29);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_send(send_vect), -1);
//	send_vect.resize(40);
//	BOOST_CHECK_EQUAL(ntp_server.NTP_send(send_vect), 1);
//}
BOOST_AUTO_TEST_SUITE(Test_NTP_protocol)

BOOST_AUTO_TEST_CASE(server_client_test)
{
	Main_Protocol server(0);
	Main_Protocol client(1);

	BOOST_CHECK_EQUAL(client.open_socket("127.0.0.1", 4444), 0);
	BOOST_CHECK_EQUAL(server.open_socket("127.0.0.1", 123), 0);
	BOOST_CHECK_EQUAL(client.set_receiver("127.0.0.1", 123), 0);

	std::string send_vect(38, 0);
	std::iota(send_vect.begin(), send_vect.end(), 0);
	BOOST_CHECK_EQUAL(client.send_user_data(send_vect), 0);

	std::string resive_vect(38, 0 );
	std::vector<int> index_miss_packet;
	BOOST_CHECK_EQUAL(server.receiv_user_data(1, resive_vect, index_miss_packet), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	send_vect.resize(38);
	std::iota(send_vect.begin(), send_vect.end(), 10);
	BOOST_CHECK_EQUAL(server.send_user_data(send_vect), 0);

	resive_vect.resize(38);
	BOOST_CHECK_EQUAL(client.receiv_user_data(1, resive_vect, index_miss_packet), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	server.close();
	client.close();
}

BOOST_AUTO_TEST_CASE(UDP_test)
{
	UDP server(0);
	UDP client(1);

	BOOST_CHECK_EQUAL(client.open_socket("127.0.0.1", 4444), 0);
	BOOST_CHECK_EQUAL(server.open_socket("127.0.0.1", 123), 0);
	BOOST_CHECK_EQUAL(client.set_receiver("127.0.0.1", 123), 0);

	std::string send_vect(40, 0);
	std::iota(send_vect.begin(), send_vect.end(), 0);
	BOOST_CHECK_EQUAL(client.send(send_vect), 0);

	std::string resive_vect(40, 0);
	std::vector<int> index_miss_packet;
	BOOST_CHECK_EQUAL(server.recive(resive_vect), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	send_vect.resize(40);
	std::iota(send_vect.begin(), send_vect.end(), 10);
	BOOST_CHECK_EQUAL(server.send(send_vect), 0);

	resive_vect.resize(40);
	BOOST_CHECK_EQUAL(client.recive(resive_vect), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	server.close();
	client.close();
}



BOOST_AUTO_TEST_SUITE_END()