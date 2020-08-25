#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#define ADV_TO_RAD 0.00568
#define PULSE_TO_METER 0.000501
#define KP 0.025
#define KI 0
#define KD 0
#define T 0.002

BusOut led(LED1, LED2, LED3, LED4);	//set LED
AnalogIn pen(AD7);									//Potentiometer Input Output
Ticker pen_control;								//speed control time
Serial pc(USBTX, USBRX);						//serial 

// motor object
Motor motor_left(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_PWM);		//left motor
Motor motor_right(MOTOR2_IN1, MOTOR2_IN2, MOTOR2_PWM);	//right motor
// GPIO object
BusIn in(GPIO1, GPIO2, GPIO3, GPIO4);
// encoder
QEI qei_left(GPIO1, GPIO2, NC, 48, QEI::X4_ENCODING);
QEI qei_right(GPIO3, GPIO4, NC, 48, QEI::X4_ENCODING);

//*************** car speed control ***************//

int adv;												// Now AD
int goal_pen_val = 485;							// goal AD
double speed, last_speed;						// car speed, car last speed
int pen_diff;										// difference

void pen_control_handler(){
	
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

//  Calculate PID control
	float duty_ratio = ((e * KP + ei * KI + ed * KD)*100);
	
//  Introduce x, dx, theta, dtheta
	int left = qei_left.getPulses();
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
	
	if (duty_ratio > 100) duty_ratio = 100;
	else if (duty_ratio < -100) duty_ratio = -100;
	
		speed = duty_ratio;
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
		printf("goal:%d", goal_pen_val);
		wait(0.08);
	}
}

