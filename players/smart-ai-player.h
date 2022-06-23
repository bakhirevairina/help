#pragma once

#include "players/player-interface.h"

#include <set>

class smart_ai_player_t : public player_interface_t {
public:
  virtual ~smart_ai_player_t() = default;

  std::pair<int, int> make_move(const field_t &, const field_t &) override;

  void on_incorrect_move(int, int) override;

  void on_duplicate_move(int, int) override;

  void on_miss(int, int) override;

  void on_hit(int, int) override;

  void on_kill(int, int) override;

  void on_win() override;

  void on_lose() override;

private:
  int _x, _y;

  std::vector<std::pair<int,int>> queue;
  std::pair<int, int> hitted = {-1, -1};
  std::set<std::pair<int, int>> _close_to_ships;
};
