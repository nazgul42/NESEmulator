#include "GamepadState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
using namespace std;

GamepadState::GamepadState(void)
{
}

GamepadState::~GamepadState(void)
{
}

bool GamepadState::initializeKeyboard(ALLEGRO_EVENT_QUEUE* event_queue)
{
  cout << "Initializing gamepad...";
  if (!al_install_keyboard())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize keyboard!", NULL, NULL);
      return false;
    }
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  cout << "Done.\n";
  return true;
}

void GamepadState::keyDown(ALLEGRO_EVENT event)
{
  setValueForKey(event, true);
}

void GamepadState::keyUp(ALLEGRO_EVENT event)
{
  setValueForKey(event, false);
}

void GamepadState::setValueForKey(ALLEGRO_EVENT event, bool value)
{
  switch (event.keyboard.keycode)
    {
    case ALLEGRO_KEY_X:
      player1.A = value;
      break;
    case ALLEGRO_KEY_Z:
      player1.B = value;
      break;
    case ALLEGRO_KEY_UP:
      player1.U = value;
      break;
    case ALLEGRO_KEY_DOWN:
      player1.D = value;
      break;
    case ALLEGRO_KEY_LEFT:
      player1.L = value;
      break;
    case ALLEGRO_KEY_RIGHT:
      player1.R = value;
      break;
    case ALLEGRO_KEY_ENTER:
      player1.ST = value;
      break;
    case ALLEGRO_KEY_BACKSLASH:
      player1.SEL = value;
      break;
    }
}

void GamepadState::strobe()
{
  latchedP1 = player1;
  latchedP2 = player2;
  p1Index = 0;
  p2Index = 0;
}

bool GamepadState::gamepadValueForIndex(gamepad pad, int index)
{
  switch (index)
    {
    case 0:
      return pad.A;
    case 1:
      return pad.B;
    case 2:
      return pad.SEL;
    case 3:
      return pad.ST;
    case 4:
      return pad.U;
    case 5:
      return pad.D;
    case 6:
      return pad.L;
    case 7:
      return pad.R;
    default:
      return false;
    }
}

bool GamepadState::readPlayer1()
{
  return gamepadValueForIndex(latchedP1,p1Index++);
}

bool GamepadState::readPlayer2()
{
  return gamepadValueForIndex(latchedP2,p2Index++);
}