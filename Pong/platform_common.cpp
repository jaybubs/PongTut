struct Button_State {
  bool is_down;
  bool changed;
};

enum {
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_1UP,
  BUTTON_1DN,
  BUTTON_ENTER,
  BUTTON_ESC,

  BUTTON_COUNT, //SHOULD BE THE LAST ITEM
};

struct Input {
  Button_State buttons[BUTTON_COUNT];
};
