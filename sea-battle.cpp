#include "game/sea-battle.h"
#include "sea-battle.h"

sea_battle_t::sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1,
                           std::shared_ptr<player_interface_t> player2, field_t field2)
    : player1(std::move(player1)), player2(std::move(player2)), p1_f(&_field1), p2_f(&_field2),
      p1_e_f(std::move(field1)), p2_e_f(std::move(field2)) {

  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i)
    for (size_t j = 0; j < field_t::FIELD_SIZE; ++j) {

      if (p1_e_f[i][j] == field_t::SHIP_CELL)
        p1_e_f[i][j] = field_t::EMPTY_CELL;
      if (p2_e_f[i][j] == field_t::SHIP_CELL)
        p2_e_f[i][j] = field_t::EMPTY_CELL;
    }
}



void sea_battle_t::play() {
  turn_t current_turn = FIRST_PLAYER;
  std::pair<int, int> move;
  bool correct_move = true;
  int x, y;
  field_t *player_field, *enemy_field, *player_enemy_field;
  std::shared_ptr<player_interface_t> player;
  bool win = 0;
  int *enemy_ship_num;



  while (1) {
    if (current_turn == FIRST_PLAYER) {
      player = player1;
      player_field = &(this->p1_f);
      player_enemy_field = &(this->p1_e_f);
      enemy_field = &(this->p2_f);
      enemy_ship_num = &p2_ships;
    } else {
      player = player2;
      player_field = &(this->p2_f);
      player_enemy_field = &(this->p2_e_f);
      enemy_field = &(this->p1_f);
      enemy_ship_num = &p1_ships;
    }

    if (win) {
      (*player).on_lose(); // on previous turn player won
      return;
    }

    move = player->make_move(*player_field, *player_enemy_field);
    x = move.first;
    y = move.second;

    // if invalid cell, try again
    if (!field_t::is_cell_valid(x, y)) {
      (*player).on_incorrect_move(x, y);
      continue;
    }

    // if cell already revealed
    if ((*player_enemy_field)[x][y] == field_t::HIT_CELL 
        || (*player_enemy_field)[x][y] == field_t::MISS_CELL) {
      player->on_duplicate_move(x, y);
      continue;
    }

    // if miss
    if ((*enemy_field)[x][y] == field_t::EMPTY_CELL) {
      player->on_miss(x, y);
      (*player_enemy_field)[x][y] = field_t::MISS_CELL;
      continue;
    }

    if ((*enemy_field)[x][y] == field_t::SHIP_CELL) {
      (*player_enemy_field)[x][y] = field_t::HIT_CELL;

      if (sea_battle_t::is_dead(enemy_field, player_enemy_field, x, y)) {
        (player)->on_kill(x, y);
        enemy_ship_num--;

        if (enemy_ship_num == 0) {
          (*player).on_win();

          continue;
        }
      }
      (*player).on_hit(x, y);
    }
  }
  current_turn = change_turn(current_turn);
}

// may be counted only on enemy's field, if only HIT_CELL touches
bool sea_battle_t::is_dead(field_t *p_field_enemy, field_t *enemy_field, int x, int y) {
  return ship_len(p_field_enemy, x, y) == ship_len(enemy_field, x, y);
}

// ships may be curved :D
int sea_battle_t::ship_len(field_t *field, int x, int y) {
  if (!field_t::is_cell_valid(x, y))
    return 0;

  return ((*field)[x][y] == field_t::EMPTY_CELL || (*field)[x][y] == field_t::MISS_CELL)
           ? 0
           : 1 + this->sea_battle_t::ship_len(field, x + 1, y) + this->sea_battle_t::ship_len(field, x - 1, y) +
                 this->sea_battle_t::ship_len(field, x, y + 1) + this->sea_battle_t::ship_len(field, x, y - 1);
}   

sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}


