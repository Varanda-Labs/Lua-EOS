/***************************************************************
 *
 *                 This code is part of LUA_EOS
 *
 * Copyrights 2021 - Varanda Labs Inc.
 *
 * License:
 *   Creative Commons: CC BY-NC-SA 4.0
 *   Attribution-NonCommercial-ShareAlike 4.0 International
 *   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
 * 
 *   Exceptions: The following functions are Apache 2.0:
 *
 *   Note: for purchasing a commercial license contact:
 *     m@varanda.ca
 *
 ***************************************************************
 */


/***************************************************************
 *
 * The following code in this file is not under Lua EOS License.
 *
 ***************************************************************
 */

// from file lv_port_disp.c
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     The first version
 */
#include <lvgl.h>
#include <drv_clcd.h>

#include <stdio.h>
#define L() printf("Line: %d\n", __LINE__)

#define USE_OLDER

#ifdef USE_OLDER
static lv_disp_buf_t disp_buf;

#else
/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;
#endif

static rt_device_t lcd_device = 0;
static struct rt_device_graphic_info info;

static lv_disp_drv_t disp_drv;  /*Descriptor of a display driver*/

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void lcd_fb_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t x;
    uint32_t y;
    uint32_t location = 0;

    /* 16 bit per pixel */
    lv_color16_t *fbp16 = (lv_color16_t *)info.framebuffer;

    for (y = area->y1; y <area->y2 + 1; y++)
    {
        for (x = area->x1; x <area->x2 + 1; x++)
        {
            location = x + y * info.width;
            fbp16[location].full = color_p->full;
            color_p++;
        }
    }

    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    rt_err_t result;
    lv_color_t *fbuf1, *fbuf2;

    lcd_device = rt_device_find("lcd");
    if (lcd_device == 0)
    {
        rt_kprintf("error!\n");
        return;
    }
    result = rt_device_open(lcd_device, 0);
    if (result != RT_EOK)
    {
        rt_kprintf("error!\n");
        return;
    }
    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        rt_kprintf("error!\n");
        /* get device information failed */
        return;
    }

    RT_ASSERT(info.bits_per_pixel == 8 || info.bits_per_pixel == 16 ||
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32);

    fbuf1 = rt_malloc(info.width * info.height * sizeof(lv_color_t));
    if (fbuf1 == RT_NULL)
    {
        rt_kprintf("Error: alloc disp buf fail\n");
        return;
    }

    fbuf2 = rt_malloc(info.width * info.height * sizeof(lv_color_t));
    if (fbuf2 == RT_NULL)
    {
        rt_kprintf("Error: alloc disp buf fail\n");
        rt_free(fbuf1);
        return;
    }

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
#ifdef USE_OLDER
    lv_disp_buf_init(&disp_buf, fbuf1, fbuf2, info.width * info.height);
#else
    lv_disp_draw_buf_init(&disp_buf, fbuf1, fbuf2, info.width * info.height);
#endif

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = info.width;
    disp_drv.ver_res = info.height;

#ifdef USE_OLDER
    disp_drv.buffer = &disp_buf;
#else
    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;
#endif

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = lcd_fb_flush;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

    printf("display init done!!!\n");
}

// from file lv_port_indev.c
#include <lvgl.h>
#include <stdbool.h>
#include <rtdevice.h>

#include <drv_clcd.h>

static lv_indev_state_t last_state = LV_INDEV_STATE_REL;
static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static void input_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = last_state;
}

void lv_port_indev_input(rt_int16_t x, rt_int16_t y, lv_indev_state_t state)
{
    last_state = state;
    last_x = x;
    last_y = y;
}

lv_indev_t * button_indev;

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = input_read;

    /*Register the driver in LVGL and save the created input device object*/
    button_indev = lv_indev_drv_register(&indev_drv);
}

// from lv_demo.c

#include <rtthread.h>
#include <lvgl.h>
#define DBG_TAG    "LVGL.demo"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef LV_THREAD_STACK_SIZE
#define LV_THREAD_STACK_SIZE 4096
#endif

#ifndef LV_THREAD_PRIO
#define LV_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif


static void lvgl_thread(void *parameter)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    // ----------------native test----------------
    lv_obj_t * label;
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), 0);
    lv_obj_set_pos(btn1, 30, 10);
    lv_obj_set_size(btn1, 120, 50);
    //lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    //lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40,0);
    label = lv_label_create(btn1, 0);
    lv_label_set_text(label, "Button");
    //--------------------------------------------

    /* handle the tasks of LVGL */
    while(1)
    {
        lv_task_handler();
        rt_thread_mdelay(1);
    }
}

extern int lcd_init(void);

int luaeos_lvgl_init(void)
{
    rt_thread_t tid;

    //lcd_init();

    tid = rt_thread_create("LVGL", lvgl_thread, RT_NULL, LV_THREAD_STACK_SIZE, LV_THREAD_PRIO, 10);
    if(tid == RT_NULL)
    {
        LOG_E("Fail to create 'LVGL' thread");
    }
    rt_thread_startup(tid);
    return 0;
}
