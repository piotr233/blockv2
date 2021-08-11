//
// Created by studio25 on 29.07.2021.
//

#ifndef BLOCK_V2_BOTS_BOT_H_
#define BLOCK_V2_BOTS_BOT_H_
//#include "../game.h"
#include "../utility/coord.h"
//#include "../utility/movement_direction.h"
#include <ostream>
#include <transposition.h>

/// contains all used cell types
enum class BotType {
  BASIC, /// something like stone
  BEDROCK,
  TURN, /// changes the direction of  block
  GOAL,
  ENEMY,
  ENGINE,
  FACTORY,
  TP,
  EMPTY,
  NONE,
  SIZE
};


Coord NextPosition(Direction direction, const Coord &current_position);
//Coord NextPosition(movement_direction::Direction direction,
//                   const Coord &current_position);

enum class TurnDirection { CLOCKWISE, COUNTER_CLOCKWISE };

class Bot {
public:
  Bot() = default;

  virtual Bot *Clone() {
    assert(false);
    return new Bot(*this);
  };

  virtual void OutputToFile(std::ostream &out) const {};
  virtual BotType GetType() const { return type_; }

  virtual void Action(const std::vector<Bot *> &plane,
                      const Coord &bot_position, const unsigned plane_width,
                      const unsigned plane_height){};

  virtual void CalculateMovementDirection(
      const std::vector<Bot *> &plane, const Coord &bot_position,
      unsigned int plane_width, unsigned int plane_height,
                                          Direction push_direction){};

  virtual void LockEdge(const std::vector<Bot *> &plane,
                        const Coord &bot_position,
                        const unsigned int plane_width,
                        const unsigned int plane_height,
                        Direction push_direction){};

  virtual void ClearMovementDirection(){};

  const BotType type_ = BotType::NONE;

  Transposition movement_;
};

#endif // BLOCK_V2_BOTS_BOT_H_
