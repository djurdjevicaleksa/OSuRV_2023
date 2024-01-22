#pragma once

//Da li se koristi wide duty ili sevo duty
#define WIDE_DUTY 1

//Brzine kretanja servo motora
#define SERVO_SPEED_COUNT 4

//Predefinisane 4 vrednosti brzina okretanja
#define SERVO_SPEED_1 1
#define SERVO_SPEED_2 2
#define SERVO_SPEED_3 3
#define SERVO_SPEED_4 4

//Da li treba ispisivati debug podatke (potvrda nakon uspesnog izvrsavanja blokova koda)
#define DEBUG_MODE 0

//Koji dzojstik upravlja rukom
#define SELECTED_JOYSTICK LEFT_JOYSTICK

//Koje dugme sluzi za saltanje serva unapred
#define SELECTED_NEXT_BUTTON LEFT_BUMPER

//Koje dugme sluzi za saltanje serva unazad
#define SELECTED_PREVIOUS_BUTTON LEFT_TRIGGER

//Koje dugme sluzi za saltanje brzine unazad
#define SELECTED_SPEED_UP BUTTON_2

//Koje dugme sluzi za saltanje brzine unapred
#define SELECTED_SPEED_DOWN BUTTON_4

//Da li da ruka nastavi tamo gde je stala nakon gasenja i ponovnog pokretanja programa, ili da se vrati na pocetnu poziciju.
//'motor_ctrl' mora da radi sve vreme da bi ovo radilo.
#define REMEMBER_PAST_STATE 0

//////////////////////////////////////////////////////////////////////////////

//Formula za promenu ugla
#define DELTA_DUTY ((float)speeds[selectedSpeed] / 90) * 400

//Min-mid-max wide_servo vrednosti duty-ja
#define FAR_LEFT 100
#define MIDDLE 500
#define FAR_RIGHT 900

#define ANALOG_UP -32767
#define ANALOG_DOWN 32767
