#include "midi_message_buffer.h"
#include "string.h"

static inline uint8_t min(uint8_t a, uint8_t b) {
    return a < b ? a : b;
}

void init_message_buff(midi_msg_buffer_t * buf, uint8_t *data, uint8_t len) {
    buf->bytes_avail=0;
    buf->data = data;
    buf->len = len;
    buf->head = 0;
    buf->tail = 0;
}

int8_t get_next_message(midi_msg_buffer_t * buf, uint8_t *msg, uint8_t len) {
    if(buf->bytes_avail < len){
        return -1;
    }/*
    const uint8_t part1 = min(len, buf->len - buf->head);
    const uint8_t part2 = len - part1;

    memcpy(msg,         buf->data + buf->head, part1);
    memcpy(msg + part1, buf->data,           part2);

    buf->head = (buf->head + len) % buf->len;
    buf->bytes_avail -= len;*/
    memcpy(msg, buf->data + buf->head, len);
    buf->head = buf->head + len;
    if(buf->head>=buf->len) buf->head = 0;
    buf->bytes_avail -= len;
    return 0;
}
int8_t put_message(midi_msg_buffer_t * buf, uint8_t *msg, uint8_t len) {
    if(buf->len - buf->bytes_avail < len){
        return -1;
    }/*
    const uint8_t part1 = min(len, buf->len - buf->tail);
    const uint8_t part2 = len - part1;

    memcpy(buf->data + buf->tail, msg, part1);
    memcpy(buf->data, msg + part1, part2);

    buf->tail = (buf->tail + len) % buf->len;
    buf->bytes_avail += len;*/
    memcpy(buf->data + buf->tail, msg, len);
    buf->tail = buf->tail + len;
    if(buf->tail>=buf->len) buf->tail = 0;
    buf->bytes_avail += len;
    return 0;
}