/*
 * keycodes.h
 *
 *  Created on: Nov 6, 2024
 *      Author: Pep
 */

#ifndef INC_KEYCODES_H_
#define INC_KEYCODES_H_

#include "tusb.h"

//--------------------------------------------------------------------+
// Keycode Definitions
//--------------------------------------------------------------------+

enum {
  KC_NO = HID_KEY_NONE,
  KC_TRNS = 0x01,

  // Keyboard keycodes
  KC_A = HID_KEY_A,
  KC_B = HID_KEY_B,
  KC_C = HID_KEY_C,
  KC_D = HID_KEY_D,
  KC_E = HID_KEY_E,
  KC_F = HID_KEY_F,
  KC_G = HID_KEY_G,
  KC_H = HID_KEY_H,
  KC_I = HID_KEY_I,
  KC_J = HID_KEY_J,
  KC_K = HID_KEY_K,
  KC_L = HID_KEY_L,
  KC_M = HID_KEY_M,
  KC_N = HID_KEY_N,
  KC_O = HID_KEY_O,
  KC_P = HID_KEY_P,
  KC_Q = HID_KEY_Q,
  KC_R = HID_KEY_R,
  KC_S = HID_KEY_S,
  KC_T = HID_KEY_T,
  KC_U = HID_KEY_U,
  KC_V = HID_KEY_V,
  KC_W = HID_KEY_W,
  KC_X = HID_KEY_X,
  KC_Y = HID_KEY_Y,
  KC_Z = HID_KEY_Z,
  KC_1 = HID_KEY_1,
  KC_2 = HID_KEY_2,
  KC_3 = HID_KEY_3,
  KC_4 = HID_KEY_4,
  KC_5 = HID_KEY_5,
  KC_6 = HID_KEY_6,
  KC_7 = HID_KEY_7,
  KC_8 = HID_KEY_8,
  KC_9 = HID_KEY_9,
  KC_0 = HID_KEY_0,
  KC_ENT = HID_KEY_ENTER,
  KC_ESC = HID_KEY_ESCAPE,
  KC_BSPC = HID_KEY_BACKSPACE,
  KC_TAB = HID_KEY_TAB,
  KC_SPC = HID_KEY_SPACE,
  KC_MINS = HID_KEY_MINUS,
  KC_EQL = HID_KEY_EQUAL,
  KC_LBRC = HID_KEY_BRACKET_LEFT,
  KC_RBRC = HID_KEY_BRACKET_RIGHT,
  KC_BSLS = HID_KEY_BACKSLASH,
  KC_NUSH = HID_KEY_EUROPE_1,
  KC_SCLN = HID_KEY_SEMICOLON,
  KC_QUOT = HID_KEY_APOSTROPHE,
  KC_GRV = HID_KEY_GRAVE,
  KC_COMM = HID_KEY_COMMA,
  KC_DOT = HID_KEY_PERIOD,
  KC_SLSH = HID_KEY_SLASH,
  KC_CAPS = HID_KEY_CAPS_LOCK,
  KC_F1 = HID_KEY_F1,
  KC_F2 = HID_KEY_F2,
  KC_F3 = HID_KEY_F3,
  KC_F4 = HID_KEY_F4,
  KC_F5 = HID_KEY_F5,
  KC_F6 = HID_KEY_F6,
  KC_F7 = HID_KEY_F7,
  KC_F8 = HID_KEY_F8,
  KC_F9 = HID_KEY_F9,
  KC_F10 = HID_KEY_F10,
  KC_F11 = HID_KEY_F11,
  KC_F12 = HID_KEY_F12,
  KC_PSCR = HID_KEY_PRINT_SCREEN,
  KC_SCRL = HID_KEY_SCROLL_LOCK,
  KC_PAUS = HID_KEY_PAUSE,
  KC_INS = HID_KEY_INSERT,
  KC_HOME = HID_KEY_HOME,
  KC_PGUP = HID_KEY_PAGE_UP,
  KC_DEL = HID_KEY_DELETE,
  KC_END = HID_KEY_END,
  KC_PGDN = HID_KEY_PAGE_DOWN,
  KC_RGHT = HID_KEY_ARROW_RIGHT,
  KC_LEFT = HID_KEY_ARROW_LEFT,
  KC_DOWN = HID_KEY_ARROW_DOWN,
  KC_UP = HID_KEY_ARROW_UP,
  KC_NUM = HID_KEY_NUM_LOCK,
  KC_PSLS = HID_KEY_KEYPAD_DIVIDE,
  KC_PAST = HID_KEY_KEYPAD_MULTIPLY,
  KC_PMNS = HID_KEY_KEYPAD_SUBTRACT,
  KC_PPLS = HID_KEY_KEYPAD_ADD,
  KC_PENT = HID_KEY_KEYPAD_ENTER,
  KC_P1 = HID_KEY_KEYPAD_1,
  KC_P2 = HID_KEY_KEYPAD_2,
  KC_P3 = HID_KEY_KEYPAD_3,
  KC_P4 = HID_KEY_KEYPAD_4,
  KC_P5 = HID_KEY_KEYPAD_5,
  KC_P6 = HID_KEY_KEYPAD_6,
  KC_P7 = HID_KEY_KEYPAD_7,
  KC_P8 = HID_KEY_KEYPAD_8,
  KC_P9 = HID_KEY_KEYPAD_9,
  KC_P0 = HID_KEY_KEYPAD_0,
  KC_PDOT = HID_KEY_KEYPAD_DECIMAL,
  KC_NUBS = HID_KEY_EUROPE_2,
  KC_APP = HID_KEY_APPLICATION,
  KC_KB_POWER = HID_KEY_POWER,
  KC_PEQL = HID_KEY_KEYPAD_EQUAL,
  KC_F13 = HID_KEY_F13,
  KC_F14 = HID_KEY_F14,
  KC_F15 = HID_KEY_F15,
  KC_F16 = HID_KEY_F16,
  KC_F17 = HID_KEY_F17,
  KC_F18 = HID_KEY_F18,
  KC_F19 = HID_KEY_F19,
  KC_F20 = HID_KEY_F20,
  KC_F21 = HID_KEY_F21,
  KC_F22 = HID_KEY_F22,
  KC_F23 = HID_KEY_F23,
  KC_F24 = HID_KEY_F24,
  KC_EXEC = HID_KEY_EXECUTE,
  KC_HELP = HID_KEY_HELP,
  KC_MENU = HID_KEY_MENU,
  KC_SLCT = HID_KEY_SELECT,
  KC_STOP = HID_KEY_STOP,
  KC_AGIN = HID_KEY_AGAIN,
  KC_UNDO = HID_KEY_UNDO,
  KC_CUT = HID_KEY_CUT,
  KC_COPY = HID_KEY_COPY,
  KC_PSTE = HID_KEY_PASTE,
  KC_FIND = HID_KEY_FIND,
  KC_KB_MUTE = HID_KEY_MUTE,
  KC_KB_VOLUME_UP = HID_KEY_VOLUME_UP,
  KC_KB_VOLUME_DOWN = HID_KEY_VOLUME_DOWN,
  KC_LCAP = HID_KEY_LOCKING_CAPS_LOCK,
  KC_LNUM = HID_KEY_LOCKING_NUM_LOCK,
  KC_LSCR = HID_KEY_LOCKING_SCROLL_LOCK,
  KC_PCMM = HID_KEY_KEYPAD_COMMA,
  KC_KP_EQUAL_AS400 = HID_KEY_KEYPAD_EQUAL_SIGN,
  KC_INT1 = HID_KEY_KANJI1,
  KC_INT2 = HID_KEY_KANJI2,
  KC_INT3 = HID_KEY_KANJI3,
  KC_INT4 = HID_KEY_KANJI4,
  KC_INT5 = HID_KEY_KANJI5,
  KC_INT6 = HID_KEY_KANJI6,
  KC_INT7 = HID_KEY_KANJI7,
  KC_INT8 = HID_KEY_KANJI8,
  KC_INT9 = HID_KEY_KANJI9,
  KC_LNG1 = HID_KEY_LANG1,
  KC_LNG2 = HID_KEY_LANG2,
  KC_LNG3 = HID_KEY_LANG3,
  KC_LNG4 = HID_KEY_LANG4,
  KC_LNG5 = HID_KEY_LANG5,
  KC_LNG6 = HID_KEY_LANG6,
  KC_LNG7 = HID_KEY_LANG7,
  KC_LNG8 = HID_KEY_LANG8,
  KC_LNG9 = HID_KEY_LANG9,
  KC_ERAS = HID_KEY_ALTERNATE_ERASE,
  KC_SYRQ = HID_KEY_SYSREQ_ATTENTION,
  KC_CNCL = HID_KEY_CANCEL,
  KC_CLR = HID_KEY_CLEAR,
  KC_PRIR = HID_KEY_PRIOR,
  KC_RETN = HID_KEY_RETURN,
  KC_SEPR = HID_KEY_SEPARATOR,
  KC_OUT = HID_KEY_OUT,
  KC_OPER = HID_KEY_OPER,
  KC_CLAG = HID_KEY_CLEAR_AGAIN,
  KC_CRSL = HID_KEY_CRSEL_PROPS,
  KC_EXSL = HID_KEY_EXSEL,

