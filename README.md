# Lua-EOS
Lua EOS is an embedded multi-task framework where the application layer can be written in Lua Scripts.
It provides event driven non-preemptive Lua tasks.
It is the same successful idea been used in many systems where Java VM provides a secure way for end users to
write applications without compromising the native implementation. Lua is very light weight and suitable for
embedded systems.  
The solution that Lua EOS delivers: product suppliers can securely provide programming capabilities to their end users without disclosing any native code (libraries). End user scripts run "out-of-the-box" from the file system. For example, supposed a company sells a weather station; The end user could write small scripts to take actions like turn fan on when temperature is above a value, send an email when a sensor is triggered, etc.

- RT-Thread Port:
Work is in progress. For details see [this](https://github.com/Varanda-Labs/Lua-EOS/tree/master/rt-thread) link

- ESP32 Port:

![ESP32 demo](./docs/images/lua_esp32_demo.png)

- Qt based Simulator

![Simulator demo](./docs/images/lua_sim_demo.png)
Details [here](./simulator)


## License
Creative Commons: CC BY-NC-SA 4.0  
Attribution-NonCommercial-ShareAlike 4.0 International  
[https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode](https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode)

## Video demo
#### [Early days](https://rumble.com/vodset-lua-eos-early-days-first-video-about-it.html)
#### [Nscreen32 Port (WIP May 09th, 2021)](https://rumble.com/vgsbhf-lua-eos-update.html)
#### [ESP32 Port (WIP April 15th, 2021)](https://rumble.com/vfp949-lua-eos-esp32-port.html)
#### [Update 1 (April 08th, 2021)](https://rumble.com/vfhdbt-lua-eos-april-08-2021-update.html)


