#ifndef IR_BLASTER_RMT_TYPEDEF_H
#define IR_BLASTER_RMT_TYPEDEF_H

// Wave Option
#define LOW 1
#define HIGH 2

// Db Option
#define REP 0x01
#define NAV 0x02

#define PULSE_SIZE 5
#define PULSE_SEQ 2

#define DATA_MAP_SIZE 32

typedef struct {
    unsigned char High;
    unsigned char Low;
} FREQ;

typedef struct {
    unsigned char Pol;
    unsigned short Time;
} PULSE;

typedef struct {
    unsigned char Type;
    unsigned char Pulse;
    unsigned char Array;
    unsigned char Size;
} DATA_MAP;

typedef struct {
    FREQ Freq;
    PULSE Pulse[PULSE_SIZE][PULSE_SEQ];
} WAVE_FORM;

typedef struct {
    DATA_MAP DataMap[DATA_MAP_SIZE];
} DATA_FORM;

#endif // IR_BLASTER_RMT_TYPEDEF_H
