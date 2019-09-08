import socket
import os
import sys
import xml_loader

HOST = "192.168.43.81" # The host's ipaddress
PORT = 5000 # The Port number

def main():
	sock = socket.socket()
	sock.bind((HOST, PORT))
	sock.listen(2)
	print("Listening for connections")
	conn, addr = sock.accept()
	print("{} is connected".format(addr))


def listening(conn):
	while True:
		data = conn.recv(1024)
		if not data:
			break
		if data == "close":
			conn.close()
			break
		value = execute(data)
		conn.send(bytes(value.endcode()))

def get_summary():
# This function will read the xml files for weather and air quality and format
# Them into a string like below
	return  "2,305,3"

def get_bus_information(stop_id):
# Here the we will be using the GTFS API to get transit information at the given
# Stop id
	return "423,3"

def execute(cmd):
# This function will execute the commands being passed by the CED
	if cmd == "update_summary":
		return get_summary()

	if cmd == "update_bus":
		return get_bus_information(cmd.split(" ")[1])
