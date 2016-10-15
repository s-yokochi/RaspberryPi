//gcc main.c -o main -lwiringPi
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//HC-SR04
#define    HC_SR04_TRIG_PIN    8
#define    HC_SR04_ECHO_PIN    7
int HC_SR04_init(void);
long HC_SR04_get_distance(void);
int HC_SR04_pulseIn(int pin, int level, int timeout);
/*****************************************************************
*
*****************************************************************/
int main (int argc, char **argv)
{
	int i,j,k,l,m;
	long distance;

	if(HC_SR04_init() == -1)return 1;	//HC-SR04 初期化

	for(i=0;i<200;i++){
		delayMicroseconds(100000); //wait
		distance = HC_SR04_get_distance();	//HC-SR04 距離取得

		printf("distance=%d cm\n",distance);
}

/*****************************************************************
*	HC-SR04 距離取得
*****************************************************************/
long HC_SR04_get_distance(void)
{
	long duration, distance;

	digitalWrite(HC_SR04_TRIG_PIN, LOW);
	delayMicroseconds(2);

	digitalWrite(HC_SR04_TRIG_PIN, HIGH);
	delayMicroseconds(10);

	digitalWrite(HC_SR04_TRIG_PIN, LOW);
	duration = HC_SR04_pulseIn(HC_SR04_ECHO_PIN, HIGH, 1000000);
//	printf("duration=%d\n",duration);
	distance = (duration/2) / 29.1;
//	printf("distance=%d cm\n",distance);

	return distance;
}
/*****************************************************************
*	HC-SR04 初期化
*****************************************************************/
int HC_SR04_init(void)
{
	if(wiringPiSetupGpio() == -1) return -1;
	pinMode(HC_SR04_TRIG_PIN,OUTPUT);
	pinMode(HC_SR04_ECHO_PIN,INPUT);

	return 0;
}
/*****************************************************************
*	HC-SR04 パルス波時間を取得する
*****************************************************************/
int HC_SR04_pulseIn(int pin, int level, int timeout)
{
	struct timeval tn, t0, t1;
	long micros;

	gettimeofday(&t0, NULL);
	micros = 0;
	while (digitalRead(pin) != level){
		gettimeofday(&tn, NULL);
		if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
		micros += (tn.tv_usec - t0.tv_usec);
		if (micros > timeout) return 0;
	}

	gettimeofday(&t1, NULL);
	while (digitalRead(pin) == level){
	gettimeofday(&tn, NULL);
		if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
		micros = micros + (tn.tv_usec - t0.tv_usec);
		if (micros > timeout) return 0;
	}

	if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
	micros = micros + (tn.tv_usec - t1.tv_usec);

	return micros;
}
