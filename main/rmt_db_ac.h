#ifndef IR_BLASTER_RMT_DB_AC_H
#define IR_BLASTER_RMT_DB_AC_H

#include "rmt_typedef.h"

// Waveform List
#define SHARP32AC_1 0x00
#define SHARP48AC_1 0x01
#define SHARP63AC_1 0x02
#define DAIKIN64AC_1 0x03
#define DAIKIN104AC_2 0x04
#define DAIKIN160AC_1 0x05
#define PANASONIC68AC_1 0x06
#define PANASONIC152AC_1 0x07
#define PANASONIC216AC_2 0x08
#define GREE67AC_1 0x09

// Dataform List
#define SHARP32AC_DM 0x00
#define SHARP48AC_DM 0x01
#define SHARP63AC_DM 0x02
#define DAIKIN64AC_DM_1 0x03
#define DAIKIN104AC_DM 0x04
#define DAIKIN160AC_DM_1 0x05
#define PANASONIC68AC_DM 0x06
#define PANASONIC152AC_DM_1 0x07
#define PANASONIC216AC_DM_1 0x08
#define GREE67AC_DM 0x09

// Checksum List
#define lson5_v1 0x00
#define daikin_v1 0x01
#define sob_v 0x02
#define sob8_v1 0x03
#define sob10_v1 0x04
#define gree_v 0x05

// Data Type Define...
#define NONE 0x00
#define LEADER 0x10
#define CUSTOM1 0x20
#define CUSTOM2 0x20
#define DATA 0x20
#define POWER 0x20
#define MODE 0x20
#define FAN 0x20
#define MC_FA 0x20
#define MC_FL 0x20
#define MC_FM 0x20
#define MC_FH 0x20
#define MW_FA 0x20
#define MW_FL 0x20
#define MW_FM 0x20
#define MW_FH 0x20
#define MD_FA 0x20
#define MD_FL 0x20
#define MD_FM 0x20
#define MD_FH 0x20
#define MA_FA 0x20
#define MA_FL 0x20
#define MA_FM 0x20
#define MA_FH 0x20
#define TEMP 0x20
#define SWING 0x20
#define COMMAND 0x20
#define KEYPRESS 0x20
#define VERIFY 0x20
#define VERIFY1 0x20
#define DEFAULT1 0x20
#define DEFAULT2 0x20
#define DEFAULT3 0x20
#define DEFAULT4 0x20
#define DEFAULT5 0x20
#define DEFAULT6 0x20
#define DEFAULT7 0x20
#define DEFAULT8 0x20
#define DEFAULT9 0x20
#define DEFAULTA 0x20
#define DEFAULTB 0x20
#define DEFAULTC 0x20
#define DEFAULTD 0x20
#define DEFAULTE 0x20
#define DEFAULTF 0x20
#define END 0x40
#define TOGGLE 0x80
#define BAR 0x01

typedef struct
{
    unsigned short Num;
    unsigned short WaveForm;
    unsigned short DataForm;
    unsigned char Option;
    unsigned char Data[31];
    unsigned char Temp[13];
    unsigned char Default[15];
    unsigned char Command;
    unsigned char Keypress;
    unsigned short Verify;
    unsigned short Verify1;
    unsigned char RepMsk;
} AC_DB_FORM;

extern const AC_DB_FORM AC_DbForm[];
extern const WAVE_FORM AC_WaveForm[];
extern const DATA_FORM AC_DataForm[];

extern unsigned int acdb_size;

#endif // IR_BLASTER_RMT_DB_AC_H
