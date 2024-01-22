#pragma once

//Konvertuje vrednost iz wide_duty u servo_duty.
int WideToServoDuty(int wide_duty);

//Ispisuje na stdout poruku u koliko je "config.h" : DEBUG_MODE 1
void DebugInfo(char* message);