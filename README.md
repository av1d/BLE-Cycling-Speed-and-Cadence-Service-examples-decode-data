# Connect to BLE peripheral with CSC service then read and decode its data

![screenshot of Arduino IDE running the full example](https://raw.githubusercontent.com/av1d/Python-and-Arduino-BLE-Cycling-Speed-and-Cadence-Service-examples-decode-data/main/screenshot.png)

This seems to be an often requested task yet documentation and examples are nearly non-existent, so I took it upon myself to publish this in hope it'll help others.

This is a full Arduino example for connecting to a Bluetooth Low Energy periphal with Cycling Speed and Cadence Service (CSC) to obtain and decode its data from a bicycle cadence sensor. Also included is a Python function for decoding the data (not full implementation, but it will get you past the tough part).

The data fed to the function should be 5 octets (00 00 00 00 00) and represent 'Crank Revolutions' and 'Last Crank Event Time'. The data will be decoded and printed in human-readable format. The time is presented in milliseconds.

If you make something with this I'd like to see it if you feel like sharing.
