#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>
#ifndef _Time_h
#include <TimeLib.h>
#endif

#ifndef NTP_H
#define NTP_H

#ifndef byte
#define byte char
#endif

class NTP {
	private:
		const unsigned long seventyYears = 2208988800UL;
		const int NTP_PACKET_SIZE = 48;
		byte _packetBuffer[48];
		IPAddress _timeServerIP;
		uint32_t _port;
		float _timezone;
		WiFiUDP udp;
		void send();
		void init(const char*, uint32_t);
	public:
		NTP();
		NTP(const char*);
		NTP(uint32_t);
		NTP(const char*, uint32_t);
		void setTimezone(float);
		void begin();
		time_t get();
		time_t get(float);
};
#endif
