#ifndef IR_BLASTER_RMT_DB_AC_C
#define IR_BLASTER_RMT_DB_AC_C

#include "rmt_db_ac.h"
#include "rmt_ac.h"

AC_DB_FORM AC_DbForm[] = {
    {1000,
     SHARP67AC,
     SHARP67AC_DM,
     0,
     {0x01, 0x00, 0x01, 0x04, 0x02, 0x00, 0x01, 0x02, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xff, 0xff},
     {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e},
     {0x00, 0x02, 0x50, 0x02, 0x02, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
     0xff,
     0x00,
     SHARP67AC,
     0xff,
     0xff},

    {1010,
     PANASONIC152AC,
     PANASONIC152AC_DM,
     0,
     {0x09, 0x08, 0x03, 0x04, 0x02, 0x00, 0x03, 0x05, 0x07, 0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x02, 0xff, 0xff},
     {0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3C},
     {0x02, 0x20, 0xe0, 0x04, 0x00, 0x00, 0x80, 0x00, 0x0e, 0xe0, 0x00, 0x81, 0x00, 0xff, 0xff},
     0xff,
     0xff,
     PANASONIC152AC,
     0xff,
     0xff},
};

WAVE_FORM AC_WaveForm[] = {
    // SHARP67AC
    {{70, 140},
     {{{HIGH, 665}, {LOW, 540}},
      {{HIGH, 665}, {LOW, 1620}},
      {{HIGH, 9000}, {LOW, 4460}},
      {{HIGH, 665}, {LOW, 19960}},
      {{LOW, 0}, {LOW, 0}}}},

    // PANASONIC152AC
    {{70, 140},
     {{{HIGH, 469}, {LOW, 419}},
      {{HIGH, 483}, {LOW, 1252}},
      {{HIGH, 3531}, {LOW, 1712}},
      {{LOW, 0}, {LOW, 0}},
      {{LOW, 0}, {LOW, 0}}}},
};

DATA_FORM AC_DataForm[] = {
    // SHARP67AC_DM
    {{{LEADER, 2, 0, 0},
      {MODE, 0, 4, 3},
      {POWER, 0, 3, 1},
      {FAN, 0, 6, 2},
      {SWING, 0, 5, 2},
      {TEMP, 0, 7, 4},
      {DEFAULT1, 0, 8, 4},
      {DEFAULT1, 0, 8, 4},
      {DEFAULT2, 0, 9, 2},
      {POWER, 0, 3, 1},
      {DEFAULT1, 0, 8, 1},
      {DEFAULT3, 0, 10, 8},
      {DEFAULT4, 0, 11, 3},
      {LEADER, 3, 0, 0},
      {SWING, 0, 5, 2},
      {DEFAULT1, 0, 8, 2},
      {DEFAULT1, 0, 8, 8},
      {DEFAULT5, 0, 12, 8},
      {DEFAULT1, 0, 8, 8},
      {VERIFY1, 0, 18, 4},
      {END, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0}}},

    // PANASONIC152AC_DM
    {{{LEADER, 2, 0, 0},
      {DEFAULT1, 0, 8, 8},
      {DEFAULT2, 0, 9, 8},
      {DEFAULT3, 0, 10, 8},
      {DEFAULT4, 0, 11, 8},
      {DEFAULT5, 0, 12, 8},
      {POWER, 0, 3, 4},
      {MODE, 0, 4, 4},
      {TEMP, 0, 7, 6},
      {DEFAULT6, 0, 13, 2},
      {DEFAULT7, 0, 14, 8},
      {SWING, 0, 5, 4},
      {FAN, 0, 6, 4},
      {DEFAULT8, 0, 15, 16},
      {DEFAULT9, 0, 16, 8},
      {DEFAULTA, 0, 17, 8},
      {DEFAULTB, 0, 21, 16},
      {DEFAULTC, 0, 22, 8},
      {DEFAULTD, 0, 23, 16},
      {VERIFY1, 0, 18, 8},
      {END, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0},
      {NONE, 0, 0, 0}}},
};

unsigned int ac_db_size = sizeof(AC_DbForm) / sizeof(AC_DbForm[0]);

#endif // IR_BLASTER_RMT_DB_AC_C
