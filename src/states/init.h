#pragma once

#include <asw/asw.h>

#include "./state.h"

class Init : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void update(float dt) override { manager.set_next_scene(States::Menu); }
};