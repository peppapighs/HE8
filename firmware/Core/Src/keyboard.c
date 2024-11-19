/*
 * keyboard.h
 *
 *  Created on: Nov 13, 2024
 *      Author: Pep
 */

#include "keyboard.h"

#include <memory.h>
#include <stdbool.h>

#include "eeprom.h"
#include "keyboard_config.h"
#include "main.h"

//--------------------------------------------------------------------+
// Keyboard Configuration
//--------------------------------------------------------------------+

switch_profile_t const switch_profiles[SWITCH_PROF_COUNT] = {
    [SWITCH_PROF_GEON_RAW_HE] =
        {
            .travel_distance = 340,
            .adc_rest_value = 1700,
            .adc_offset = 700,
        },
    [SWITCH_PROF_GATERON_MAGNETIC_JADE] =
        {
            .travel_distance = 350,
            .adc_rest_value = 1700,
            .adc_offset = 700,
        },
    [SWITCH_PROF_WOOTING_LEKKER] =
        {
            .travel_distance = 400,
            .adc_rest_value = 1700,
            .adc_offset = 700,
        },
    [SWITCH_PROF_GATERON_KS_20] =
        {
            .travel_distance = 410,
            .adc_rest_value = 1700,
            .adc_offset = 700,
        },
};

uint16_t const distance_table[DISTANCE_TABLE_GRAIN] = {
    1023, 1022, 1022, 1021, 1020, 1020, 1019, 1018, 1018, 1017, 1016, 1016,
    1015, 1014, 1014, 1013, 1012, 1012, 1011, 1010, 1010, 1009, 1008, 1008,
    1007, 1006, 1006, 1005, 1004, 1004, 1003, 1002, 1002, 1001, 1000, 1000,
    999,  998,  998,  997,  996,  995,  995,  994,  993,  993,  992,  991,
    991,  990,  989,  989,  988,  987,  987,  986,  985,  985,  984,  983,
    983,  982,  981,  981,  980,  979,  979,  978,  977,  976,  976,  975,
    974,  974,  973,  972,  972,  971,  970,  970,  969,  968,  968,  967,
    966,  966,  965,  964,  963,  963,  962,  961,  961,  960,  959,  959,
    958,  957,  957,  956,  955,  955,  954,  953,  952,  952,  951,  950,
    950,  949,  948,  948,  947,  946,  946,  945,  944,  943,  943,  942,
    941,  941,  940,  939,  939,  938,  937,  936,  936,  935,  934,  934,
    933,  932,  932,  931,  930,  929,  929,  928,  927,  927,  926,  925,
    925,  924,  923,  922,  922,  921,  920,  920,  919,  918,  918,  917,
    916,  915,  915,  914,  913,  913,  912,  911,  911,  910,  909,  908,
    908,  907,  906,  906,  905,  904,  903,  903,  902,  901,  901,  900,
    899,  898,  898,  897,  896,  896,  895,  894,  893,  893,  892,  891,
    891,  890,  889,  888,  888,  887,  886,  886,  885,  884,  883,  883,
    882,  881,  881,  880,  879,  878,  878,  877,  876,  876,  875,  874,
    873,  873,  872,  871,  871,  870,  869,  868,  868,  867,  866,  865,
    865,  864,  863,  863,  862,  861,  860,  860,  859,  858,  857,  857,
    856,  855,  855,  854,  853,  852,  852,  851,  850,  849,  849,  848,
    847,  846,  846,  845,  844,  844,  843,  842,  841,  841,  840,  839,
    838,  838,  837,  836,  835,  835,  834,  833,  833,  832,  831,  830,
    830,  829,  828,  827,  827,  826,  825,  824,  824,  823,  822,  821,
    821,  820,  819,  818,  818,  817,  816,  815,  815,  814,  813,  812,
    812,  811,  810,  809,  809,  808,  807,  806,  806,  805,  804,  803,
    803,  802,  801,  800,  800,  799,  798,  797,  797,  796,  795,  794,
    794,  793,  792,  791,  791,  790,  789,  788,  788,  787,  786,  785,
    785,  784,  783,  782,  781,  781,  780,  779,  778,  778,  777,  776,
    775,  775,  774,  773,  772,  772,  771,  770,  769,  768,  768,  767,
    766,  765,  765,  764,  763,  762,  761,  761,  760,  759,  758,  758,
    757,  756,  755,  755,  754,  753,  752,  751,  751,  750,  749,  748,
    748,  747,  746,  745,  744,  744,  743,  742,  741,  740,  740,  739,
    738,  737,  737,  736,  735,  734,  733,  733,  732,  731,  730,  729,
    729,  728,  727,  726,  726,  725,  724,  723,  722,  722,  721,  720,
    719,  718,  718,  717,  716,  715,  714,  714,  713,  712,  711,  710,
    710,  709,  708,  707,  706,  706,  705,  704,  703,  702,  702,  701,
    700,  699,  698,  698,  697,  696,  695,  694,  694,  693,  692,  691,
    690,  689,  689,  688,  687,  686,  685,  685,  684,  683,  682,  681,
    680,  680,  679,  678,  677,  676,  676,  675,  674,  673,  672,  671,
    671,  670,  669,  668,  667,  667,  666,  665,  664,  663,  662,  662,
    661,  660,  659,  658,  657,  657,  656,  655,  654,  653,  652,  652,
    651,  650,  649,  648,  647,  647,  646,  645,  644,  643,  642,  642,
    641,  640,  639,  638,  637,  636,  636,  635,  634,  633,  632,  631,
    631,  630,  629,  628,  627,  626,  625,  625,  624,  623,  622,  621,
    620,  619,  619,  618,  617,  616,  615,  614,  613,  613,  612,  611,
    610,  609,  608,  607,  607,  606,  605,  604,  603,  602,  601,  600,
    600,  599,  598,  597,  596,  595,  594,  593,  593,  592,  591,  590,
    589,  588,  587,  586,  586,  585,  584,  583,  582,  581,  580,  579,
    578,  578,  577,  576,  575,  574,  573,  572,  571,  570,  570,  569,
    568,  567,  566,  565,  564,  563,  562,  562,  561,  560,  559,  558,
    557,  556,  555,  554,  553,  552,  552,  551,  550,  549,  548,  547,
    546,  545,  544,  543,  542,  542,  541,  540,  539,  538,  537,  536,
    535,  534,  533,  532,  531,  530,  530,  529,  528,  527,  526,  525,
    524,  523,  522,  521,  520,  519,  518,  517,  516,  516,  515,  514,
    513,  512,  511,  510,  509,  508,  507,  506,  505,  504,  503,  502,
    501,  500,  499,  499,  498,  497,  496,  495,  494,  493,  492,  491,
    490,  489,  488,  487,  486,  485,  484,  483,  482,  481,  480,  479,
    478,  477,  476,  475,  474,  473,  472,  471,  470,  469,  468,  467,
    466,  465,  465,  464,  463,  462,  461,  460,  459,  458,  457,  456,
    455,  454,  453,  452,  451,  450,  449,  448,  447,  446,  444,  443,
    442,  441,  440,  439,  438,  437,  436,  435,  434,  433,  432,  431,
    430,  429,  428,  427,  426,  425,  424,  423,  422,  421,  420,  419,
    418,  417,  416,  415,  414,  413,  412,  410,  409,  408,  407,  406,
    405,  404,  403,  402,  401,  400,  399,  398,  397,  396,  395,  393,
    392,  391,  390,  389,  388,  387,  386,  385,  384,  383,  382,  380,
    379,  378,  377,  376,  375,  374,  373,  372,  371,  369,  368,  367,
    366,  365,  364,  363,  362,  360,  359,  358,  357,  356,  355,  354,
    353,  351,  350,  349,  348,  347,  346,  345,  343,  342,  341,  340,
    339,  338,  337,  335,  334,  333,  332,  331,  329,  328,  327,  326,
    325,  324,  322,  321,  320,  319,  318,  316,  315,  314,  313,  312,
    310,  309,  308,  307,  306,  304,  303,  302,  301,  299,  298,  297,
    296,  294,  293,  292,  291,  289,  288,  287,  286,  284,  283,  282,
    281,  279,  278,  277,  276,  274,  273,  272,  270,  269,  268,  266,
    265,  264,  263,  261,  260,  259,  257,  256,  255,  253,  252,  251,
    249,  248,  246,  245,  244,  242,  241,  240,  238,  237,  236,  234,
    233,  231,  230,  229,  227,  226,  224,  223,  221,  220,  219,  217,
    216,  214,  213,  211,  210,  208,  207,  205,  204,  202,  201,  199,
    198,  196,  195,  193,  192,  190,  189,  187,  186,  184,  182,  181,
    179,  178,  176,  174,  173,  171,  170,  168,  166,  165,  163,  161,
    160,  158,  156,  154,  153,  151,  149,  147,  146,  144,  142,  140,
    139,  137,  135,  133,  131,  129,  127,  126,  124,  122,  120,  118,
    116,  114,  112,  110,  108,  106,  104,  102,  99,   97,   95,   93,
    91,   88,   86,   84,   81,   79,   77,   74,   72,   69,   67,   64,
    61,   59,   56,   53,   50,   47,   44,   40,   37,   33,   29,   25,
    21,   16,   10,   0};

