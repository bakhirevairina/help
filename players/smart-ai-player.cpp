#include "players/smart-ai-player.h"

std::pair<int, int> smart_ai_player_t::make_move(const field_t &my_field, // NOLINT
                                                 const field_t &enemy_field) {

  if (hitted.second != -1 && hitted.first!=-1) {
    for (auto direction : field_t::DIRECTIONS) {
      int x = hitted.first ;
      int y = hitted.second;
      while (field_t::is_cell_valid(x+  direction.first , y + direction.second )) {
        x += direction.first;
        y += direction.second;
        if (enemy_field[x  ][y  ] == field_t::EMPTY_CELL &&
            _close_to_ships.find({x , y }) == _close_to_ships.end())
          return {x , y };
        else if (enemy_field[x ][y ] != field_t::HIT_CELL)
          break;
      }
    }
    return {-1, -1};
  }
  
  //for (size_t i = 0; i < field_t::FIELD_SIZE; ++i)
  //  for (size_t j = 0; j < field_t::FIELD_SIZE; ++j)
  //    if (enemy_field[i][j] == field_t::HIT_CELL)
  //      for (auto direction : field_t::DIRECTIONS)
  //        _close_to_ships.insert({i + direction.first, j + direction.second});

  if (queue.size()==0)
  for (int x = 0; x < field_t::FIELD_SIZE; x++)
    for (int y = 0; y < field_t::FIELD_SIZE; y++)
      if ((x + y) % 2 == 0)
        queue.push_back(std::make_pair(x, y));
      else
        queue.insert(queue.begin(), std::make_pair(x, y));

  int x, y;
  //not close to ships and empty     
  while (true) {
    bool no_ship_near = true;
    auto coords = queue.back();
    queue.pop_back();
     x = coords.first;
     y = coords.second;
    if (enemy_field[x][y] != field_t::EMPTY_CELL)
      continue;

    for (auto direction : field_t::DIRECTIONS)
      //if (field_t::is_cell_valid(x + direction.first,y + direction.second);
      if (enemy_field[x + direction.first][y + direction.second] == field_t::HIT_CELL)
        no_ship_near = false;
    if (no_ship_near)
      return {x, y};

  }


}

void smart_ai_player_t::on_incorrect_move(int, int) {}

void smart_ai_player_t::on_duplicate_move(int, int) {}

void smart_ai_player_t::on_miss(int, int) {}

void smart_ai_player_t::on_hit(int x, int y) {
  hitted = std::make_pair(x, y);
}

void smart_ai_player_t::on_kill(int x, int y) {
  hitted = std::make_pair(-1, -1);
}

void smart_ai_player_t::on_win() {}

void smart_ai_player_t::on_lose() {}
