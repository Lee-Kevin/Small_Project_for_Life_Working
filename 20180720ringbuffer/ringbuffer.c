/*
 * File      : ringbuffer.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-09-30     Bernard      first version.
 * 2013-05-08     Grissiom     reimplement
 */


#include <string.h>
#include "ringbuffer.h"

void ring_buffer_init(struct ring_buffer *rb,
                        uint8_t           *pool,
                        int16_t            size)
{
    assert(rb != NULL);
    assert(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = ALIGN_DOWN(size, ALIGN_SIZE);
}

/**
 * put a block of data into ring buffer
 */
uint32_t ring_buffer_put(struct ring_buffer *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    uint16_t size;

    assert(rb != NULL);

    /* whether has enough space */
	/* 查找是否还有空余空间 */
    size = ring_buffer_space_len(rb);

	// size = (rb)->buffer_size - ring_buffer_data_len(rb)
    /* no space */
    if (size == 0)
        return 0;

    /* drop some data */
	/* 空间不足以存放足够多的数据 */
    if (size < length)
        length = size;
	
	/* 判断是否需要进行循环覆盖 */
    if (rb->buffer_size - rb->write_index > length) 
    {
		/* 不需要进行循环覆盖 */
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;
		/* 返回成功写入的数据的长度 */
        return length;
    }
	/* 需要进行循环覆盖 */
    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
	/* 此处标记 代表已经循环了一圈 */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);
	/* 新的一圈中，write_index的值 */
    return length;
}

/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
uint32_t ring_buffer_put_force(struct ring_buffer *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    enum ring_buffer_state old_state;

    assert(rb != NULL);

    old_state = ring_buffer_status(rb);

    if (length > rb->buffer_size)
        length = rb->buffer_size;

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;

        if (old_state == RT_RINGBUFFER_FULL)
            rb->read_index = rb->write_index;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

	/* 如果以前状态是满的，新的状态则继续是满的 */
    if (old_state == RT_RINGBUFFER_FULL)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = rb->write_index;
    }

    return length;
}


/**
 *  get data from ring buffer
 */
uint32_t ring_buffer_get(struct ring_buffer *rb,
                            uint8_t           *ptr,
                            uint16_t           length)
{
    uint32_t size;

    assert(rb != NULL);

    /* whether has enough data  */
	/* 获取当前buffer中的数据长度 */
    size = ring_buffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

	/* 判断是否需要将ringbuffer 过循环一圈 */
    if (rb->buffer_size - rb->read_index > length)
    {
        /* copy all of data */
		/* 直接将需要的数据复制出来 */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length;
        return length;
    }

    memcpy(&ptr[0],
           &rb->buffer_ptr[rb->read_index],
           rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index],
           &rb->buffer_ptr[0],
           length - (rb->buffer_size - rb->read_index));

    /* we are going into the other side of the mirror */
	/* 用来标记已经读了一圈 */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = length - (rb->buffer_size - rb->read_index);

    return length;
}

/**
 * put a character into ring buffer
 */
uint32_t ring_buffer_putchar(struct ring_buffer *rb, const uint8_t ch)
{
    assert(rb != NULL);

    /* whether has enough space */
    if (!ring_buffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
uint32_t ring_buffer_putchar_force(struct ring_buffer *rb, const uint8_t ch)
{
    enum ring_buffer_state old_state;

    assert(rb != NULL);

    old_state = ring_buffer_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if (old_state == RT_RINGBUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        rb->write_index++;
        if (old_state == RT_RINGBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}

/**
 * get a character from a ringbuffer
 */
uint32_t ring_buffer_getchar(struct ring_buffer *rb, uint8_t *ch)
{
    assert(rb != NULL);

    /* ringbuffer is empty */
    if (!ring_buffer_data_len(rb))
        return 0;

    /* get character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}
