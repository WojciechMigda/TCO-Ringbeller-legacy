# Build and usage

For build and usage instructions please refer to 
`solutions/README.md`.

# Bugs

Yes, there are bugs.

First, I did not have access to the Quectel EC25 modem, and the only testing I could perform was done with Huawei E3131a data modem. This enabled me to only test basic connectivity and SMS sending scenarios.

For other scenarios I had to rely on available Quectel PDF manual on their AT commands and other information I could dig across the internet.

## Linking

It was pointed out by one of the reviewers that on his computer the linker was unable to produce the executable because of missing threading library.
While this problem was not visible on my setup, indeed, there's a bug where the compilation options for the library (`BOOST_ASIO_DISABLE_THREADS` macro)
are not propagated as one of the properties of the library target. Hence, the application itself may be compiled requiring threads.

## AT+USBCFG
One of the bugs that was uncovered during review phase testing is related how I handle response to the AT+USBCFG command. The way I understood the Quectel description of this command was that the result will be sent back as a +USBCFG URC (weird, but I went with it).

It turns out that the result is sent back just as with any other regular command. As a consequence, my implementation for making and receiving voice calls perpetually waits for +USBCFG URC which never arrives.

## RING
This bug is a simple mistake on my part. For handling of an incoming voice call I wanted to wait for a RING notification. Existing AT command response parser was ready for that and RING was one of final result codes which had their own designated enum.

However, in a haste just before completing the voice call functionality I added checking for a 'RING' string literal being received in the response body instead of checking the enum.

## ATE
It was pointed out by one of the reviewers that the board the solution was tested with had ATE echo enabled and this caused confusion for my solution.

## Installation

`make install` issued from the build folder will probably install the executable as well. It should only install the library. Also, cmake find module for the library is missing.

# Design improvements

While I am mostly satisfied with the design decisions I made, the parser does too much while preparing single response message.

Instead of relying on the parser preparing response message, which comprises of pieces such as body, result code, etc. it should send responses which correspond to single lines received from the modem: final result code (OK, RING, ERROR, etc.), the actual AT response, which also includes URCs (lines which start with + or ^), prompt (> ), and everything else.

Then the responsibility of combining these into the actual expectes response would fall onto the library client. Also, handling of URC would become simpler.

# clipp problems

While clipp is a very handy and powerful header-only library for CLI preparation and does a great job with simple cases, as I progressed I have stumbled upon bugs in clipp which resulted in my CLI not being able to properly handle passed valid input.

I was able to work around this by creating a monstrous combo in which each command is paired with the common options and only then summed to form the final CLI.
This causes the help message to be very verbose and full repetitions.
