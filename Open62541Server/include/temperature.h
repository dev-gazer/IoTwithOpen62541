#include <wiringPi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define DHTPIN		0


double read_dht11_dat();
double get_temperature();