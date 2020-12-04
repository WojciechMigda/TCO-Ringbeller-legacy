# Validation

Steps describing modem and audio device setup are included in `README.md`.

This document shows details of validation scenarios.

NOTE: make sure that the link to the modem is flushed so that data that might not have been consumed to the end by the previous application execution is not sitting there stale.
This can be done by running `socat` as described in `README.md` and testing simple AT command.

## Packet capture

To capture packets on the usb device one needs to load `usbmod` module:

```
$ modprobe usbmon
```

Then it is possible to monitor data exchanged on the port with `tcpdump` or `wireshark`.

## Examples

SMS sending and reception examples take care of configuring SMS format and text encoding.
Voice call examples take care of AUC (PCM in/out) configuration.

### modem error 

E.g. if it is used by another client.

```
Executing AT/OK scenario
[2020-12-04 11:09:17.951] [error] Failed to open device /dev/ttyUSB0. Reason: Device or resource busy
DONE with status=-1
```

### AT/OK

Shows output at the default logging level:
```
./main at_ok --device /dev/ttyUSB0
Executing AT/OK scenario
Sent 4 bytes to the modem
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
DONE with status=0
```

Shows output at the `debug` logging level.
```
$ ./main at_ok --device /dev/ttyUSB0 --debug 
Executing AT/OK scenario
Sent 4 bytes to the modem
[2020-12-04 10:53:01.138] [debug] match_condition sz:0 range:[]
[2020-12-04 10:53:01.140] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
DONE with status=0
```

### ATI
Shows output at the `trace` logging level. Please note function entry and exit logs, as required in the problem specification.
```
$ 
Executing ATI scenario
[2020-12-04 11:08:16.242] [trace] Fn Enter: std::size_t Ringbeller::write(SyncWriteStream&, const Ringbeller::request<isSet, isXTest, isXRead, isXWrite, isXExec, Body>&, Ringbeller::ec_t&) [with SyncWriteStream = boost::asio::basic_serial_port<>; bool isSet = false; bool isXTest = false; bool isXRead = false; bool isXWrite = false; bool isXExec = false; Body = Ringbeller::string_body; std::size_t = long unsigned int; Ringbeller::ec_t = boost::system::error_code]
[2020-12-04 11:08:16.242] [trace] to_string serialized message to ATI
[2020-12-04 11:08:16.242] [trace] Fn Leave: std::size_t Ringbeller::write(SyncWriteStream&, const Ringbeller::request<isSet, isXTest, isXRead, isXWrite, isXExec, Body>&, Ringbeller::ec_t&) [with SyncWriteStream = boost::asio::basic_serial_port<>; bool isSet = false; bool isXTest = false; bool isXRead = false; bool isXWrite = false; bool isXExec = false; Body = Ringbeller::string_body; std::size_t = long unsigned int; Ringbeller::ec_t = boost::system::error_code]
Sent 5 bytes to the modem
[2020-12-04 11:08:16.242] [trace] Fn Enter: std::size_t Ringbeller::read(SyncReadStream&, DynamicBuffer&, Ringbeller::response<Body, Sequence>&, Ringbeller::ec_t&) [with SyncReadStream = boost::asio::basic_serial_port<>; DynamicBuffer = boost::asio::basic_streambuf<>; Body = Ringbeller::string_body; Sequence = Ringbeller::vector_sequence; std::size_t = long unsigned int; Ringbeller::ec_t = boost::system::error_code]
[2020-12-04 11:08:16.242] [debug] match_condition sz:0 range:[]
[2020-12-04 11:08:16.245] [debug] match_condition sz:117 range:[
Manufacturer: huawei
Model: E3131A
Revision: 21.157.41.00.78
IMEI: 8XredactedX6
+GCAP: +CGSM,+DS,+ES

OK
]
[2020-12-04 11:08:16.246] [trace] Fn Leave: std::size_t Ringbeller::read(SyncReadStream&, DynamicBuffer&, Ringbeller::response<Body, Sequence>&, Ringbeller::ec_t&) [with SyncReadStream = boost::asio::basic_serial_port<>; DynamicBuffer = boost::asio::basic_streambuf<>; Body = Ringbeller::string_body; Sequence = Ringbeller::vector_sequence; std::size_t = long unsigned int; Ringbeller::ec_t = boost::system::error_code]
Read 117 bytes from the modem
Response result code: OK
Response result text is empty
Response body: Manufacturer: huawei
Response body: Model: E3131A
Response body: Revision: 21.157.41.00.78
Response body: IMEI: 8XredactedX6
Response body: +GCAP: +CGSM,+DS,+ES
DONE with status=0
```

### Send SMS
```
$ ./main send-sms +48XredactedX --device /dev/ttyUSB0 --debug
Executing "Send SMS" scenario
SMS target: +48XredactedX
SMS text: Feel the bern!
Setting up SMS format as "text"
Sent 11 bytes to the modem
[2020-12-04 11:11:03.403] [debug] match_condition sz:0 range:[]
[2020-12-04 11:11:03.406] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
Setting up GSM encoding
Sent 15 bytes to the modem
[2020-12-04 11:11:03.406] [debug] match_condition sz:0 range:[]
[2020-12-04 11:11:03.409] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
Sending SMS
Sent 24 bytes to the modem
Waiting for prompt..
[2020-12-04 11:11:03.409] [debug] match_condition sz:0 range:[]
[2020-12-04 11:11:03.412] [debug] match_condition sz:4 range:[
> ]
Prompt received, sending SMS body
Sent 15 bytes to the modem
DONE with status=0

```

### Receive SMS
```
Executing "Receive SMS" scenario
Setting up SMS format as "text"
Sent 11 bytes to the modem
[2020-12-04 11:15:35.072] [debug] match_condition sz:0 range:[]
[2020-12-04 11:15:35.075] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
Setting up GSM encoding
Sent 15 bytes to the modem
[2020-12-04 11:15:35.075] [debug] match_condition sz:0 range:[]
[2020-12-04 11:15:35.078] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
Setting up reception of unsolicited SMS notifications (+CMT)
Sent 19 bytes to the modem
[2020-12-04 11:15:35.078] [debug] match_condition sz:0 range:[]
[2020-12-04 11:15:35.081] [debug] match_condition sz:6 range:[
OK
]
Read 6 bytes from the modem
Response result code: OK
Response result text is empty
Response body is empty
Waiting for incoming SMS notification (press Ctrl-C to terminate)
[2020-12-04 11:15:35.081] [debug] match_condition sz:0 range:[]
```
