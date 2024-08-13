#ifndef IR_BLASTER_CMD_H
#define IR_BLASTER_CMD_H

#define CMD_TYPE_RESET 1
#define CMD_TYPE_OTA 2
#define CMD_TYPE_RMT 3
#define CMD_TYPE_RMT_AUTO_ON 4
#define CMD_TYPE_RMT_AUTO_OFF 5

#include "context.h"

void handle_command(void *arg, char *cmd);

#endif // IR_BLASTER_CMD_H
