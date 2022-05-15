from building import *

cwd  = GetCurrentDir()
CPPPATH = [cwd]

LOCAL_CFLAGS = ''
LOCAL_CFLAGS += ' -DLUA_EMBEDDED -DRT_THREAD'
#LOCAL_CFLAGS += ' -DUSE_DOUG_LEA_MALLOC -DMSPACES -DLACKS_SYS_MMAN_H'
#LOCAL_CFLAGS += ' -DUSE_DL_PREFIX -DMSPACES=1 -DUSE_DOUG_LEA_MALLOC -DONLY_MSPACES=0 -DNO_MALLINFO=0 -DHAVE_MMAP=0'
LOCAL_CFLAGS += ' -I' + cwd + '.'
LOCAL_CFLAGS += ' -I' + cwd + '/rt-thread'
LOCAL_CFLAGS += ' -I' + cwd + '/lua_eos/native'
LOCAL_CFLAGS += ' -I' + cwd + '/lua/src'
LOCAL_CFLAGS += ' -I' + cwd + 'utils'
LOCAL_CFLAGS += ' -I' + cwd + '/lvgl'
LOCAL_CFLAGS += ' -I' + cwd + '/lvgl_patch'
LOCAL_CFLAGS += ' -I' + cwd + '/home/mvaranda/rt-thread/bsp/qemu-vexpress-a9/packages/Lua_EOS-latest/rt-thread'
LOCAL_CFLAGS += ' -Wno-unused-function'
LOCAL_CFLAGS += ' -Wno-implicit-function-declaration'

print(cwd)

src = Glob("rt-thread/*.c") + Glob("lua/src/*.c", exclude=['lua/src/luac.c'])
src += ['lua_eos/native/mos_rt-thread.c']
src += ['lua_eos/native/lua_eos.c']
src += ['lua_eos/native/bidings.c']
src += ['lua_eos/native/nat_cmd.c']
src += ['../../applications/lcd_init.c']
src += Glob("lvgl/src/*/*.c")
#src += ['utils/malloc.c']

group = DefineGroup('lua-eos', src, depend = ['PKG_USING_LUA_EOS'], CPPPATH = CPPPATH, LOCAL_CFLAGS = LOCAL_CFLAGS)

Return('group')
