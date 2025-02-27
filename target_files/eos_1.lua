--/***************************************************************
-- *
-- *                 This code is part of LUA_EOS
-- *
-- * Copyrights 2021 - Varanda Labs Inc.
-- *
-- * License:
-- *   Creative Commons: CC BY-NC-SA 4.0
-- *   Attribution-NonCommercial-ShareAlike 4.0 International
-- *   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
-- *
-- *   Note: for purchasing a commercial license contact:
-- *     m@varanda.ca
-- *
-- ***************************************************************
-- */

-- http://lua-users.org/wiki/LuaStyleGuide
-- check optimization: https://github.com/pallene-lang/pallene

function print(txt)
  local m
  if type(txt) == 'string' then
    m = txt
  else
    m = tostring(txt)
  end
  eos_print(m)
  eos_print("\n")
end

function print_sl(m)
  eos_print(m)
end
--print = eos_print

function show(t)
  for k,v in pairs(t) do
    print(tostring(k).. ": " .. tostring(v))
    if type(v) == 'table' then
      show(v) -- recursive
    end
  end 
end

function _eos_delay(t)
 local x = 0
 local y = 0
 while (x < 100000) do
   x = x + 1
   while (y < t * 50000) do
     y = y + 1
   end
 end
end

local function getfenv(fn)
  local i = 1
  while true do
    local name, val = debug.getupvalue(fn, i)
    if name == "_ENV" then
      return val
    elseif not name then
      break
    end
    i = i + 1
  end
end

local function setfenv(fn, env)
  local i = 1
  while true do
    local name = debug.getupvalue(fn, i)
    if name == "_ENV" then
      debug.upvaluejoin(fn, i, (function()
        return env
      end), 1)
      break
    elseif not name then
      break
    end

    i = i + 1
  end

  return fn
end


eos = {}
local tasks = {}

local EOS_PATH = "../lua_eos/?.lua;"
package.path = EOS_PATH

-- the following id's must match with sys_events_t enumeration
EV_SYS_START_UP =           {id = 1,    name = "EV_SYS_START_UP",             pri = false}
EV_SYS_SHUT_DOWN =          {id = 2,    name = "EV_SYS_SHUT_DOWN",            pri = false}
EV_SYS_TIMER =              {id = 3,    name = "EV_SYS_TIMER",                pri = false}
EV_SYS_TEXT_FROM_CONSOLE =  {id = 4,    name = "EV_SYS_TEXT_FROM_CONSOLE",    pri = false}
EV_SYS_SPLASH_DONE =        {id = 5,    name = "EV_SYS_SPLASH_DONE",          pri = false}
EV_SYS_USER_DEF =           {id = 6,    name = "EV_SYS_USER_DEF",             pri = false}
EV_SYS_LVGL =               {id = 7,    name = "EV_SYS_LVGL",                 pri = false}



local events_list = {
  EV_SYS_START_UP, 
  EV_SYS_SHUT_DOWN,
  EV_SYS_TIMER,
  -- timer is a special case... no need registration
  EV_SYS_TEXT_FROM_CONSOLE,
  EV_SYS_SPLASH_DONE,
  EV_SYS_USER_DEF,
  EV_SYS_LVGL
}

local user_event = 1000
local task_id_cnt = 1

local ST_START = 0
local ST_RUN = 1
local ST_WAIT_DELAY = 2
local ST_WAIT_EVENT = 3
local ST_ERROR = 4

-- ----------------------------
--
--      S C H E D U L E R
--
-- ----------------------------

local function scheduler()
  local success
  local args
  local nat_ev
  
  while(1) do
    nat_ev = eos_read_event_table()
    
    -- for each task load its ev_q if got event and if subscribed for it
    for k,task in pairs(tasks) do
      if nat_ev ~= nil then
        for kk,ev in pairs(nat_ev) do
          -- check if timer 
          if ev.ev_id == EV_SYS_TIMER.id and ev.task_id == task.task_id and ev.timer_id == 0 then
            if ev.timer_id == 0 then
              task.state = ST_RUN
            end
          elseif ev.ev_id == EV_SYS_USER_DEF.id then
            -- ignore... this event is only used to wake up the thread
          else
            for kkk, sub in pairs(task.subscription) do
--              print("check for " .. task.name)
--              print("  sub.name = " .. sub.name )
--              print("  sub.id = " .. sub.id )
              
              if sub.id == ev.ev_id then
                --print("********* add event to task " .. task.name)
                -- TODO: add exception for timer as task id must also match
                local arg
                if ev.ev_id == EV_SYS_TIMER.id then
                  arg = ev.timer_id
                else
                  arg = ev.arg
                end
                table.insert(task.ev_q, {event=sub, arg=arg})
              end
            end
          end  
        end
      end
    end
    
    -- loop while pendinf event in queued
    -- process each task
    for k,task in pairs(tasks) do
      success = true
      if task.state == ST_START or task.state == ST_RUN then
        success, args = coroutine.resume(task.co, task, "ID", "ARGS")

      elseif task.state ==  ST_WAIT_EVENT then
        while task.state ==  ST_WAIT_EVENT and #task.ev_q > 0 do
          local e = table.remove(task.ev_q, 1)

