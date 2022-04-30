#include <rtthread.h>
#include <stdio.h>
//#include <lvgl.h>
#define DBG_TAG    "LUAEOS.demo"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef LUA_EOS_THREAD_STACK_SIZE
#define LUA_EOS_THREAD_STACK_SIZE 4096
#endif

#ifndef LUA_EOS_THREAD_PRIO
#define LUA_EOS_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif

#include "log.h"
#include "shell.h"
#include "msh.h"

static int shell_in(char *cmd, rt_size_t length)
{
    printf("shell_in called\n");
    msh_exec(cmd, length);
    return 0;
}

static void lua_eos_thread(void *parameter)
{

    /* handle the tasks of LVGL */
    while(1)
    {
	    //printf("Hello from Lua-EOS\n");
        rt_thread_mdelay(3000);
    }
}

static int lua_eos_init(void)
{
    rt_thread_t tid;
    log_init();

    // LOG("\nNormal Log");
    // LOG_E("Error log");

    finsh_set_external_shell(&shell_in);
    finsh_system_init();

    tid = rt_thread_create( "LVGL", 
                            lua_eos_thread, 
                            RT_NULL, 
                            LUA_EOS_THREAD_STACK_SIZE, 
                            LUA_EOS_THREAD_PRIO, 
                            10);
    if(tid == RT_NULL)
    {
        LOG_E("Fail to create 'LVGL' thread");
    }
    rt_thread_startup(tid);

    return 0;
}

INIT_APP_EXPORT(lua_eos_init);

