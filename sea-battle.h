#pragma once

#include <memory>

#include "game/field.h"
#include "players/player-interface.h"

class sea_battle_t {
public:
  enum turn_t { FIRST_PLAYER = 0, SECOND_PLAYER = 1 };

  sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1, std::shared_ptr<player_interface_t> player2,
               field_t field2);

  ~sea_battle_t() = default;

  void play();

  static turn_t change_turn(turn_t current_turn);
  static std::string get_player_name(turn_t turn);
  bool is_dead(field_t *p_field_enemy, field_t *enemy_field, int x, int y);
  int ship_len(field_t *field, int x, int y);

private:

  std::shared_ptr<player_interface_t> player1, player2;
  field_t *p1_f, *p2_f;
  field_t p1_e_f, p2_e_f;
  turn_t cur_turn=turn_t::FIRST_PLAYER;
  int p1_ships = 10;
  int p2_ships = 10;
};