--          print("show event from Q:")
--          show(e)
--          print("--------------------")
          
          success, args = coroutine.resume(task.co, task, e.event, e.arg)
        end
      elseif task.state ==  ST_WAIT_DELAY then
        -- do nothing
      elseif task.state ==  ST_ERROR then
        -- TODO: remove task from the list
      else
        print("unexpected e.state = " .. task.state)
        return
      end

      if success == false then
        print("Error: " .. args)
        return
      end
    end
      
    
    -----------------------------------------------
    
  end
end

function eos.delay(ctx, time)
  eos_set_timer(ctx.task_id, 0, time)
  ctx.state = ST_WAIT_DELAY
  local c, e, arg = coroutine.yield()
end

function eos.yield(ctx)
  eos_set_timer(ctx.task_id, 0, 0)
  ctx.state = ST_WAIT_DELAY
  local c, e, arg = coroutine.yield()
end

function eos.create_user_event(name, high_priority)
  local p = false
  for k,v in pairs(events_list) do
    if v.name == name then
      return nil, "event name already taken: " .. name
    end
  end
  if high_priority ~= nil then
    p = high_priority
  end
  local ev = { id = user_event,
               name = name,
               pri = high_priority
  }
  user_event = user_event + 1
  table.insert(events_list, ev)
  return ev
end

function eos.post(event, arg)
  for k,task in pairs(tasks) do
    for kk, e in pairs(task.subscription) do
      if e == event then
        table.insert(task.ev_q, {event=event, arg=arg})
      end
    end
  end
--  local obj = { user_ev = event, user_arg = arg}
--  print("obj")
--  print(obj)
  eos_user_event()
end

function eos.subscribe_event(ctx, event)
  -- chech if event already subscribed
  for k,v in pairs(ctx.subscription) do
    if event.id == v.id then
      print("event already subscribed")
      return
    end
  end
  table.insert(ctx.subscription, event)
  -- print("Subscribed: " .. event.name)
end

function eos.subscribe_event_by_name(ctx, name)
  for k,v in pairs(events_list) do
    if name == v.name then
      eos.subscribe_event(ctx, v)
      return true
    end
  end
  return false, "could not find event " .. name
end


function eos.create_task(task_func, name)
  local e = {}
  e["task_id"] = task_id_cnt
  task_id_cnt = task_id_cnt + 1
  e["co"] = coroutine.create(task_func)
  e["name"] = name
  e["state"] = ST_START
  e["subscription"] = {}
  e["ev_q"] = {} -- entries have "ev_id" and "table_values"
  table.insert(tasks, e)
end

function eos.scheduler()
  scheduler()
end

function eos.wait_event(ctx)
  ctx.state = ST_WAIT_EVENT
  local c, e, arg = coroutine.yield()
  return e,arg
end

function eos.error(ctx, msg)
  ctx.state = ST_ERROR
end

function lua_error_handler( err )
   print( "ERROR:", err )
end

LUA_PROMPT = "lua> "
LUA_PROMPT_MULTI = "lua>> "

function luashell( ctx )
  shell_ctx = ctx -- global
  local f,msg, ok
  local chunk = ""
  local more = false
    
  -- print_sl ("Lua EOS Shell version " .. EOS_VERSION .. "\r\nCopyrights 2021 Varanda Labs\r\n\r\n" .. LUA_PROMPT)
  
  -- subscribe for events
  ok,msg = eos.subscribe_event_by_name(ctx, "EV_SYS_TEXT_FROM_CONSOLE")
  if ok == false then
    print("subscribe_event_by_name error")
    print(msg)
  end

  while(1) do
    local ev, arg = eos.wait_event(ctx)

    if more == true then
      chunk = chunk .. arg
      f, msg = load(chunk)
    else
      f, msg = load(arg)
    end
    if f == nil then
      if string.find(msg, "<eof>") ~= nil then
        if more == false then -- if first time:
          chunk = chunk .. arg
          more = true
        end
        print_sl(LUA_PROMPT_MULTI)
      else
        print("load: " .. msg)
        chunk = ""
        more = false
      end
    else
      --local ok, e = xpcall( f, lua_error_handler )
      local ok, msg = pcall(f)

      if ok == false then
        print(msg)
      end
      chunk = ""
      more = false
      print_sl(LUA_PROMPT)
    end
  end
  print("Exiting Shell thred")
end

local function launcher(ctx)
  eos.create_task(app, "app")
end


--eos.create_task(luashell, "luashell")
--eos.create_task(app, "app")

--eos.create_task(launcher, "launcher")
eos.create_task(luashell, "luashell")
eos.create_task(app, "app")
eos.post(EV_SYS_START_UP, "Starting up")
eos.scheduler()
-- while(1) do
--   print("Hello from Lua Interpreter\n")
--   _eos_delay(100);
-- end




