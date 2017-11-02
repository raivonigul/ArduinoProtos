/*
* esp_3rd_watchdog_01.ino
*/

//extern "C" {
//#include "user_interface.h"
//}

int getBootDevice(void) {
	int bootmode;
	asm(
		"movi %0, 0x60000200\n\t"
		"l32i %0, %0, 0x118\n\t"
		: "+r" (bootmode) /* Output */
		: /* Inputs (none) */
		: "memory" /* Clobbered */
	);
	return ((bootmode >> 0x10) & 0x7);
}

void setup() {

	Serial.begin(115200);
	Serial.printf("\n\nReason for reboot: %s\n", ESP.getResetReason().c_str());
	Serial.println("----------------------------------------------");

	if (getBootDevice() == 1) {
		Serial.println("\nThis sketch has just been uploaded over the UART.");
		Serial.println("The ESP8266 will freeze on the first restart.");
		Serial.println("Press the reset button or power cycle the ESP now");
		Serial.println("and operation will be resumed thereafter.");
		while (1) { yield(); }
	}
}

char user_input() {
	while (1) {
		Serial.println();
		Serial.println("This is a test of the ESP watchdogs. In the top text box enter a number (1, 2, or 3)");
		Serial.println("  1 - to go endless loop that does not feed both ESP watchdogs");
		Serial.println("  2 - to go endless loop that does not feed the software ESP watchdog");
		Serial.println("  3 - to go endless loop that does feed ESP watchdog");
		Serial.println("and then press the Enter key or click on the [Send] button");
		Serial.println();
		while (!Serial.available()) {
			delay(1);
		}
		char ch = Serial.read();
		if (ch >= '1' && ch <= '3') {
			return ch;
		}
		else if (ch != '\n') {
			Serial.println("\nInvalid entry");
		}
	}
}

void loop() {
	switch (user_input()) {
	case '1':
		Serial.println("\n\nStarting endless loop without feeding both ESP watchdogs");
		Serial.println("The software watchdog should timeout after 3 seconds and reset the ESP...");
		while (1) {} // software watchdog timeout

		break;
	case '2':
		Serial.println("\n\nStarting endless loop without feeding the software ESP watchdogs");
		Serial.println("The hardware watchdog should timeout after 7 seconds and reset the ESP...");
		ESP.wdtDisable();
		while (1) {} // hardware watchdog timeout
		break;
	case '3':
		Serial.println("\n\nStarting endless loop feeding ESP watchdogs");
		Serial.println("Wait at least 10 seconds to verify that the watchdogs will not catch");
		Serial.println("the problem and then press the reset button to break out of this loop!");
		while (1) { delay(2); };
		break;
	defaut:
		delay(1000);
		break;
	}
}