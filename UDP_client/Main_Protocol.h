#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "UDP.h"

class Main_Protocol : public UDP
{
private:
	void missing_packet(unsigned int& count_packet, const std::string& receiv_data, std::vector<int>& index_miss_packet);
	bool check_crc(const std::string& receiv_data);
	int concat_data(std::string& main_data, const std::string& receiv_data);
	uint16_t calculate_crc(const std::string& data);
	bool end_packet(const unsigned int& quntaty_packets, const std::string& receiv_data);

public:
	Main_Protocol(bool type_dev) : UDP(type_dev) {};
	~Main_Protocol() {};

	int send_user_data(std::string data);
	int send_command(const uint8_t& command, const std::string& command_data);
	int receiv_user_data(const unsigned int& quntaty_packets, std::string& main_data, std::vector<int>& index_miss_packet);
	int receiv_command(uint8_t& command, std::string& command_data);
};

