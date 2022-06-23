#include "players/smart-ai-player.h"

std::pair<int, int> smart_ai_player_t::make_move(const field_t& my_field, // NOLINT
    const field_t& enemy_field) {
  if (is_cell_useful.size() == 0) {
    std::vector<bool> row(true, field_t::FIELD_SIZE);
    for (int i=0;i<field_t::FIELD_SIZE;i++)
      is_cell_useful.push_back(row);
  }


    if (enemy_field[0][0] == field_t::EMPTY_CELL && queue.size() == 0) // if not hitted any ships
  {
    for (int x = 0; x < field_t::FIELD_SIZE; x++)
      for (int y = 0; y < field_t::FIELD_SIZE; y++) {
        if ((x + y) % 2 == 0)
          queue.push_back(std::make_pair(x, y));
        else
          queue.insert(queue.begin(), std::make_pair(x, y));
      }
  }

    if (hitted.second == -1)
    while (true) {
        auto res = queue.back();
      queue.pop_back();
        if (is_cell_useful[res.first][res.second])
          return res;
    }

    int x, y;
    for (auto dir : field_t::DIRECTIONS) {
      x = hitted.first;
      y = hitted.second;
      while (field_t::is_cell_valid(x += dir.first, y += dir.second) && enemy_field[x][y] != field_t::MISS_CELL) {
        if (enemy_field[x][y] == field_t::EMPTY_CELL)
          return std::make_pair(x,y);
      }

    }
}

void smart_ai_player_t::on_incorrect_move(int, int) {}

void smart_ai_player_t::on_duplicate_move(int, int) {}

void smart_ai_player_t::on_miss(int x, int y) {
  is_cell_useful[x ][y ] = false;
}

void smart_ai_player_t::on_hit(int x, int y) {
  hitted = {x, y};
  for (auto dir : field_t::DIRECTIONS)
    if (field_t::is_cell_valid(x + dir.first, y + dir.second))
      is_cell_useful[x + dir.first][y + dir.second] = false;
}

void smart_ai_player_t::on_kill(int, int) {
  hitted = {-1, -1};
}

void smart_ai_player_t::on_win() {}

void smart_ai_player_t::on_lose() {}
