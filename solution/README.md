# Building the application

Building of the application is handled by `CMake`. The process follows
standard approach, but I will repeat the most important details below.

The code resides in the top-most folder, called `solution`. This is 
also where `CMakeLists.txt` resides.

Designated out-of-source folder is located below and is called `.build`.
There is a small `README.md` file there with basic `cmake` invocation
example.
As customary with `cmake` it allows to build the software using different
toolchains under different operating systems.

Even though core development of this program was done under Linux
(64-bit Ubuntu 18.04) it should be possible to build the program
also on Raspbian. The compiler needs to support C++17.

Specifically, I have used `g++` `7.5.0` and `cmake` `3.10.2`.
As far as I know Raspbian has compilers that are newer than `7.5`.

Also, there is a dependency to `boost`. Version used by me is `1.65`.
Other dependencied are already included in the source code bundle.
They include:

* `spdlog` (logging)
* `fmt` (printing and string operations)
* `clipp` (CLI parameters)

Once in the `.build` folder first you need to generate build system files.
On Linux these are native `Makefiles`.
Simply execute

`cmake -DCMAKE_BUILD_TYPE=Release ..`

then issue

`make -j`

and if you like you may install the program and the library by issuing

`make install`

Make sure, though, that you have privileges to install into default install paths. It is always possible to provide custom install prefixes, should one want that.

Successful build will create program executable in the `app` subfolder of `.build` directory. The program is called `main`.

The library, both static and dynamic one, are created in the `lib` subfolder of `.build` directory.

NOTE: please note that no code was copied from `https://github.com/Wiznet/wiznet-iot-shield-mbed-kr`.

## Extras
If one wishes extra compilation flags can be passed to the compiler:

`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -Werror -march=native" ..`

Such invocation will force the compiler to be more picky about warnings and treat warnings as errors.

To enable extra debugging messages for `boost::asio` macro `BOOST_ASIO_ENABLE_HANDLER_TRACKING` can be added:

`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-DBOOST_ASIO_ENABLE_HANDLER_TRACKING" ..`

# Usage

To see the program's synopsis simply execute `main` without arguments.

There are several commands supported. Specifying a command is mandatory.
Overal structure of the command line is:

`./main command --device <device to use> <command options> <common options>`

Implemented commands allow to execute provided examples. These are:

* `at_ok` - simple synchronous AT / OK scenario, useful for basic connectivity check.
* `ati` - synchronous `ATI` scenario. It demonstrates reception of more complex response.
* `at+cops` - asynchronous example that executes `AT+COPS=?` read AT command.
* `rcv_sms` - asynchronous example in which modem is configured to receive unsolicited notifications
about incoming SMSs (`+CMT`), and then waits fir such notification. Once received the notifiction is presented to the user. It contains basic SMS parameters, including SMS body.
* `send_sms` - asynchronous example which demontrates how to send outbound SMS. The user can pass
destination address `--da` and optional SMS text `--text`.

"common options" include options that control logging output level (debug, trace, info), and modem device parameters (baud rate, parity, etc.).

# Library
The library is built on top of `boost::asio` and tries to mimic approaches commonly found in `boost::asio` applications. I was strongly inspired by the design of `boost::beast` library for http and websockets.

Main namespace of the library is `Ringbeller`.

The library as such offers to types of interfaces: synchronous and asynchronous. Provided API
allows to send and receive AT commands by invoking `write`, `async_write`, `read`, and `async_read`
functions. There is also `async_write_text` which is used to send SMS text.

Note that availability of an asynchronous API might be of particular interest in terms of integration with higher level languages, e.g. for Node.js wrappers.

Provided api gives choice to use either `boost::system::error` or `boost::asio` exceptions to report error conditions.

There are separate types to represent request and response messages (`messages.hpp`).

# Practical guide to the application

First, you need to know what device corresponds to the attached modem.

First check `dmsg` output:

```
$ dmesg | grep tty
[   46.077438] usb 2-1: GSM modem (1-port) converter now attached to ttyUSB0
[   46.078503] usb 2-1: GSM modem (1-port) converter now attached to ttyUSB1
[   46.078615] usb 2-1: GSM modem (1-port) converter now attached to ttyUSB2
```

Usually there are several devices created. This is in line with Chapter 2 of Quectel "**LTE&5G Linux USB Driver User Guide**" (`https://forums.quectel.com/uploads/short-url/pSWY9JoNAkQ0p7rZLLAewQPXtf5.pdf`).

`/dev/ttyUSB0` will be probably device you will want to use.

Its state can be queried with `stty`:

```
$ stty -F /dev/ttyUSB0
speed 9600 baud; line = 0;
eof = ^A; min = 1; time = 0;
ignbrk -brkint -icrnl -imaxbel
-opost -onlcr
-isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
```

NOTE: remember to have proper access permissions to the device. This usually requires the user to be member of `tty` and maybe `dialout`.

From my experience default settings of the modem device are working fine and you don't need to configure the modem parameters using application options.

Basic connectivity to the device can be verified also with `socat`:

```
$ socat  - /dev/ttyUSB0,crnl
AT

OK
(more commands)
```

with which you can execute AT commands in the console.

## Audio capture

As such my application does not include audio capture and sending by its own. This can be achieved in simple manner outside in another terminal.

Audio setup with EC25 is described in Quectel's "**LTE Standard UAC Application Note**", Rev. LTE_Standard_UAC_Application_Note_V1.0
Date: 2019-10-24. (`https://www.quectel.com/UploadImage/Downlad/Quectel_LTE_Standard_UAC_Application_Note_V1.0.pdf`)

Section 4.2.2 describes steps to configure Linux kernel to have sound devices associated with voice calls handled by EC25.

Page 14 shows that the device will be called `/dev/snd` and that 8 kHz 16-bit linear PCM samples can be sent and read `tinyplay` and `tinycap` programs from `tinyalsa` repository on github.

With that, my application can be used to make and receive calls, but the voice itself can be handled with `tinyalsa` utilities.

Additional details about linux kernel configuration are also included in the above mentioned 
"**LTE&5G Linux USB Driver User Guide**".

