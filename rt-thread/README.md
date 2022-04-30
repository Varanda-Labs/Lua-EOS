# Building RT-Thread using Linux

## Preparation
We will add packages and fix path for python if necessary

### Install packages

```
sudo apt install -y gcc python3 python3-pip gcc-arm-none-eabi \
 binutils-arm-none-eabi scons libncurses5-dev qemu qemu-system-arm git
```

### Fix default python
Check if "python" command works (type exit() to close python).<br>
If worked then skip this item.<br>
Otherwise, perform the same check for python3.

If python3 does not work try:
```
sudo apt install -y python3
```
Assuming python3 is installed:

- Check the real location of python3 using "which python3" command:
```
which python3
```
- next, do the following (replacing /usr/bin by your path in case different):
```
sudo ln -s /usr/bin/python3 /usr/bin/python
```


## Clone Varanda's rt-thread repo
```
git clone -b lua-eos-01 https://github.com/mvaranda/rt-thread
cd rt-thread/bsp/qemu-vexpress-a9/
```

## While Lua EOS is not in the main rt-thread repository we will redirect it.
```
export RTT_PACKAGE_URL=https://github.com/Varanda-Labs/packages.git
```
### Removing existing ~/.env directory
we remove ~/.env as we are redirecting package URL:
```
rm -rf ~/.env
```

## Config RT-Thread
```
scons --menuconfig
```

goto: RT-Thread online packages-->language packages<br>
and select "Lua EOS is an embedded multi-task framework".<br>

Select "Save", confirm and select "exit a couple times.<br>
Then:
```
source ~/.env/env.sh
pkgs --update
scons
./qemu.sh
```

# Building RT-Thread using Mac
## Preparation
Install MacPorts from [here](https://www.macports.org/)
<br>Then:
```
sudo port install qemu
sudo port install scons
sudo port install ncurses
sudo port install arm-none-eabi-gcc
export PATH=$PATH:/opt/local/bin
git clone -b lua-eos-01 https://github.com/mvaranda/rt-thread
cd rt-thread/bsp/qemu-vexpress-a9/
```


edit file rtconfig.py
```
Change line:
  from:
    EXEC_PATH   = r'/usr/bin'
  to:
    EXEC_PATH   = r'/opt/local/bin'
```

## Add packages

To use packages, need python requests package.<br>

if no pip:
```
curl -O https://bootstrap.pypa.io/pip/2.7/get-pip.py
python get-pip.py
python -m pip install --upgrade "pip < 21.0"
python -m pip install requests
```

The next steps are the same than the above Linux's "Config RT-Thread"

