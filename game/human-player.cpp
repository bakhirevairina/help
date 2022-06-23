#include "players/human-player.h"

std::pair<int, int> human_player_t::make_move(const field_t& my_field, const field_t& enemy_field) {
  output_stream << "Your field :" << std::endl << my_field << std::endl;
  output_stream << "Enemy's field" << enemy_field << std::endl;

 // bool incorrect_move = false;
	//
	//if (!first_time)
	//  human_player_t::on_incorrect_move();//?

	//bool first_time = false;

	std::string input;
    input_stream >> input;
	//suitable for small field_
	if (input.size() > 3 || input.size() == 0)
	  return std::make_pair(-1, -1);
 

	// no number
	if (!('0' < input[0] && input[0] <= '9'))
	  return std::make_pair(-1, -1);

	// letter is out of range
	char c = input.back();
	if (!('A' <= c && c <= 'J') || !('a' <= c && c <= 'j'))
	  return std::make_pair(-1, -1);

	input.pop_back();
	int x;
	x = stoi(input);
	// 10j is max,
	if (input.size() > 1 && x != field_t::FIELD_SIZE)
	  return std::make_pair(-1, -1);
	// x in range
	if (x < 1 || x > field_t::FIELD_SIZE)
	  return std::make_pair(-1, -1);

	return std::make_pair(x - 1, std::tolower(c) - 'a');
  
}

void human_player_t::on_incorrect_move(int x, int y)
{
  output_stream << "invalid size(10j is max and 1j is min[2; 3])" << std::endl;
}

void human_player_t::on_duplicate_move(int x, int y)
{
  output_stream << x << 'A' + y << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int x, int y)
{
  output_stream << x << 'A' + y << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int x, int y)
{
  output_stream << x << 'A' + y << " HIT! Your next move!" << std::endl;
}

void human_player_t::on_kill(int x, int y)
{
  output_stream << x << 'A' + y << " KILL! Your next move!" << std::endl;
}

