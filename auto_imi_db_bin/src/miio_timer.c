/**************************************************************************
* @ file    : miio_timer.c
* @ version : 1.0
* @ brief   : 
*   Time     |    Author       |       Modify content
* 2020.07.14    dongguoyang       Create.
*
* @Copyright (c) 2019 chuangmi inc.
***************************************************************************/

#include "miio_timer.h"
//#include <miio/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CLOCK_ID   CLOCK_REALTIME
#define TIMEOUT_MSG_QUEUE_COUNT    15

#define log_debug printf
#define log_err	  printf
#define log_info  printf

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({  \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define list_for_each(pos, head)    \
        for((pos) = (head)->next; (pos) != NULL; (pos) = (pos)->next)


static imi_node_t list_for_entry(imi_node_t head, event_timer_e timer_id)
{
    imi_node_t pos = NULL;

    list_for_each(pos, head) {
        if(pos->imi_timer.evp.sigev_value.sival_int == timer_id) {
            //pos = container_of(&(pos->imi_timer.evp), struct _imi_node_s, evp);
            return pos;
        }
    }

    log_err("[%s:%d] Not find [%d] timer id node", __FUNCTION__, __LINE__, timer_id);
    return NULL;
}

/*
** return :
**      -1: Can not find this timer or param error.
**      -2: Current timer is invalid, delete it (delete_designation_timer()).
**       0: Current timer is exist.
*/
int check_timer_status(imi_node_t head, event_timer_e timer_id)
{
    struct itimerspec curr_timer;
    imi_node_t imi_node = NULL;

    if(!head) {
        log_err("[%s:%d] Param head is null", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node = list_for_entry(head, timer_id);
    if(!imi_node) {
        log_err("[%s:%d] Can not find this event timer, timer id [%d]", __FUNCTION__, __LINE__, timer_id);
        return -1;
    }

    if(timer_gettime(imi_node->imi_timer.timer, &curr_timer) != 0) {
        log_err("[%s:%d] Get timer info error", __FUNCTION__, __LINE__);
        return -1;
    } else {
        log_debug("[%s:%d] Get timer info: surplus time [%lu], time interval [%lu]", 
                    __FUNCTION__, __LINE__, curr_timer.it_value.tv_sec, curr_timer.it_interval.tv_sec);
        if(curr_timer.it_value.tv_sec == 0 && curr_timer.it_interval.tv_sec == 0) {
            log_info("[%s:%d] Current timer is invalid, please delete it.", __FUNCTION__, __LINE__);
            return -2;
        }
    }

    return 0;
}

imi_node_t timer_init()
{
    imi_node_t head_list = (imi_node_t)malloc(sizeof(struct _imi_node_s));
    if(!head_list) {
        log_err("[%s:%d] head node malloc error", __FUNCTION__, __LINE__);
        return NULL;
    }

    bzero(head_list, sizeof(struct _imi_node_s));

    head_list->next = NULL;

    log_info("[%s:%d] Init head node success !!!", __FUNCTION__, __LINE__);

    return head_list;
}

static void __list_add(imi_node_t head, imi_node_t node)
{
    if(head == NULL || node == NULL) {
        log_err("[%s:%d] Head or node list is null", __FUNCTION__, __LINE__);
        return;
    }

    if(head->next == NULL) {
        head->next = node;
    } else {
        node->next = head->next;
        head->next = node;
    }
}

static int __create_event_timer(imi_node_t imi_node, event_timer_e timer_id, timer_mode_e mode, int sec, time_out_func_t time_out_func)
{
    if(!imi_node) {
        log_err("[%s:%d] Imi node is null, please init node", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node->imi_timer.evp.sigev_value.sival_ptr = &(imi_node->imi_timer.timer);
    imi_node->imi_timer.evp.sigev_value.sival_int = timer_id;
    imi_node->imi_timer.evp.sigev_notify = SIGEV_THREAD;
    imi_node->imi_timer.evp.sigev_notify_function = (void *)time_out_func;

    if(timer_create(CLOCK_ID, &(imi_node->imi_timer.evp), &(imi_node->imi_timer.timer))) {
        log_err("[%s:%d] Timer create func error", __FUNCTION__, __LINE__);
        return -1;
    }

    if(mode == TIMER_MODE_LOOP) {
        imi_node->imi_timer.its.it_interval.tv_sec = sec;
    } else if(mode == TIMER_MODE_SINGLE) {
        imi_node->imi_timer.its.it_interval.tv_sec = 0;
    }
    imi_node->imi_timer.its.it_interval.tv_nsec = 0;
    imi_node->imi_timer.its.it_value.tv_sec = sec;
    imi_node->imi_timer.its.it_value.tv_nsec = 0;

    if(timer_settime(imi_node->imi_timer.timer, 0, &(imi_node->imi_timer.its), NULL)) {
        log_err("[%s:%d] Timer set error.", __FUNCTION__, __LINE__);
        timer_delete(imi_node->imi_timer.timer);
        return -1;
    }

    return 0;
}

int create_event_timer(imi_node_t head, event_timer_e timer_id, timer_mode_e mode, int sec, time_out_func_t time_out_func)
{
    int ret = 0;
    if(!head) {
        log_err("[%s:%d] Param head is null", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = check_timer_status(head, timer_id);
    if(ret != -1) {
        log_err("[%s:%d] Can not create this timer, timer is exist", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node_t node = (imi_node_t)malloc(sizeof(struct _imi_node_s));
    if(!node) {
        log_err("[%s:%d] Malloc node error", __FUNCTION__, __LINE__);
        return -1;
    }

    bzero(node, sizeof(struct _imi_node_s));
    node->next = NULL;

    if(__create_event_timer(node, timer_id, mode, sec, time_out_func) != 0) {
        log_err("[%s:%d] Create event timer error, release this node", __FUNCTION__, __LINE__);
        if(node) free(node);
        return -1;
    }

    __list_add(head, node);

    log_info("[%s:%d] Create timer id [%d] success, timer sec [%d] !!!", __FUNCTION__, __LINE__, timer_id, sec);

    return 0;
}

int refresh_event_timer(imi_node_t head, event_timer_e timer_id, int sec)
{
    imi_node_t imi_node = NULL;

    if(!head) {
        log_err("[%s:%d] Param head is null", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node = list_for_entry(head, timer_id);
    if(!imi_node) {
        log_err("[%s:%d] Can not refresh this event timer, timer id [%d]", __FUNCTION__, __LINE__, timer_id);
        return -1;
    }

    imi_node->imi_timer.its.it_interval.tv_sec = sec;
    imi_node->imi_timer.its.it_interval.tv_nsec = 0;
    imi_node->imi_timer.its.it_value.tv_sec = sec;
    imi_node->imi_timer.its.it_value.tv_nsec = 0;

    if(timer_settime(imi_node->imi_timer.timer, 0, &(imi_node->imi_timer.its), NULL)) {
        log_err("[%s:%d] Timer set error, refresh fail ...", __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

static int delete_designation_node(imi_node_t head, event_timer_e timer_id)
{
    imi_node_t pos = NULL;
    imi_node_t prev = NULL;

    prev = head;
    list_for_each(pos, head) {
        if(pos->imi_timer.evp.sigev_value.sival_int == timer_id) {
            if(pos->next == NULL) {
                prev->next = NULL;
                free(pos);
                log_info("[%s:%d] Free timer id [%d] node success !!!", __FUNCTION__, __LINE__, timer_id);
                return 0;
            } else {
                prev->next = pos->next;
                pos->next = NULL;
                free(pos);
                log_info("[%s:%d] Free timer id [%d] node success !!!", __FUNCTION__, __LINE__, timer_id);
                return 0;
            }
        }
        prev = prev->next;
    }

    log_err("[%s:%d] Can not find this node, timer id [%d]", __FUNCTION__, __LINE__, timer_id);
    return -1;
}

/*
**   return 0 : success
*/
int delete_designation_timer(imi_node_t head, event_timer_e timer_id)
{
    imi_node_t imi_node = NULL;

    if(!head) {
        log_err("[%s:%d] Param head is null", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node = list_for_entry(head, timer_id);
    if(!imi_node) {
        log_err("[%s:%d] Can not find this event timer, timer id [%d]", __FUNCTION__, __LINE__, timer_id);
        return -1;
    }

    if(timer_delete(imi_node->imi_timer.timer) != 0) {
        log_err("[%s:%d] delete timer fail , timer id [%d]...", __FUNCTION__, __LINE__, timer_id);
    }

    return delete_designation_node(head, timer_id);
}


int delete_all_timer(imi_node_t head)
{
    if(!head) {
        log_err("[%s:%d] Param head is null", __FUNCTION__, __LINE__);
        return -1;
    }

    imi_node_t pos = NULL;
    imi_node_t prev = NULL;

    prev = head;
    //TO DO 释放的时候有问题，参考cloudv2
    list_for_each(pos, head) {
        timer_delete(pos->imi_timer.timer);

        if(pos->next == NULL) {
            log_debug("[%s:%d] Free timer id [%d] node success !!!", __FUNCTION__, __LINE__, pos->imi_timer.evp.sigev_value.sival_int);
            prev->next = NULL;
            free(pos);
        } else {
            log_debug("[%s:%d] Free timer id [%d] node success !!!", __FUNCTION__, __LINE__, pos->imi_timer.evp.sigev_value.sival_int);
            prev->next = pos->next;
            pos->next = NULL;
            free(pos);
        }
        prev = prev->next;
    }

    log_info("[%s:%d] Release all timer success !", __FUNCTION__, __LINE__);

    return 0;
}
