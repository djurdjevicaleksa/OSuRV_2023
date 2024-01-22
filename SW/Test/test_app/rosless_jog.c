#include <stdint.h> // uint16_t and family
#include <stdio.h> // printf and family
#include <unistd.h> // file ops
#include <fcntl.h> // open() flags
#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> //usleep

#include "motor_ctrl.h"
#include "libenjoy.h" //upravljanje dzojstikom
#include "config.h" //konfiguracija
#include "tools.h" //pomocne funkcije

//Check the configuration for the program's execution in "config.h"

int main()
{
	libenjoy_context *ctx = libenjoy_init(); // initializing libenjoy library
	libenjoy_joy_info_list *info;
	libenjoy_enumerate(ctx);
	info = libenjoy_get_info_list(ctx);
	DebugInfo("Libenjoy setup complete!\n");

	uint16_t duties[MOTOR_CLTR__N_SERVO];
	int speeds[SERVO_SPEED_COUNT] = {SERVO_SPEED_1, SERVO_SPEED_2, SERVO_SPEED_3, SERVO_SPEED_4};
	 
	int selectedServo = 0;
	int selectedSpeed = 0;

	//Pokusaj ostvarivanja komunikacije sa drajverom servo motora
	int fd;
	fd = open(DEV_FN, O_RDWR);
	if(fd < 0){
		fprintf(stderr, "[ERROR] \"%s\" not opened!\n", DEV_FN);
		fprintf(stderr, "\tfd = %d %s\n", fd, strerror(-fd));
		return 4;
	}
	DebugInfo("Motor control opened!\n");
	
	//namestanje pocetnog stanja ruke na centralni polozaj, ili ucitavanje poslednje pozicije
	if(!REMEMBER_PAST_STATE)
	{ 	
		duties[0] = WIDE_DUTY ? 390 : WideToServoDuty(390);
		duties[1] = WIDE_DUTY ? 530 : WideToServoDuty(530);
		duties[2] = WIDE_DUTY ? 650 : WideToServoDuty(650);
		duties[3] = WIDE_DUTY ? 680 : WideToServoDuty(680);

		lseek(fd, SEEK_SET, 0);    
		int r = write(fd, (char*)&duties, sizeof(duties));
		if(-1 == r)
		{
			perror("[ERROR] Servos failed to return to their starting positions!\n");
			return -1;
		}

		DebugInfo("[SUCCESS] Default position loaded!\n");
	}
	else
	{
		lseek(fd, SEEK_SET, 0);
		int r = read(fd, (char*)&duties, sizeof(duties));
		if(-1 == r)
		{
			perror("[ERROR] Servos failed to return to their past positions!\n");
			return -1;
		}

		DebugInfo("[SUCCESS] Past position loaded!\n");
	}	

    if(info->count != 0) // pokusaj uspostavljanja komunikacije sa prvim kontrolerom sa liste povezanih kontrolera
    {
        libenjoy_joystick *joy;
        joy = libenjoy_open_joystick(ctx, info->list[0]->id);
		
        if(joy) //ako je uspostavljena komunikacija hvataj inpute
        {
            int counter = 0;
            libenjoy_event ev;
            printf("[SUCCESS] Controller connected, ready to receive input!\n");

            while(1)
            {
                while(libenjoy_poll(ctx, &ev)) //cita inpute kao event i stavlja ih u "ev"
                {
                    switch(ev.type)
                    {
                    	case LIBENJOY_EV_AXIS: //ako je event input sa nekog od dzojstika
                        {
							
							if(ev.part_id == SELECTED_JOYSTICK)
							{
								DebugInfo("[STATUS] Recognized selected joystick input!\n");
								while(ev.data == ANALOG_DOWN) //smanjivanje vrednosti duty-ja
								{	
									DebugInfo("[STATUS] Recognized analog down!\n");
									libenjoy_poll(ctx, &ev);									
									
									if(duties[selectedServo] > WIDE_DUTY ? FAR_LEFT + DELTA_DUTY : WideToServoDuty(FAR_LEFT + DELTA_DUTY))
									{
										duties[selectedServo] -= WIDE_DUTY ? DELTA_DUTY : WideToServoDuty(DELTA_DUTY);										
									}
									else
									{
										duties[selectedServo] = WIDE_DUTY ? FAR_LEFT : WideToServoDuty(FAR_LEFT);
									}
									
									lseek(fd, SEEK_SET, 0);
		
									int r = write(fd, (char*)&duties, sizeof(duties));
									if(-1 == r)
									{
									    perror("[ERROR] Unable to write duties to servos after ANALOG_DOWN!\n");
									    return -1;
									}
									
									printf("\tDuty[%d]: %d\n",selectedServo, duties[selectedServo]);
									usleep(50000);
								}

								while(ev.data == ANALOG_UP) //povecavanje vrednosti duty-ja
								{
									DebugInfo("[STATUS] Recognized analog up!\n");
									libenjoy_poll(ctx, &ev);
									if(duties[selectedServo] < WIDE_DUTY ? FAR_RIGHT - DELTA_DUTY : WideToServoDuty(FAR_RIGHT - DELTA_DUTY))
									{
										duties[selectedServo] += WIDE_DUTY ? DELTA_DUTY : WideToServoDuty(DELTA_DUTY);
									}
									else
									{
										duties[selectedServo] = WIDE_DUTY ? FAR_RIGHT : WideToServoDuty(FAR_RIGHT);
									}
									
									if(duties[selectedServo] > 900)
									{
										duties[selectedServo] = 900;
									}
									
									lseek(fd, SEEK_SET, 0);
		
									int r = write(fd, (char*)&duties, sizeof(duties));
									if(-1 == r)
									{
									    perror("[ERROR] Unable to write duties to servos after ANALOG_UP!\n");
									    return -1;
									}
									printf("\tDuty[%d]: %d\n",selectedServo, duties[selectedServo]);
									usleep(50000);
								}
							}
							break;
						}

                    	case LIBENJOY_EV_BUTTON:
                        {
							if(ev.part_id == SELECTED_NEXT_BUTTON) //menjaj motore unapred
							{
								if(ev.data)
								{
								    if(selectedServo < MOTOR_CLTR__N_SERVO - 1)
									{
									    selectedServo++;
									    printf("[SELECTED] Servo %d\n", selectedServo);
									}
								}																
							}

							if(ev.part_id == SELECTED_PREVIOUS_BUTTON) //menjaj motore unazad
							{
								if(ev.data)
								{
								    if(selectedServo > 0)
								    {
										selectedServo--;
										printf("[SELECTED] Servo %d\n", selectedServo);
								    }
								}								
							}
							
							if(ev.part_id == SELECTED_SPEED_UP) //povecaj brzinu
							{
								if(ev.data)
								{
								    if(selectedSpeed < SERVO_SPEED_COUNT - 1)
									{
									    selectedSpeed++;
									    printf("[SELECTED] [%d] Speed increased to %d units.\n", selectedSpeed, speeds[selectedSpeed]);
									}
								}																
							}
							
							if(ev.part_id == SELECTED_SPEED_DOWN) //smanji brzinu
							{
								if(ev.data)
								{
								    if(selectedSpeed > 0)
									{
									    selectedSpeed--;
									    printf("[SELECTED] [%d] Speed decreased to %d units.\n", selectedSpeed, speeds[selectedSpeed]);
									}
								}																
							}

							break;
						}
                    	case LIBENJOY_EV_CONNECTED:
						{
							if(!ev.data)
							{
								printf("[STATUS] Controller \"%u\" disconnected!\n", ev.joy_id);
							}
							else
							{
								printf("[STATUS] Controller \"%u\" reconnected!\n", ev.joy_id);
							}
                        	break;
						}           	
                    }
                }

				//brojac 1s za mogucnost ponovnog poznavanja joysticka nakon diskonektovanja
                usleep(50000);

                counter += 50;
                if(counter >= 1000)
                {
					DebugInfo("[SUCCESS] Reloaded controllers!\n");
                    libenjoy_enumerate(ctx);
                    counter = 0;
                }
            }
			
            libenjoy_close_joystick(joy);
        }
        else
            printf("[ERROR] Couldn't establish a connection with a joystick!\n");
    }
    else
        printf("[ERROR] No joysticks are available!\n");

    libenjoy_free_info_list(info);

    libenjoy_close(ctx);
	close(fd);
    return 0;
}