  // System control keycodes
  KC_PWR = 0x00A5,
  KC_WAKE = 0x00A6,
  KC_SLEP = 0x00A7,

  // Consumer control keycodes
  KC_BRIU = 0x00A8,
  KC_BRID = 0x00A9,
  KC_MPLY = 0x00AA,
  KC_MNXT = 0x00AB,
  KC_MPRV = 0x00AC,
  KC_MSTP = 0x00AD,
  KC_MUTE = 0x00AE,
  KC_VOLU = 0x00AF,
  KC_VOLD = 0x00B0,
  KC_MSEL = 0x00B1,
  KC_MAIL = 0x00B2,
  KC_CALC = 0x00B3,
  KC_MYCM = 0x00B4,
  KC_WSCH = 0x00B5,
  KC_WHOM = 0x00B6,
  KC_WBAK = 0x00B7,
  KC_WFWD = 0x00B8,
  KC_WSTP = 0x00B9,
  KC_WREF = 0x00BA,
  KC_WFAV = 0x00BB,

  // Modifier keycodes
  KC_LCTL = HID_KEY_CONTROL_LEFT,
  KC_LSFT = HID_KEY_SHIFT_LEFT,
  KC_LALT = HID_KEY_ALT_LEFT,
  KC_LGUI = HID_KEY_GUI_LEFT,
  KC_RCTL = HID_KEY_CONTROL_RIGHT,
  KC_RSFT = HID_KEY_SHIFT_RIGHT,
  KC_RALT = HID_KEY_ALT_RIGHT,
  KC_RGUI = HID_KEY_GUI_RIGHT,
};

//--------------------------------------------------------------------+
// Keycode Range Check Functions
//--------------------------------------------------------------------+

#define IS_KEYBOARD_KEY(keycode) (KC_A <= keycode && keycode <= KC_EXSL)
#define IS_SYSTEM_CONTROL_KEY(keycode) (KC_PWR <= keycode && keycode <= KC_SLEP)
#define IS_CONSUMER_CONTROL_KEY(keycode)                                       \
  (KC_BRIU <= keycode && keycode <= KC_WFAV)
#define IS_MODIFIER_KEY(keycode) (KC_LCTL <= keycode && keycode <= KC_RGUI)

//--------------------------------------------------------------------+
// Keycode Conversion Functions
//
// WARNING: These functions do not perform bounds checking. Make sure the
// keycode is within the valid range before calling these functions.
//--------------------------------------------------------------------+

// Convert a keycode to a HID system control keycode
uint16_t keycode_to_system_control(uint16_t keycode);
// Convert a keycode to a HID consumer control keycode
uint16_t keycode_to_consumer_control(uint16_t keycode);
// Convert a keycode to a HID modifier flag
uint8_t keycode_to_modifier(uint16_t keycode);

#endif /* INC_KEYCODES_H_ */
