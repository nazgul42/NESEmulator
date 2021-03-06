#include "GamepadState.h"
#include <allegro5/allegro.h>
#include <iostream>
#include "serial.h"
using namespace std;

GamepadState::GamepadState(void)
{
  player1.A = false;
  player1.B = false;
  player1.U = false;
  player1.D = false;
  player1.L = false;
  player1.R = false;
  player1.ST = false;
  player1.SEL = false;
  player2.A = false;
  player2.B = false;
  player2.U = false;
  player2.D = false;
  player2.L = false;
  player2.R = false;
  player2.ST = false;
  player2.SEL = false;
}

GamepadState::~GamepadState(void)
{
}

bool GamepadState::initializeKeyboard(ALLEGRO_EVENT_QUEUE* event_queue)
{
  cout << "Initializing gamepad...";
  if (!al_install_keyboard())
    {
      cout << "Error! Failed to initialize keyboard.\n";
      return false;
    }
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  cout << "Done.\n";
  return true;
}

bool GamepadState::initializeArduino()
{
  serial = new Serial();
  cout << "Setting baud rate...";
  if (!serial->Set_baud(9600))
    //return false;
  cout << "Done\nOpening serial port...";
  if (!serial->Open("/dev/tty.usbmodemfd131"))
    //return false;
  cout << "Done\n";
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
    case ALLEGRO_KEY_A:
      player1.A = value;
      break;
    case ALLEGRO_KEY_Z:
    case ALLEGRO_KEY_D:
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

/*void GamepadState::runArduinoTest()
{
  Serial* serial = new Serial();
  serial->Set_baud(9600);
  serial->Open("/dev/tty.usbmodemfd141");
  int data;
  while (true)
    {
      if (serial->Read(&data, 1))
	cout << (data & 0xFF) << "\n";
	}
	}*/

void GamepadState::readFromArduino()
{
  int data;
  if (serial->Read(&data, 1))
    {
      setStateArduino(data);
      serial->Input_discard(); // Ignore spammed signals... I think this improves responsiveness. It just seems to work.
    }
}

void GamepadState::setStateArduino(int arduinoState)
{
  player1.R = !(arduinoState & (1 << 7));
  player1.L = !(arduinoState & (1 << 6));
  player1.D = !(arduinoState & (1 << 5));
  player1.U = !(arduinoState & (1 << 4));
  player1.ST = !(arduinoState & (1 << 3));
  player1.SEL = !(arduinoState & (1 << 2));
  player1.A = !(arduinoState & (1 << 0));
  player1.B = !(arduinoState & (1 << 1));
  /*if (player1.A)
    cout << "A\n";
  if (player1.B)
    cout << "B\n";
  if (player1.ST)
    cout << "START\n";
  if (player1.SEL)
    cout << "SELECT\n";
  if (player1.R)
    cout << "->\n";
  if (player1.L)
    cout << "<-\n";
  if (player1.D)
    cout << "v\n";
  if (player1.U)
  cout << "^\n";*/
}
