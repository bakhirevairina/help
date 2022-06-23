#include "players/human-player.h"

#include <cmath>

human_player_t::human_player_t(std::istream &input_stream, std::ostream &output_stream)
    : input_stream(input_stream), output_stream(output_stream) {
}

std::pair<int, int> human_player_t::make_move(const field_t &my_field, const field_t &enemy_field) {
  output_stream << "Your field:" << std::endl;
  output_stream << my_field << std::endl;
  output_stream << "Enemy's field" << std::endl;
  output_stream << enemy_field << std::endl;
  output_stream << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;

  std::string move;
  while (true) {
    std::getline(input_stream, move);

    
    if (move.size() == 2 && '1' <= move[0] && move[0] <= '9' && 'A' <= move[1] && move[1] <= 'J')
      return std::make_pair(move[0] - '1', move[1] - 'A');

    if (move.size() == 3 && move[0] == '1' && move[1] == '0' && 'A' <= move[2] && move[2] <= 'J')
      return std::make_pair(9, int(move[2] - 'A'));

    output_stream << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;
  }
}

void human_player_t::on_incorrect_move(int, int) {}

void human_player_t::on_duplicate_move(int i, int j) {
  output_stream << std::to_string(1 + i) << char('A' + j) << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int i, int j) {
  output_stream << std::to_string(1 + i) << char('A' + j) << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int i, int j) {
  output_stream << std::to_string(i+1) << char('A' + j) << " HIT! Your next move!" << std::endl;
}

void human_player_t::on_kill(int i, int j) {
  output_stream << std::to_string(1 + i) << char('A' + j) << " KILL! Your next move!" << std::endl;
}

void human_player_t::on_win() {
  output_stream << "You WIN!" << std::endl;
}

void human_player_t::on_lose() {
  output_stream << "You LOSE!" << std::endl;
}