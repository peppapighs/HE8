/*
 * keycodes.h
 *
 *  Created on: Nov 6, 2024
 *      Author: Pep
 */

#ifndef INC_KEYCODES_H_
#define INC_KEYCODES_H_

#include <stdint.h>

//--------------------------------------------------------------------+
// Keycode Definitions
//--------------------------------------------------------------------+

enum {
  KC_NO = 0x0000,
  KC_TRNS = 0x0001,

  // Keyboard keycodes
  KC_A = 0x0004,
  KC_B = 0x0005,
  KC_C = 0x0006,
  KC_D = 0x0007,
  KC_E = 0x0008,
  KC_F = 0x0009,
  KC_G = 0x000A,
  KC_H = 0x000B,
  KC_I = 0x000C,
  KC_J = 0x000D,
  KC_K = 0x000E,
  KC_L = 0x000F,
  KC_M = 0x0010,
  KC_N = 0x0011,
  KC_O = 0x0012,
  KC_P = 0x0013,
  KC_Q = 0x0014,
  KC_R = 0x0015,
  KC_S = 0x0016,
  KC_T = 0x0017,
  KC_U = 0x0018,
  KC_V = 0x0019,
  KC_W = 0x001A,
  KC_X = 0x001B,
  KC_Y = 0x001C,
  KC_Z = 0x001D,
  KC_1 = 0x001E,
  KC_2 = 0x001F,
  KC_3 = 0x0020,
  KC_4 = 0x0021,
  KC_5 = 0x0022,
  KC_6 = 0x0023,
  KC_7 = 0x0024,
  KC_8 = 0x0025,
  KC_9 = 0x0026,
  KC_0 = 0x0027,
  KC_ENT = 0x0028,
  KC_ESC = 0x0029,
  KC_BSPC = 0x002A,
  KC_TAB = 0x002B,
  KC_SPC = 0x002C,
  KC_MINS = 0x002D,
  KC_EQL = 0x002E,
  KC_LBRC = 0x002F,
  KC_RBRC = 0x0030,
  KC_BSLS = 0x0031,
  KC_NUSH = 0x0032,
  KC_SCLN = 0x0033,
  KC_QUOT = 0x0034,
  KC_GRV = 0x0035,
  KC_COMM = 0x0036,
  KC_DOT = 0x0037,
  KC_SLSH = 0x0038,
  KC_CAPS = 0x0039,
  KC_F1 = 0x003A,
  KC_F2 = 0x003B,
  KC_F3 = 0x003C,
  KC_F4 = 0x003D,
  KC_F5 = 0x003E,
  KC_F6 = 0x003F,
  KC_F7 = 0x0040,
  KC_F8 = 0x0041,
  KC_F9 = 0x0042,
  KC_F10 = 0x0043,
  KC_F11 = 0x0044,
  KC_F12 = 0x0045,
  KC_PSCR = 0x0046,
  KC_SCRL = 0x0047,
  KC_PAUS = 0x0048,
  KC_INS = 0x0049,
  KC_HOME = 0x004A,
  KC_PGUP = 0x004B,
  KC_DEL = 0x004C,
  KC_END = 0x004D,
  KC_PGDN = 0x004E,
  KC_RGHT = 0x004F,
  KC_LEFT = 0x0050,
  KC_DOWN = 0x0051,
  KC_UP = 0x0052,
  KC_NUM = 0x0053,
  KC_PSLS = 0x0054,
  KC_PAST = 0x0055,
  KC_PMNS = 0x0056,
  KC_PPLS = 0x0057,
  KC_PENT = 0x0058,
  KC_P1 = 0x0059,
  KC_P2 = 0x005A,
  KC_P3 = 0x005B,
  KC_P4 = 0x005C,
  KC_P5 = 0x005D,
  KC_P6 = 0x005E,
  KC_P7 = 0x005F,
  KC_P8 = 0x0060,
  KC_P9 = 0x0061,
  KC_P0 = 0x0062,
  KC_PDOT = 0x0063,
  KC_NUBS = 0x0064,
  KC_APP = 0x0065,
  KC_KB_POWER = 0x0066,
  KC_PEQL = 0x0067,
  KC_F13 = 0x0068,
  KC_F14 = 0x0069,
  KC_F15 = 0x006A,
  KC_F16 = 0x006B,
  KC_F17 = 0x006C,
  KC_F18 = 0x006D,
  KC_F19 = 0x006E,
  KC_F20 = 0x006F,
  KC_F21 = 0x0070,
  KC_F22 = 0x0071,
  KC_F23 = 0x0072,
  KC_F24 = 0x0073,
  KC_EXEC = 0x0074,
  KC_HELP = 0x0075,
  KC_MENU = 0x0076,
  KC_SLCT = 0x0077,
  KC_STOP = 0x0078,
  KC_AGIN = 0x0079,
  KC_UNDO = 0x007A,
  KC_CUT = 0x007B,
  KC_COPY = 0x007C,
  KC_PSTE = 0x007D,
  KC_FIND = 0x007E,
  KC_KB_MUTE = 0x007F,
  KC_KB_VOLUME_UP = 0x0080,
  KC_KB_VOLUME_DOWN = 0x0081,
  KC_LCAP = 0x0082,
  KC_LNUM = 0x0083,
  KC_LSCR = 0x0084,
  KC_PCMM = 0x0085,
  KC_KP_EQUAL_AS400 = 0x0086,
  KC_INT1 = 0x0087,
  KC_INT2 = 0x0088,
  KC_INT3 = 0x0089,
  KC_INT4 = 0x008A,
  KC_INT5 = 0x008B,
  KC_INT6 = 0x008C,
  KC_INT7 = 0x008D,
  KC_INT8 = 0x008E,
  KC_INT9 = 0x008F,
  KC_LNG1 = 0x0090,
  KC_LNG2 = 0x0091,
  KC_LNG3 = 0x0092,
  KC_LNG4 = 0x0093,
  KC_LNG5 = 0x0094,
  KC_LNG6 = 0x0095,
  KC_LNG7 = 0x0096,
  KC_LNG8 = 0x0097,
  KC_LNG9 = 0x0098,
  KC_ERAS = 0x0099,
  KC_SYRQ = 0x009A,
  KC_CNCL = 0x009B,
  KC_CLR = 0x009C,
  KC_PRIR = 0x009D,
  KC_RETN = 0x009E,
  KC_SEPR = 0x009F,
  KC_OUT = 0x00A0,
  KC_OPER = 0x00A1,
  KC_CLAG = 0x00A2,
  KC_CRSL = 0x00A3,
  KC_EXSL = 0x00A4,

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
  KC_EJCT = 0x00BC,
  KC_MFFD = 0x00BD,
  KC_MRWD = 0x00BE,
  KC_CPNL = 0x00BF,
  KC_ASST = 0x00C0,
  KC_MCTL = 0x00C1,
  KC_LPAD = 0x00C2,

