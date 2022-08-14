#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include "UDP.h"
#include "Main_Protocol.h"
#include "cmd_exec.h"

BOOST_AUTO_TEST_SUITE(Test_NTP_protocol)

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

	std::iota(send_vect.begin(), send_vect.end(), 10);
	BOOST_CHECK_EQUAL(server.send(send_vect), 0);

	BOOST_CHECK_EQUAL(client.recive(resive_vect), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	server.close();
	client.close();
}

BOOST_AUTO_TEST_CASE(User_protocol_test)
{
	Main_Protocol server(0);
	Main_Protocol client(1);

	BOOST_CHECK_EQUAL(client.open_socket("127.0.0.1", 4444), 0);
	BOOST_CHECK_EQUAL(server.open_socket("127.0.0.1", 123), 0);
	BOOST_CHECK_EQUAL(client.set_receiver("127.0.0.1", 123), 0);

	std::string send_vect(client.get_size_packet_self() - 2, 0);
	std::iota(send_vect.begin(), send_vect.end(), 0);
	BOOST_CHECK_EQUAL(client.send_user_data(send_vect), 0);

	std::string resive_vect(server.get_size_packet_self() - 2, 0 );
	std::vector<int> index_miss_packet;
	BOOST_CHECK_EQUAL(server.receiv_user_data(1, resive_vect, index_miss_packet), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	std::iota(send_vect.begin(), send_vect.end(), 10);
	BOOST_CHECK_EQUAL(server.send_user_data(send_vect), 0);

	BOOST_CHECK_EQUAL(client.receiv_user_data(1, resive_vect, index_miss_packet), 0);
	BOOST_CHECK(resive_vect == send_vect, true);

	server.close();
	client.close();
}

BOOST_AUTO_TEST_CASE(Exec_test)
{
	std::string ListStdOut;
	uint32_t RetCode;
	SystemCapture("echo 'Hellow world'", "C:\\Windows\\", ListStdOut, RetCode);
	BOOST_CHECK(!strcmp(ListStdOut.c_str(), "'Hellow world'\r\n"));
	ListStdOut.clear();
	SystemCapture("echo A", "C:\\Windows\\", ListStdOut, RetCode);
	BOOST_CHECK(!strcmp(ListStdOut.c_str(), "A\r\n"));
}


BOOST_AUTO_TEST_SUITE_END()