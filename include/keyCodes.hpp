#pragma once

namespace Manduca {



enum class KeyCode : int {
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

} // namespace Manduca