  // Modifier keycodes
  KC_LCTL = 0x00E0,
  KC_LSFT = 0x00E1,
  KC_LALT = 0x00E2,
  KC_LGUI = 0x00E3,
  KC_RCTL = 0x00E4,
  KC_RSFT = 0x00E5,
  KC_RALT = 0x00E6,
  KC_RGUI = 0x00E7,

  // End of basic keycodes
  BASIC_KC_MAX = 0x00FF,

  // Modifier mask keycodes
  MOD_MASK = 0x0100,
  MOD_MASK_CTL = MOD_MASK,
  MOD_MASK_SFT = 0x0200,
  MOD_MASK_ALT = 0x0400,
  MOD_MASK_GUI = 0x0800,
  MOD_MASK_MAX = 0x1FFF,

  // Modifier-tap keycodes
  MOD_TAP = 0x2100,
  MOD_TAP_CTL = MOD_TAP,
  MOD_TAP_SFT = 0x2200,
  MOD_TAP_ALT = 0x2400,
  MOD_TAP_GUI = 0x2800,
  MOD_TAP_MAX = 0x3FFF,

  // Layer-tap keycodes (support up to 16 layers)
  LAYER_TAP = 0x4000,
  LAYER_TAP_MAX = 0x4FFF,

  // Layer-modifier keycodes (support up to 16 layers)
  LAYER_MOD = 0x5000,
  LAYER_MOD_CTL = 0x5010,
  LAYER_MOD_SFT = 0x5020,
  LAYER_MOD_ALT = 0x5040,
  LAYER_MOD_GUI = 0x5080,
  LAYER_MOD_MAX = 0x51FF,

  // Start of one-time keycodes (keys that will not be processed again unless
  // they are released)
  ONE_TIME_KC_START = 0x5200,

