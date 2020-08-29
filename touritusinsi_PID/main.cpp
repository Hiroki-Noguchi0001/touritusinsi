#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#define ADV_TO_RAD 0.00568
#define PULSE_TO_METER 0.000501
#define KP 0.18 // 0.34
#define KI 0.05
#define KD 0.0
#define T 0.001

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

//*************** car speed control ***************//
int adv;												// Now AD
int goal_pen_val = 120;					// goal AD
double speed, last_speed;						// car speed, car last speed
int pen_diff;										// difference
float TARGET_THETA;			//goal theta
float theta;					//now theta
float e;
float e0 = 0;
double ed;
float	ei;
float duty_ratio;

void pen_control_handler(){
	adv = pen.read_u16()>>6;
	adv /= 2;
	adv *= 2;
	//--------------------------------------
	//filter = LPF(adv, last_LPF, 0.4);
	//last_LPF = filter;
	//--------------------------------------
	
//	theta = adv * ADV_TO_RAD;
	//TARGET_THETA = goal_pen_val * ADV_TO_RAD;
	e0 = e;
	e = goal_pen_val - adv;
	ei += (e + e0) / 2.0 * T;
	ed = (e - e0) / T;
	
		if(ei > 1000) ei = 1000;
		if(ei <	-1000) ei = -1000;
	
//  Calculate PID control
	 duty_ratio = ((e * KP + ei * KI + ed * KD)*100); 
		
		if(adv <= 0 || adv >= 190){
			duty_ratio = 0;
		}
		
	


	//  Introduce x, dx, theta, dtheta
	/*	int left = qei_left.getPulses();
	int right = qei_right.getPulses();
	float x0 = 0;
	float x = (left + right) / 2 * PULSE_TO_METER;
	float dx = (x - x0) / T;
	  x0 = x;
		theta = e;
	float dtheta = ed;
	float theta0 = theta;
*/

//  Calculate state feedback control
//	duty_ratio = -(int)((x*K1 + dx*K2 + theta*K3 + dtheta*K4)*100);


	//if (duty_ratio > 100) duty_ratio = 100;
	//else if (duty_ratio < -100) duty_ratio = -100;
		duty_ratio = duty_ratio / 100;
		
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
	printf("adv:%d speed:%2.2f \r\n", adv ,speed);
	//	printf("low_filter:%lf",filter);
	//	printf("goal:%lf, theta:%lf",TARGET_THETA, theta);
			printf("e:%f\r\n, ei:%f\r\n, ed:%lf \r\n",e, ei, ed);
		//printf("duty:%f\r\n", duty_ratio);
		//printf("e0 : %lf\r\n", e0);
		wait(0.08);
	}
}
