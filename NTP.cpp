#include "NTP.h"

NTP::NTP() {
	init("time.nist.gov", 2390);
}
NTP::NTP(uint32_t port) {
	init("time.nist.gov", port);
}
NTP::NTP(const char* serverName) {
	init(serverName, 2390); 
}
NTP::NTP(const char* serverName, uint32_t port) {
	init(serverName, port); 
}

void NTP::init(const char* serverName, uint32_t port) {
	WiFi.hostByName("time.nist.gov", _timeServerIP);
	_port = port;
}

void NTP::send() {
	// set all bytes in the buffer to 0
	memset(_packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	_packetBuffer[1] = 0;     // Stratum, or type of clock
	_packetBuffer[2] = 6;     // Polling Interval
	_packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	_packetBuffer[12]  = 49;
	_packetBuffer[13]  = 0x4E;
	_packetBuffer[14]  = 49;
	_packetBuffer[15]  = 52;
	
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(_timeServerIP, 123); //NTP requests are to port 123
	udp.write(_packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}

void NTP::setTimezone(float timezone) {
	_timezone = timezone;
}

time_t NTP::get() {
	return get(_timezone);
}
time_t NTP::get(float timezone) {
	while (udp.parsePacket());
	send();
	uint32_t start =  millis();
	while (millis()-start < 1500) {
		if (udp.parsePacket() >= NTP_PACKET_SIZE) {
			udp.read(_packetBuffer, NTP_PACKET_SIZE);
			unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
			unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);
			unsigned long secsSince1900 = highWord << 16 | lowWord;
			return secsSince1900 - seventyYears + timezone * SECS_PER_HOUR;
		}
		yield();
	}
	return 0;
}

void NTP::begin() {
	udp.begin(_port);
	send();
}
