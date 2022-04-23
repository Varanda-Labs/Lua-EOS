from building import *

cwd  = GetCurrentDir()
CPPPATH = [cwd]

LOCAL_CFLAGS = ''
LOCAL_CFLAGS += ' -DLUA_EMBEDDED -DRT_THREAD'
LOCAL_CFLAGS += ' -I' + cwd + '/rt-thread'
LOCAL_CFLAGS += ' -Wno-unused-function'

print(cwd)

src = Glob("rt-thread/*.c") # + Glob("lua/src/*.c", exclude=['lua/src/luac.c'])
src += ['lua_eos/native/mos_rt-thread.c']

group = DefineGroup('lua-eos', src, depend = ['PKG_USING_LUA_EOS'], CPPPATH = CPPPATH, LOCAL_CFLAGS = LOCAL_CFLAGS)

Return('group')
