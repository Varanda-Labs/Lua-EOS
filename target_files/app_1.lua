
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

-- require "lvgl"
-- require "lv_disp"
-- require "lv_obj"
-- require "lv_btn"
-- require "lv_checkbox"
-- require "lv_label"
-- require "lv_linemeter"
-- require "lv_slider"
-- require "lv_style"
-- require "lv_color"
-- require "lv_switch"
-- require "lv_keyboard"
-- require "lv_textarea"
-- require "lv_arc"
-- require "lv_bar"
-- require "lv_btnmatrix"
-- require "lv_calendar"


--------
global_demo = 0
glog = false
g_user_event_1 = nil


-- -------- Variables and callback for button 1 -----
btn_1 = nil
label_1 = nil
btn_1_cnt = 1

-- ----------- dome: new task demo ----------
function task_test_user_event_1( ctx )  
  local c = 1
  while(1) do
    eos.delay(ctx, 1000)
    print("Child task c = " .. tostring(c) .. "\n")
    c = c + 1
  end
end

-- -------------- Main App for simulator ------------
function app(ctx)
  print("task_test_user_event_1:")
  print(task_test_user_event_1)
  eos.create_task(task_test_user_event_1, "task_test_user_event_1")

  local c = 1
  while(1) do
    eos.delay(ctx, 5000)
    print("App c = " .. tostring(c) .. "\n")
    c = c + 1
  end

end

