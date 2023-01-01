# LoRa_Wind
Wind Sensor with LoRa transmitter
Code is for the PIC18LF46K22 microcontroller.
Data is transmitted as 50 bytes to the base receiver (Raspberry pi) running WSP2.py code.
See the file "Sensor Data Formats New Formats from 18th Sept 2021 onwards.xlsx".
Wind speed is derived by counting pulses, 2 per rotation assumed from sensor.
Gust is calculated by storing pulse count every 2 seconds.  After one minute, the highest count is stored for transmission as the gust speed.
The average speed is transmitted as the total pulse count for 1 minute.
All calibration is done at the receiver (Raspberry pi).
This module is powered from 2 x C size batteries which should last a good 2 to 3 years.
Make sure the battery housing is in an accessible location for battery change.
Alkaline batteries are recommended due to the wider temperature range of operation.
STL files are provided for the battery/transmitter enclosure.
