#ifndef IR_BLASTER_RMT_DB_TV_H
#define IR_BLASTER_RMT_DB_TV_H

#include "rmt_typedef.h"

#define SAMSUNGTV 0x00
#define LGTV 0x01

#define LC8C8D8DB8E 0x00
#define LC8CB8D8DB8E 0x01

#define NONE 0x00
#define LEADER 0x10
#define CUSTOM1 0x20
#define CUSTOM2 0x20
#define DATA 0x20
#define POWER 0x20
#define VOL_UP 0x20
#define VOL_DOWN 0x20
#define SOURCE 0x20
#define NAVI_UP 0x20
#define NAVI_DOWM 0x20
#define NAVI_LEFT 0x20
#define NAVI_RIGHT 0x20
#define NAVI_OK 0x20
#define CH_UP 0x20
#define CH_DOWN 0x20
#define MENU 0x20
#define RETURN 0x20
#define HOME 0x20
#define COMMAND 0x20
#define KEYPRESS 0x20
#define SEQUENCE 0x20
#define VERIFY 0x20
#define VERIFY1 0x20
#define DEFAULT0 0x20
#define DEFAULT1 0x80
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
    unsigned char Custom1;
    unsigned char Custom2;
    unsigned char Key[15];
    unsigned char Default[15];
    unsigned char Command;
    unsigned char Keypress;
    unsigned char Sequence;
    unsigned char Verify;
    unsigned char Verify1;
    unsigned char RepMsk;
} TV_DB_FORM;

extern TV_DB_FORM TV_DbForm[];
extern WAVE_FORM TV_WaveForm[];
extern DATA_FORM TV_DataForm[];

extern unsigned int tv_db_size;

#endif // IR_BLASTER_RMT_DB_TV_H