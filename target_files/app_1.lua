
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

if EOS_PLATFORM == "SIMULATOR" then
  EOS_PATH = "../target_files/?.lua;"
else
  EOS_PATH = "/?.lua;"
end
package.path = EOS_PATH

glog = false

-- ----------- dome: new task demo ----------
function task_demo_1( ctx )  
  local c = 1
  while(1) do
    eos.delay(ctx, 2000)
    if glog == true then
      print("Child task c = " .. tostring(c) .. "\n")
    end
    c = c + 1
  end
end

-- -------------- Main App for simulator ------------
function app(ctx)
  
  eos.create_task(task_demo_1, "task_demo_1")

  local c = 1
  while(1) do
    eos.delay(ctx, 4000)
    if glog == true then
      print("App c = " .. tostring(c) .. "\n")
    end
    c = c + 1
  end

end