  // Layer keycodes (support up to 16 layers)
  LAYER_MOMENTARY = ONE_TIME_KC_START,
  LAYER_MOMENTARY_MAX = 0x520F,
  LAYER_DEFAULT = 0x5210,
  LAYER_DEFAULT_MAX = 0x521F,
  LAYER_TOGGLE = 0x5220,
  LAYER_TOGGLE_MAX = 0x522F,

  // Profile keycodes (support up to 16 profiles)
  PROFILE_SET = 0x5300,
  PROFILE_SET_MAX = 0x530F,

  // Firmware related keycodes
  FW_BOOTLOADER = 0x7000,
  FW_REBOOT = 0x7001,
  FW_FACTORY_RESET = 0x7002,
  FW_NKRO_ON = 0x7003,
  FW_NKRO_OFF = 0x7004,
  FW_NKRO_TOGGLE = 0x7005,

  // End of one-time keycodes
  ONE_TIME_KC_END = 0x7FFF,

  // Aliases
  XXXXXXX = KC_NO,
  _______ = KC_TRNS,
};

//--------------------------------------------------------------------+
// Keycode Range Check Functions
//--------------------------------------------------------------------+

// Basic key ranges
#define IS_KEYBOARD_KEY(keycode) (KC_A <= (keycode) && (keycode) <= KC_EXSL)
#define IS_SYSTEM_CONTROL_KEY(keycode)                                         \
  (KC_PWR <= (keycode) && (keycode) <= KC_SLEP)
#define IS_CONSUMER_CONTROL_KEY(keycode)                                       \
  (KC_BRIU <= (keycode) && (keycode) <= KC_LPAD)
#define IS_MODIFIER_KEY(keycode) (KC_LCTL <= (keycode) && (keycode) <= KC_RGUI)
#define IS_BASIC_KEY(keycode) (KC_NO <= (keycode) && (keycode) <= BASIC_KC_MAX)

// Modifier mask ranges
#define IS_MOD_MASK(keycode)                                                   \
  (MOD_MASK <= (keycode) && (keycode) <= MOD_MASK_MAX)

// Modifier-tap ranges
#define IS_MOD_TAP(keycode) (MOD_TAP <= (keycode) && (keycode) <= MOD_TAP_MAX)

// Layer-tap ranges
#define IS_LAYER_TAP(keycode)                                                  \
  (LAYER_TAP <= (keycode) && (keycode) <= LAYER_TAP_MAX)

// Layer-modifier ranges
#define IS_LAYER_MOD(keycode)                                                  \
  (LAYER_MOD <= (keycode) && (keycode) <= LAYER_MOD_MAX)

// Layer ranges
#define IS_LAYER_MOMENTARY(keycode)                                            \
  (LAYER_MOMENTARY <= (keycode) && (keycode) <= LAYER_MOMENTARY_MAX)
#define IS_LAYER_DEFAULT(keycode)                                              \
  (LAYER_DEFAULT <= (keycode) && (keycode) <= LAYER_DEFAULT_MAX)
#define IS_LAYER_TOGGLE(keycode)                                               \
  (LAYER_TOGGLE <= (keycode) && (keycode) <= LAYER_TOGGLE_MAX)

// Profile ranges
#define IS_PROFILE_SET(keycode)                                                \
  (PROFILE_SET <= (keycode) && (keycode) <= PROFILE_SET_MAX)

// Firmware related ranges
#define IS_FIRMWARE_KEY(keycode)                                               \
  (FW_BOOTLOADER <= (keycode) && (keycode) <= FW_NKRO_TOGGLE)

// One-time key ranges
#define IS_ONE_TIME_KC(keycode)                                                \
  (ONE_TIME_KC_START <= (keycode) && (keycode) <= ONE_TIME_KC_END)

//--------------------------------------------------------------------+
// Modifier Mask Macros
//--------------------------------------------------------------------+

#define LCTL(keycode) ((keycode) | MOD_MASK_CTL)
#define LSFT(keycode) ((keycode) | MOD_MASK_SFT)
#define LALT(keycode) ((keycode) | MOD_MASK_ALT)
#define LGUI(keycode) ((keycode) | MOD_MASK_GUI)
#define RCTL(keycode) (LCTL(keycode) | 0x1000)
#define RSFT(keycode) (LSFT(keycode) | 0x1000)
#define RALT(keycode) (LALT(keycode) | 0x1000)
#define RGUI(keycode) (LGUI(keycode) | 0x1000)

