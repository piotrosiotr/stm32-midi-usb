#ifndef SYSEX_H
#define SYSEX_H

#define SYSEX_HEADER    0xF0
#define SYSEX_MY_ID     0x7D

#define SYSEX_CMD_NONE  0x00
#define SYSEX_CMD_READ  0x01
#define SYSEX_CMD_WRITE 0x02

#define SYSEX_EOM       0xF7

#define SYSEX_FW_VER_A  0x00
#define SYSEX_BUTNUM_A  0x01
#define SYSEX_CHAN_A    0x02
#define SYSEX_BRIGHT_A  0x03
#define SYSEX_BUTTMO_A  0x04

#define SYSEX_BUT_PRESS         0x01
#define SYSEX_BUT_RELEASE       0x02
#define SYSEX_BUT_LOPRESS       0x03
#define SYSEX_BUT_LORELEASE     0x04
#define SYSEX_BUT_PRESS_CLR     0x05
#define SYSEX_BUT_RELEASE_CLR   0x06
#define SYSEX_BUT_LOPRESS_CLR   0x07

void sysex_parse(uint8_t *sysex);

#endif