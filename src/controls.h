#pragma once

// Input actions for the game
namespace controls {

// Place a piece
inline constexpr const char* CLICK = "click";

// Change the selection tile sprite
inline constexpr const char* CYCLE_SELECTOR = "cycle_selector";

// Bind the actions, call once after asw::core::init
void bind();

}  // namespace controls
