/*
 *   MacBook automatic keyboard brightness daemon
 *   Copyright 2011 Pau Oliva Fora <pof@eslack.org>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int get_light_sensor_value() {

	int fd;
	int i,n=0;
	char buf[10];
	char a_light[10];
	char *light_sensor="/sys/devices/platform/applesmc.768/light";

	/* read light sensor value */
	fd = open(light_sensor, O_RDONLY);
	if (fd < 0) {
		perror (light_sensor);
                fprintf (stderr, "Can't open %s\n",light_sensor);
		exit(1);
	}
        read(fd, buf, 9);
	buf[9]='\0';
	close(fd);

	/* convert light sensor string value to integer */
	for (i=0;i<sizeof(buf);i++) {
		if (buf[i]==',') break;
		if (buf[i]!='(') {
			a_light[n]=buf[i];
			n++;
		}
	}
	a_light[n]='\0';

	return atoi(a_light);
}

int calculate_keyboard_brightness_value(int light, int maxlight) {

	int brightness=0;
	
	if (light == 0 ) brightness=maxlight;
	else if (light < 8 ) brightness=maxlight*0.75;
	else if (light < 16 ) brightness=maxlight*0.66;
	else if (light < 32) brightness=maxlight*0.33;
	else if (light < 64) brightness=maxlight*0.16;
	else if (light < 128) brightness=maxlight*0.08;
	else if (light >= 128) brightness=0;

	return brightness;
}
