#include <iostream>
#include <thread>
#include <mutex>
#include "Main_Protocol.h"
#include "UDP.h"

using namespace std;

std::mutex console_mtx;
std::mutex client_mtx;

void receiv_thread(Main_Protocol &client)
{
	std::string mess;
	uint8_t command;
	std::string quantaty_packets;
	std::vector<int> invalid_packet;
	mess.reserve(100);
	while (true)
	{
		mess.clear();

		client.receiv_command(command, quantaty_packets);
		if (command == 0)
			break;

		client_mtx.lock();
		client.receiv_user_data(std::stoi(quantaty_packets), mess, invalid_packet);
		client_mtx.unlock();

		console_mtx.lock();
		std::cout << mess << endl;
		std::cout << "Command: ";
		console_mtx.unlock();
	}
}


int main() 
{
	Main_Protocol client(1);
	client.open_socket("127.0.0.1", 123);
	client.set_receiver("127.0.0.1", 4444);

	std::thread receiv_socket(receiv_thread, std::ref(client));
	std::cout << "Command: ";

	while (true)
	{
		std::string mess;
		mess.reserve(100);
		getline(cin, mess);
		console_mtx.lock();
		std::cout << std::endl;
		console_mtx.unlock();

		client_mtx.lock();
		int quantaty_massage = std::ceil(static_cast<float>(mess.size()) / (client.get_size_packet_self() - 2));
		client.send_command(1, std::to_string(quantaty_massage));
		client.send_user_data(mess);
		client_mtx.unlock();

		if (!strcmp(mess.c_str(), "exit"))
			break;
	}
	if(receiv_socket.joinable())
		receiv_socket.join();
	client.close();
	return 0;
}