/*
 * servo.h
 *
 *  Created on: May 22, 2017
 *      Author: kmrosent
 */

#ifndef SERVO_H_
#define SERVO_H_

int servoAngle;

void Setup_Servo();
void Update_servo(int angle);
void Update_servo_add(int angle);
void Rotate();
#endif /* SERVO_H_ */
