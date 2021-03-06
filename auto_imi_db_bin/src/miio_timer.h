/**************************************************************************
* @ file    : miio_timer.h
* @ version : 1.0
* @ brief   : 
*   Time     |    Author       |       Modify content
* 2020.07.14    dongguoyang       Create.
*
* @Copyright (c) 2019 chuangmi inc.
***************************************************************************/

#ifndef __MIIO_TIMER_H__
#define __MIIO_TIMER_H__


#include <signal.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    SYNC_DATA_TIMEOUT,
} event_timer_e;

typedef enum {
    TIMER_MODE_LOOP,
    TIMER_MODE_SINGLE,
} timer_mode_e;

typedef void (*time_out_func_t)(event_timer_e timer_id);

typedef struct _imi_timer_s {
    struct sigevent evp;
    struct itimerspec its;
    timer_t timer;
} imi_timer_t;

typedef struct _imi_node_s {
    imi_timer_t imi_timer;
    struct _imi_node_s *next;
} *imi_node_t;

imi_node_t timer_init();
int create_event_timer(imi_node_t head, event_timer_e timer_id, timer_mode_e mode, int sec, time_out_func_t time_out_func);
int refresh_event_timer(imi_node_t head, event_timer_e timer_id, int sec);
int delete_designation_timer(imi_node_t head, event_timer_e timer_id);
int delete_all_timer(imi_node_t head);
int check_timer_status(imi_node_t head, event_timer_e timer_id);

#ifdef __cplusplus
}
#endif
#endif