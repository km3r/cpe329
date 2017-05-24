/*
 * servo.c
 *
 *  Created on: May 22, 2017
 *      Author: kmrosent
 */

#include <servo.h>

void Setup_Servo() {
    servoAngle = 0;
}
void Update_servo(int angle) {
    if (angle > 180) angle = 180;
    if (angle < 0) angle = 0;
    if (angle % 10 != 0) angle = (angle/10)*10;
    servoAngle = angle;
    Rotate();
}
void Update_servo_add(int angle) {
    Update_servo(angle + servoAngle);
}

void Rotate() {

}
