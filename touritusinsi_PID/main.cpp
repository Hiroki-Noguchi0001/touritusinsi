#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#define ADV_TO_RAD 0.00568
#define PULSE_TO_METER 0.000501
<<<<<<< Updated upstream
#define KP 0.025
#define KI 0
#define KD 0
#define T 0.002
=======
#define KP 0.02
#define KI 0.0
#define KD 0

#define T 0.002
//#define LPF(x, x0, r)	((r) * (x - x0))
>>>>>>> Stashed changes

BusOut led(LED1, LED2, LED3, LED4);	//set LED
AnalogIn pen(AD7);									//Potentiometer Input Output
Ticker pen_control;								//speed control time
Serial pc(USBTX, USBRX);						//serial 

// motor object
Motor motor_right(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_PWM);		//left motor
Motor motor_left(MOTOR2_IN1, MOTOR2_IN2, MOTOR2_PWM);	//right motor

// GPIO object
BusIn in(GPIO1, GPIO2, GPIO3, GPIO4);

// encoder
QEI qei_left(GPIO1, GPIO2, NC, 48, QEI::X4_ENCODING);
QEI qei_right(GPIO3, GPIO4, NC, 48, QEI::X4_ENCODING);

<<<<<<< Updated upstream
//*************** car speed control ***************//

int adv;												// Now AD
int goal_pen_val = 485;							// goal AD
double speed, last_speed;						// car speed, car last speed
int pen_diff;										// difference
=======
//*************** car speed control ***************//
int adv;												// Now AD
int goal_pen_val = 488;					// goal AD
double speed, last_speed;						// car speed, car last speed
int pen_diff;										// difference
float TARGET_THETA;			//goal theta
float theta;					//now theta
float e;
float e0 = 0;
double ed;
float	ei;
float duty_ratio;
int stop = 1;
//double filter;
//double last_LPF = 0;
>>>>>>> Stashed changes

void pen_control_handler(){
	adv = pen.read_u16()>>6;
	adv /= 4;
	adv *= 4;
	//--------------------------------------
	//filter = LPF(adv, last_LPF, 0.4);
	//last_LPF = filter;
	//--------------------------------------
	
<<<<<<< Updated upstream
	float e0 = 0;
	adv = pen.read_u16()>>6;   
	float theta = adv * ADV_TO_RAD;
	float TARGET_THETA = goal_pen_val * ADV_TO_RAD;
	float e = TARGET_THETA - theta;
	float ed = (e - e0) / T;
	float ei = ei + e * T;
		e0 = e; 
	
		if(ei > 10000) ei = 10000;
		if(ei > -10000) ei = -10000;

=======
//	theta = adv * ADV_TO_RAD;
	//TARGET_THETA = goal_pen_val * ADV_TO_RAD;
	e0 = e;
	e = goal_pen_val - adv;
	ei += (e + e0) / 2.0 * T;
	ed = (e - e0) / T;
		if(ei > 1000) ei = 1000;
		if(ei <	-1000) ei = -1000;
	
>>>>>>> Stashed changes
//  Calculate PID control
	 duty_ratio = ((e * KP + ei * KI + ed * KD)*100); 
		
		if(adv <= 300 || adv >= 564){
			duty_ratio = 0;
		}
		
	
<<<<<<< Updated upstream
//  Introduce x, dx, theta, dtheta
	int left = qei_left.getPulses();
=======


	//  Introduce x, dx, theta, dtheta
	/*	int left = qei_left.getPulses();
>>>>>>> Stashed changes
	int right = qei_right.getPulses();
	float x0 = 0;
	float x = (left + right) / 2 * PULSE_TO_METER;
	float dx = (x - x0) / T;
	  x0 = x;
		theta = e;
	float dtheta = ed;
	float theta0 = theta;

//  Calculate state feedback control
//	duty_ratio = -(int)((x*K1 + dx*K2 + theta*K3 + dtheta*K4)*100);
<<<<<<< Updated upstream
	
=======


>>>>>>> Stashed changes
	if (duty_ratio > 100) duty_ratio = 100;
	else if (duty_ratio < -100) duty_ratio = -100;
		
		speed = duty_ratio;
			
			if(speed > 1.0) speed = 1.0;
			if(speed < -1.0) speed = -1.0;
			
				motor_left = speed;
				motor_right = speed;
}

int main(){
	//motor Max speed object
	motor_left.setMaxRatio(0.6);
	motor_right.setMaxRatio(0.6);
	pen_control.attach(&pen_control_handler, 0.001);		//speed control time
	led = 1;		//set LED
	wait(1.0);		//wait 1 second
	
	while(1) {		//loop
<<<<<<< Updated upstream
		printf("adv:%d speed:%2.2f \r\n", adv ,speed);
		printf("goal:%d", goal_pen_val);
=======
	printf("adv:%d speed:%2.2f \r\n", adv ,speed);
	//	printf("low_filter:%lf",filter);
	//	printf("goal:%lf, theta:%lf",TARGET_THETA, theta);
	//	printf("e:%f\r\n, ei:%f\r\n, ed:%lf \r\n",e, ei, ed);
		printf("duty:%f\r\n", duty_ratio);
		//printf("e0 : %lf\r\n", e0);
>>>>>>> Stashed changes
		wait(0.08);
	}
}
