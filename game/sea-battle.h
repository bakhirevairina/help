#pragma once

#include <memory>

#include "game/field.h"
#include "players/player-interface.h"

class sea_battle_t {
public:
  enum turn_t { FIRST_PLAYER = 0, SECOND_PLAYER = 1 };

  sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1,
               std::shared_ptr<player_interface_t> player2, field_t field2);

  ~sea_battle_t() = default;

  void play();

  static turn_t change_turn(turn_t current_turn);
  static std::string get_player_name(turn_t turn);

private:
  std::shared_ptr<player_interface_t> active_player_ptr, inactive_player_ptr;
  field_t *active_player_field_ptr, *inactive_pllayer_field_ptr;
  field_t _field1, _field2;
//  field_t fog_field1, fog_field2;

  static field_t remove_ships(field_t f);

  static bool is_kill(field_t &f, int i, int j);

};
