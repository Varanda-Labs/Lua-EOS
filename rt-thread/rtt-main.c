#include <rtthread.h>
#include <stdio.h>
//#include <lvgl.h>
#define DBG_TAG    "LUAEOS.demo"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef LUA_EOS_THREAD_STACK_SIZE
#define LUA_EOS_THREAD_STACK_SIZE 4096 * 10
#endif

#ifndef LUA_EOS_THREAD_PRIO
#define LUA_EOS_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif

#include <memory.h>
#include "log.h"
#include "finsh.h"
#include "shell.h"
#include "msh.h"
#include "lua_eos.h"

static int shell_in(char *cmd, rt_size_t length)
{
    printf("shell_in called\n");
    msh_exec(cmd, length);
    return 0;
}

static void lua_eos_thread(void *parameter)
{
    // run lua thread
    luaTask(NULL);

    /* handle the tasks of LVGL */
    while(1)
    {
	    //printf("Lua-EOS Terminated\n");
        rt_thread_mdelay(3000);
    }
}

void toConsole(char * msg)
{
    printf("%s", msg);
}

static int lua_eos_init(void)
{
    log_init();

    rt_thread_t tid;

    //finsh_set_external_shell(&shell_in);
    finsh_system_init();

    // start lvgl thread
    luaeos_lvgl_init();

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

#include "lua.h"
void lv_append_obj(void * obj)
{
//   lua_State * L = get_lua_state();
//   if ( ! L) {
//       return;
//   }

//   lua_getglobal(L, LV_OBJECTS);

//   if ( ! lua_istable(L, -1)) {
//     // create a table to track objects to forward events
//     lua_newtable(L);
//     lua_pushstring(L, "dummy_key");
//     lua_pushstring(L, "dummy_val");
//     lua_settable(L, -3);
//     lua_setglobal(L, LV_OBJECTS);
//     lua_getglobal(L, LV_OBJECTS); // to top again
//   }

//   lua_pushstring(L, "dummy");
//   lua_setfield( L, -2, obj );
}

char * get_line(bool echo)
{
    // LOG("writeDataFromTerm: '%s', len=%u", data.toStdString().c_str(), data.length());
    static char msg[SHELL_MAX_LINE_SIZE] = {0};
    int msg_len;
    int c_int;
    char c;

    msg_len = 0;

    while(1) {
        c_int = finsh_getchar();
        if (c_int < 0) {
            LOG_E("unexpected console input timeout");
            continue;
        }
        c = c_int & 0x000000ff;
        

        if (echo) {
            printf("%c", c);
        }

        // scan for native console switch pattern (3 scape characters):
        int esc_n = 0;
        char * ptr = msg;
        while(*ptr) {
            if (*ptr == SWITCH_SHELL_MODE_CHAR) {
                esc_n++;
                if (esc_n >= SWITCH_SHELL_MODE_COUNT) {
                    msg_len = 0;
                    memset(msg, 0, sizeof(msg));
                    //switchNativeShellMode();
                    return(NULL);
                }
            }
            else {
                esc_n = 0;
            }
            ptr++;
        }

        if (c == BACKSPACE_CHAR) { // note: 0x7f is for Mac
            msg[msg_len] = 0;
            if (msg_len > 1) {
                msg_len--;
            }
            continue;
        }

        //if (c == '\r') continue; // ignore CR
        msg[msg_len++] = c;
        if (c == '\n' || c == '\r') {
            msg[msg_len] = 0;
            return msg;
        }
        if (msg_len > (sizeof(msg) - 1) ) {
            LOG_W("line buffer overflow... ignoring line.");
            msg_len = 0;
        }
    }
    
}

#define LUA_PROMPT "lua> "

static void lua_shell_cmd(int argc, char**argv)
{
    printf ("\r\nLua EOS Shell version "  EOS_VERSION  "\r\nCopyrights 2021 Varanda Labs\r\n\r\n" LUA_PROMPT);

    while(true) {
        char* line = get_line(true);
        if (line == NULL) { /* Break on EOF or error */
            printf("\nLeaving Lua shell (Lua running on background)\n");
            return;
        }

        printf("\r\n");
        sendTextToConsoleController(line); // line_buf);
    }
}

MSH_CMD_EXPORT_ALIAS(lua_shell_cmd, luaeos, LuaEOS shell)
INIT_APP_EXPORT(lua_eos_init);

