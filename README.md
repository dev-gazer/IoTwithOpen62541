# IoTwithOpen62541
Intro to IoT/home assistant using Open62541


## Contents
This project consists of an OPC UA server (using Open62541 C Stack) and a client (using Python AsyncUA, from FreeOPCUA Stack).

The server has bindings with an external breadboard for acquiring data such as temperature and humidity (using a DHT11 sensor), while the client is connected to a Telegram bot for reading the sensor data remotely.