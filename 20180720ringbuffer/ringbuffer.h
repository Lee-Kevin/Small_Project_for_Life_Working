#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H
#include <stdint.h>
#include <assert.h>

#define ALIGN_SIZE 4
#define  ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
/* ring buffer */
struct ring_buffer
{
    uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     *
     * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;
    uint16_t write_mirror : 1;
    uint16_t write_index : 15;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int16_t buffer_size;
};


/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void ring_buffer_init(struct ring_buffer *rb,
                        uint8_t           *pool,
                        int16_t            size);
uint32_t ring_buffer_put(struct ring_buffer *rb,
                            const uint8_t     *ptr,
                            uint16_t           length);
uint32_t ring_buffer_put_force(struct ring_buffer *rb,
                                  const uint8_t     *ptr,
                                  uint16_t           length);
uint32_t ring_buffer_putchar(struct ring_buffer *rb,
                                const uint8_t      ch);
uint32_t ring_buffer_putchar_force(struct ring_buffer *rb,
                                      const uint8_t      ch);
uint32_t ring_buffer_get(struct ring_buffer *rb,
                            uint8_t           *ptr,
                            uint16_t           length);
uint32_t ring_buffer_getchar(struct ring_buffer *rb, uint8_t *ch);

enum ring_buffer_state
{
    RT_RINGBUFFER_EMPTY,
    RT_RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RT_RINGBUFFER_HALFFULL,
};

static inline uint16_t ring_buffer_get_size(struct ring_buffer *rb)
{
    assert(rb != NULL);
    return rb->buffer_size;
}

static inline enum ring_buffer_state
ring_buffer_status(struct ring_buffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
		/* 如果mirror 一致，读指针和写指针也一致，说明为空， 否则为满 */
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;
        else
            return RT_RINGBUFFER_FULL;
    }
    return RT_RINGBUFFER_HALFFULL;
}

/** return the size of data in rb */
static inline uint16_t ring_buffer_data_len(struct ring_buffer *rb)
{
    switch (ring_buffer_status(rb))
    {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

/** return the size of empty space in rb */
#define ring_buffer_space_len(rb) ((rb)->buffer_size - ring_buffer_data_len(rb))


#endif 