#define MM_MODS(keycode)                                                       \
  ((((keycode) & 0x0F00) >> 8) << (((keycode) & 0x1000) >> 10))
#define MM_KEY(keycode) ((keycode) & 0x00FF)

// Shifted keycodes
enum {
  KC_TILD = LSFT(KC_GRV),
  KC_EXLM = LSFT(KC_1),
  KC_AT = LSFT(KC_2),
  KC_HASH = LSFT(KC_3),
  KC_DLR = LSFT(KC_4),
  KC_PERC = LSFT(KC_5),
  KC_CIRC = LSFT(KC_6),
  KC_AMPR = LSFT(KC_7),
  KC_ASTR = LSFT(KC_8),
  KC_LPRN = LSFT(KC_9),
  KC_RPRN = LSFT(KC_0),
  KC_UNDS = LSFT(KC_MINS),
  KC_PLUS = LSFT(KC_EQL),
  KC_LCBR = LSFT(KC_LBRC),
  KC_RCBR = LSFT(KC_RBRC),
  KC_PIPE = LSFT(KC_BSLS),
  KC_COLN = LSFT(KC_SCLN),
  KC_DQUO = LSFT(KC_QUOT),
  KC_LABK = LSFT(KC_COMM),
  KC_RABK = LSFT(KC_DOT),
  KC_QUES = LSFT(KC_SLSH),
};

//--------------------------------------------------------------------+
// Modifier-Tap Macros
//--------------------------------------------------------------------+

#define LCTL_T(keycode) ((keycode) | MOD_TAP_CTL)
#define LSFT_T(keycode) ((keycode) | MOD_TAP_SFT)
#define LALT_T(keycode) ((keycode) | MOD_TAP_ALT)
#define LGUI_T(keycode) ((keycode) | MOD_TAP_GUI)
#define RCTL_T(keycode) (LCTL_T(keycode) | 0x1000)
#define RSFT_T(keycode) (LSFT_T(keycode) | 0x1000)
#define RALT_T(keycode) (LALT_T(keycode) | 0x1000)
#define RGUI_T(keycode) (LGUI_T(keycode) | 0x1000)

#define MT_MODS(keycode)                                                       \
  ((((keycode) & 0x0F00) >> 8) << (((keycode) & 0x1000) >> 10))
#define MT_KEY(keycode) ((keycode) & 0x00FF)

//--------------------------------------------------------------------+
// Layer-Tap Macros
//--------------------------------------------------------------------+

#define LT(layer, keycode) (LAYER_TAP | ((layer) << 8) | (keycode))
#define LT_LAYER(keycode) (((keycode) & 0x0F00) >> 8)
#define LT_KEY(keycode) ((keycode) & 0x00FF)

//--------------------------------------------------------------------+
// Layer-Modifier Macros
//--------------------------------------------------------------------+

#define LCTL_LM(layer) ((layer) | LAYER_MOD_CTL)
#define LSFT_LM(layer) ((layer) | LAYER_MOD_SFT)
#define LALT_LM(layer) ((layer) | LAYER_MOD_ALT)
#define LGUI_LM(layer) ((layer) | LAYER_MOD_GUI)
#define RCTL_LM(layer) (LCTL_LM(layer) | 0x0100)
#define RSFT_LM(layer) (LSFT_LM(layer) | 0x0100)
#define RALT_LM(layer) (LALT_LM(layer) | 0x0100)
#define RGUI_LM(layer) (LGUI_LM(layer) | 0x0100)

#define LM_MODS(keycode)                                                       \
  ((((keycode) & 0x00F0) >> 4) << (((keycode) & 0x0100) >> 6))
#define LM_LAYER(keycode) ((keycode) & 0x000F)

//--------------------------------------------------------------------+
// Layer Macros
//--------------------------------------------------------------------+

#define MO(layer) (LAYER_MOMENTARY | (layer))
#define MO_LAYER(keycode) ((keycode) & 0x000F)

#define DF(layer) (LAYER_DEFAULT | (layer))
#define DF_LAYER(keycode) ((keycode) & 0x000F)

#define TG(layer) (LAYER_TOGGLE | (layer))
#define TG_LAYER(keycode) ((keycode) & 0x000F)

//--------------------------------------------------------------------+
// Profile Macros
//--------------------------------------------------------------------+

#define PS(profile) (PROFILE_SET | (profile))
#define PS_PROFILE(keycode) ((keycode) & 0x000F)

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
