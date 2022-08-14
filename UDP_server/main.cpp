#include <iostream>
#include <thread>
#include "Main_Protocol.h"
#include "cmd_exec.h"
#include "UDP.h"

using namespace std;

void receiv_thread(Main_Protocol& socket)
{
	std::string user_data;
	uint8_t command;
	std::string quantaty_packets;
	std::vector<int> invalid_packet;

	while (true)
	{
		socket.receiv_command(command, quantaty_packets);
		if (command == 1)
		{
			socket.receiv_user_data(std::stoi(quantaty_packets), user_data, invalid_packet);
			if (!strcmp(user_data.c_str(), "exit"))
			{
				socket.send_command(0, "0");
				break;
			}
			if (invalid_packet.empty())
			{
				std::string ListStdOut;
				ListStdOut.reserve(1000);
				uint32_t RetCode{};
				int rc = SystemCapture(
					user_data,
					"C:\\Windows\\",
					ListStdOut,
					RetCode
				);
				if (rc < 0) {
					cout << "ERROR: SystemCapture\n";
				}
				std::cout << "STDOUT:" << std::endl;
				std::cout << ListStdOut << std::endl;
				int size_massage = std::ceil(static_cast<float>(ListStdOut.size()) / (socket.get_size_packet_self() - 2));
				socket.send_command(1, std::to_string(size_massage));
				socket.send_user_data(ListStdOut);
			}
		}
	}
}

int main() 
{
	Main_Protocol server(0);
	server.open_socket("127.0.0.1", 4444);

	std::thread reciev_socket(receiv_thread, std::ref(server));
	
	if (reciev_socket.joinable())
		reciev_socket.join();
	server.close();
	return 0;
}