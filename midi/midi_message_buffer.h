#ifndef MIDI_MESSAGE_BUFFER_H
#define MIDI_MESSAGE_BUFFER_H
#include "stdint.h"

typedef struct _midi_msg_buffer{
    uint8_t len;
    uint8_t *data;
    uint8_t head;
    uint8_t tail;
    uint8_t bytes_avail;
} midi_msg_buffer_t;

void init_message_buff(midi_msg_buffer_t * buf, uint8_t *data, uint8_t len);
int8_t get_next_message(midi_msg_buffer_t * buf, uint8_t *msg, uint8_t len);
int8_t put_message(midi_msg_buffer_t * buf, uint8_t *msg, uint8_t len);

#endif