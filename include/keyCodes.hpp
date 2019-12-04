/**
 * @file keyCode.cpp
 * @author Thor Mortensen (thor.mortensen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

namespace loxo {



enum class keyCode : int {
  NOP,
  FUNC_START = 27,
  FUNC_CONF = 91,
  UP = 65,
  DOWN,
  RIGHT,
  LEFT,
  ENTER = 13,
  BACK_SPACE = 127,
  TAB = 9,
  END = 70,
  HOME = 72,
  DEL_START = 51,
  DEL = 126,
  SIGINT_KEY = 3,
};

} // namespace loxo
