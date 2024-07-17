#ifndef IR_BLASTER_CMD_H
#define IR_BLASTER_CMD_H

#define CMD_TYPE_RESET 1
#define CMD_TYPE_OTA 2
#define CMD_TYPE_RMT 3

void handle_command(char *cmd);

#endif // IR_BLASTER_CMD_H
