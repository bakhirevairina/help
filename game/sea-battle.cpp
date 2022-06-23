#include "game/sea-battle.h"

#include <utility>

sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}

sea_battle_t::sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1,
                           std::shared_ptr<player_interface_t> player2, field_t field2)
  : active_player_ptr(std::move(player1))
  , inactive_player_ptr(std::move(player2))
  , active_player_field_ptr(&_field1)
  , inactive_pllayer_field_ptr(&_field2)
  , _field1(std::move(field1))
  , _field2(std::move(field2)) {    
    //fog_field1 = remove_ships(field1);
    //fog_field2 = remove_ships(field2);
}


void sea_battle_t::play() {

  field_t fog_field = remove_ships(*active_player_field_ptr);
  field_t enemy_fog_field = remove_ships(*inactive_pllayer_field_ptr);

  field_t *p_fog_field = &fog_field;
  field_t *p_enemy_fog_field = &enemy_fog_field;

  int active_player_ship_cells = 0;
  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i)
    for (size_t j = 0; j < field_t::FIELD_SIZE; ++j)
      if ((*active_player_field_ptr)[i][j] == field_t::SHIP_CELL)
        active_player_ship_cells++;

  int inactive_player_ship_cells = 0;
  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i)
    for (size_t j = 0; j < field_t::FIELD_SIZE; ++j)
      if ((*inactive_pllayer_field_ptr)[i][j] == field_t::SHIP_CELL)
        inactive_player_ship_cells++;

  while (true) {
    auto move = active_player_ptr->make_move(*active_player_field_ptr, *p_enemy_fog_field);
    auto [i, j] = move;

    if (field_t::is_cell_valid(i, j)) {
      switch ((*inactive_pllayer_field_ptr)[i][j]) {
        case field_t::MISS_CELL:
        case field_t::HIT_CELL:
          active_player_ptr->on_duplicate_move(i, j);
          break;

        case field_t::EMPTY_CELL:
          (*inactive_pllayer_field_ptr)[i][j] = field_t::MISS_CELL;
          (*p_enemy_fog_field)[i][j] = field_t::MISS_CELL;
          active_player_ptr->on_miss(i, j);
          std::swap(active_player_ptr, inactive_player_ptr);
          std::swap(active_player_field_ptr, inactive_pllayer_field_ptr);
          std::swap(p_fog_field, p_enemy_fog_field);
          std::swap(inactive_player_ship_cells, active_player_ship_cells);
          break;

        case field_t::SHIP_CELL:
          (*inactive_pllayer_field_ptr)[i][j] = field_t::HIT_CELL;
          (*p_enemy_fog_field)[i][j] = field_t::HIT_CELL;
          inactive_player_ship_cells--;

          if (is_kill(*inactive_pllayer_field_ptr, i, j)) {
            active_player_ptr->on_kill(i, j);

            if (inactive_player_ship_cells == 0) {
              active_player_ptr->on_win();
              inactive_player_ptr->on_lose();
              return;
            }
          }
          else
            active_player_ptr->on_hit(i, j);
          break;

        default:
          break;
      }
    }
    else
      active_player_ptr->on_incorrect_move(i, j);
  }
}

field_t sea_battle_t::remove_ships(field_t f) {
  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i)
    for (size_t j = 0; j < field_t::FIELD_SIZE; ++j)
      if (f[i][j] == field_t::SHIP_CELL)
        f[i][j] = field_t::EMPTY_CELL;
  return f;
}

bool sea_battle_t::is_kill(field_t &f, int i, int j) {
  for (auto direction : field_t::DIRECTIONS) {
    auto [x, y] = direction;

    while (field_t::is_cell_valid(i + x, j + y) &&
           f[i + x][j + y] == field_t::HIT_CELL) {
      x += direction.first;
      y += direction.second;
    }

    if (field_t::is_cell_valid(i + x, j + y) &&
        f[i + x][j + y] == field_t::SHIP_CELL)
      return false;
  }
  return true;
}


