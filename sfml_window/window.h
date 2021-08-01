//
// Created by studio25 on 29.07.2021.
//

#ifndef BLOCK_V2_SFML_WINDOW_WINDOW_H_
#define BLOCK_V2_SFML_WINDOW_WINDOW_H_

#include "../bots/bots_main.h"
#include "context.h"
#include "main_menu.h"
#include <SFML/Graphics/Shape.hpp>
#include <iostream>
#include <thread>

namespace sfml_window {
/// creates window object in separate tread
/// window update is done by pushing special event on to event stack
class Gui {
public:
  Gui();
  ~Gui();

private:
  void ThMainLoop();

protected:

  sfml_window::Context* current_context_;
  std::thread *window_thread_;
  sf::Event event_;

};
} // namespace sfml
#endif // BLOCK_V2_SFML_WINDOW_WINDOW_H_