keyboard_config_t keyboard_config;

//--------------------------------------------------------------------+
// Configuration Functions
//--------------------------------------------------------------------+

// Get the end of stack address from the linker script to load the boot flag
extern const uint32_t _estack;

void reboot_to_bootloader(void) {
  uint32_t *boot_flag = (uint32_t *)&_estack;

  // Set the boot flag to the magic value
  *boot_flag = BOOT_FLAG;
  NVIC_SystemReset();
}

void load_keyboard_config(void) {
  uint32_t magic = 0;
  uint16_t version = 0;

  // Load default configuration
  memcpy(&keyboard_config, &default_keyboard_config, sizeof(keyboard_config));

  // Read the magic and version from EEPROM
  eeprom_read(offsetof(keyboard_config_t, magic), (uint8_t *)&magic,
              sizeof(magic));
  eeprom_read(offsetof(keyboard_config_t, version), (uint8_t *)&version,
              sizeof(version));

  if (magic != FIRMWARE_MAGIC) {
    // Set default configuration
    save_keyboard_config();
  } else if (version != FIRMWARE_VERSION) {
    // FIXME: Handle version migration
    save_keyboard_config();
  } else {
    eeprom_read(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
  }
}

void save_keyboard_config(void) {
  eeprom_write(0, (uint8_t *)&keyboard_config, sizeof(keyboard_config));
}

void factory_reset(void) {
  memcpy(&keyboard_config, &default_keyboard_config, sizeof(keyboard_config));

  save_keyboard_config();
}

void save_key_switch_config(uint8_t profile, uint8_t key_index) {
  eeprom_write(
      offsetof(keyboard_config_t, key_switch_config) +
          ((uint32_t)profile * NUM_KEYS + key_index) *
              sizeof(key_switch_config_t),
      (uint8_t *)&keyboard_config.key_switch_config[profile][key_index],
      sizeof(keyboard_config.key_switch_config[profile][key_index]));
}

void save_keymap(uint8_t profile, uint8_t layer, uint8_t key_index) {
  eeprom_write(
      offsetof(keyboard_config_t, key_switch_config) +
          (((uint32_t)profile * NUM_LAYERS + layer) * NUM_KEYS + key_index) *
              sizeof(uint16_t),
      (uint8_t *)&keyboard_config.keymap[profile][layer][key_index],
      sizeof(keyboard_config.keymap[profile][layer][key_index]));
}

void set_nkro(bool nkro) {
  keyboard_config.nkro = nkro;

  // Save the new NKRO setting
  eeprom_write(offsetof(keyboard_config_t, nkro),
               (uint8_t *)&keyboard_config.nkro, sizeof(keyboard_config.nkro));
}

void set_switch_profile(uint8_t profile) {
  // Check if the profile is valid
  if (profile >= SWITCH_PROF_COUNT)
    return;

  keyboard_config.switch_profile = profile;

  // Save the new switch profile
  eeprom_write(offsetof(keyboard_config_t, switch_profile),
               (uint8_t *)&keyboard_config.switch_profile,
               sizeof(keyboard_config.switch_profile));
}

void set_keyboard_profile(uint8_t profile) {
  // Check if the profile is valid
  if (profile >= NUM_PROFILES)
    return;

  keyboard_config.keyboard_profile = profile;

  // Save the new keymap profile
  eeprom_write(offsetof(keyboard_config_t, keyboard_profile),
               (uint8_t *)&keyboard_config.keyboard_profile,
               sizeof(keyboard_config.keyboard_profile));
}
