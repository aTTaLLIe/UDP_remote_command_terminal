#include "Main_Protocol.h"


int Main_Protocol::send_command(const uint8_t& command, const std::string& command_data)
{
	std::string buffer;
	buffer.resize(get_size_packet_self());
	buffer[0] = command;
	buffer.replace(1, command_data.size(), command_data);
	auto crc = calculate_crc(buffer);
	buffer[buffer.size() - 2] = crc;
	buffer[buffer.size() - 1] = crc >> 8;
	send(buffer);
	return 0;
}


int Main_Protocol::send_user_data(std::string data)
{
	int size_send_data = get_size_packet_self() - sizeof(uint16_t);
	if (data.empty())
		return 0;
	uint16_t count_packeg = static_cast<uint16_t>(data.size()) / (size_send_data);
	std::string buffer(get_size_packet_self(), '\0');
	for (uint16_t i = 0; i < count_packeg; ++i)
	{
		memcpy((void *)buffer.data(), &i, sizeof(i));
		buffer.replace(buffer.begin() + sizeof(count_packeg), buffer.end(), data.substr(i * size_send_data, size_send_data));
		send(buffer);
	}
	uint16_t remainder = static_cast<uint16_t>(data.size()) % size_send_data;
	if (remainder)
	{
		std::fill(buffer.begin(), buffer.end(), '\0');
		memcpy((void*)buffer.data(), &count_packeg, sizeof(count_packeg));
		auto t = data.substr(count_packeg * size_send_data, remainder);
		buffer.replace(sizeof(count_packeg), remainder, data.substr(count_packeg * size_send_data, remainder));
		send(buffer);
	}
	return 0;
}


int Main_Protocol::receiv_command(uint8_t& command, std::string& command_data)
{
	std::string buffer;
	buffer.resize(get_size_packet_other());
	recive(buffer);
	if (!check_crc(buffer))
		return -1;
	command = buffer[0];
	command_data = buffer.substr(1, command_data.size() - 3);
	return 0;
}


int Main_Protocol::receiv_user_data(const unsigned int& quntaty_packets, std::string& main_data, std::vector<int>& index_miss_packet)
{
	std::string buffer;
	buffer.resize(get_size_packet_other());
	unsigned int count_packet{};
	while (true)
	{
		recive(buffer);
		concat_data(main_data, buffer);
		missing_packet(count_packet, buffer, index_miss_packet);
		if (end_packet(quntaty_packets, buffer))
			break;
	}
	return 0;
}


void Main_Protocol::missing_packet(unsigned int& count_packet, const std::string& receiv_data, std::vector<int>& index_miss_packet)
{
	unsigned int receive_number = (receiv_data[1] << 8) + receiv_data[0];
	if (receive_number - count_packet >= 1)
	{
		for (int i = count_packet; i < (receive_number); ++i)
			index_miss_packet.push_back(i);
	}
	count_packet = receive_number + 1;
}


int Main_Protocol::concat_data(std::string& main_data, const std::string& receiv_data)
{
	try
	{
		uint16_t number_packet = (receiv_data[1] << 8) + receiv_data[0];
		auto size_useful_data = receiv_data.size() - sizeof(number_packet);
		if (number_packet * (size_useful_data) > main_data.capacity())
		{
			main_data.resize(number_packet * (size_useful_data));
		}
		main_data.replace(number_packet * (size_useful_data), size_useful_data,
			receiv_data.substr(sizeof(number_packet), size_useful_data));
	}
	catch (const std::runtime_error& e)
	{
		return -1;
	}
	return 0;
}


bool Main_Protocol::end_packet(const unsigned int& quntaty_packets, const std::string& receiv_data)
{
	return quntaty_packets == ((receiv_data[1] << 8) + receiv_data[0] + 1);
}


bool Main_Protocol::check_crc(const std::string& receiv_data)
{
	uint16_t rec_crc = uint8_t(receiv_data[receiv_data.size() - 2]) + (receiv_data[receiv_data.size() - 1] << 8);
	uint16_t calc_crc = calculate_crc(receiv_data);
	return rec_crc == calc_crc;
}


uint16_t Main_Protocol::calculate_crc(const std::string& data)
{
	uint16_t crc{};
	for (auto i = data.cbegin(); i != data.cend() - 2; ++i)
	{
		crc += (*i + 1) * 111;
	}
	for (int i = 0; i < 2; ++i)
	{
		crc = crc ^ (crc << (i + 1) * 2);
	}
	return crc;
}
