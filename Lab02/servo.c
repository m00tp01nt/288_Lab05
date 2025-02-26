#include <stdio.h>
#include<string.h>

#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "messages.h"

#include "servo.h"

//#define objectDistLimit 50; //Distance to detect object

void scanBetween(cyBOT_Scan_t* scanner, int left, int right, int increment) {
	/* Values from 0 to 180, so set initial width to 181.
	if nothing is detected, there was no smaller object,
	therefore there was no object. */
	int minWidth = 181;

	//track the angles detected
	int minAngle = 0;
	int startAngle = 0;
	int endAngle = 0;

	//track objects
	int objectDetected = 0; //false, no object being tracked
	int objectCounter = 0;

	int radialWidth;
	int engAngle;

	float distance;

	char message[50];
	int i;

	for (i = right; i <= left; i += increment) {
		cyBOT_Scan(i, scanner);
		distance = scanner->sound_dist;


		// Object detected
		if (distance < 50) {
			if (objectDetected == 0) { //mark start of new object
				startAngle = i;
				objectDetected = 1; //now set to true
			}
		}
		else {
			if (objectDetected == 1) { //when an object ends
				endAngle = i - increment; //since i is after object ends
				radialWidth = engAngle - startAngle;

				objectCounter++;
				//to track only required values??

				sprintf(message, "Object %d ; Width - %d ; Raw - %f\r\n\r\n", objectCounter, radialWidth, scanner->sound_dist);
				sendMessage(message); //message to PuTTY

				//Comparing and storing radial width and angle
				if (radialWidth < minWidth) {
					minWidth = radialWidth;
					minAngle = (endAngle - startAngle) / 2; //minAngle now stores the actual location i hope
				}

				objectDetected = 0; //reset
			}
		}
	}

	//if objects were detected
	if (objectDetected != 0) {
		cyBOT_Scan(minAngle, scanner);
	}
	//if not
	else {
		sprintf(message, "No object detected");
		sendMessage(message);
	}
}
