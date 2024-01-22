
# ROS-less Jog - OSuRV 2023

  

## Opis

Svrha projekta je implementacija aplikacije za upravljanje robotskom rukom na RaspberryPi platformi.

Implementacija ukljucuje koriscenje `libenjoy` biblioteke (`https://github.com/Tasssadar/libenjoy`) za prepoznavanje ulaznih signala sa kontrolera.

Robotsku ruku pokrecu 4 servo motora.

  

Projekat se sastoji iz 2 celine:

- aplikacija `rosless_jog` koja preuzima ulazni signal kontrolera, obradjuje ga i pravi niz duzina trajanja impulsa (`duty`)

- motor_ctrl kome se prosledjuje niz duzina trajanja impulsa, a on generise PWM signal i salje ga servo motorima, cime se odredjuje ugao pomeraja ruke

  

## Struktura

Kljucni delovi aplikacije su:

- `/SW/Test/test_app/` sadrzi potrebne `.c` i `.h` fajlove `libenjoy` biblioteke (`libenjoy.c`, `libenjoy_linux.c`) i izvorni kod aplikacije (`rosless_jog.c`, `tools.c` i `config.h`).

- `/SW/Test/test_app/build/` sadrzi aplikaciju `rosless_jog` za upravljanje rukom

- `/SW/Driver/` sadrzi `motor_ctrl` modul za komunikaciju izmedju korisnicke aplikacije i servo motora

  
  

## Setup

Pre nego sto pokrenemo program, potrebno je izvrsiti sledece komande:

  

1) Prevodjenje modula rukovaoca:

SW/Driver/motor_ctrl ---> `make`

  

2) Ubacivanje modula u jezgro:

SW/Driver/motor_ctrl ---> `make start`

  

3) Konfigurisanje WAF skripte:

SW/Test/test_app ---> `./waf configure`

  

4) Kompajliranje WAF skriptom:

SW/Test/test_app ---> `./waf build`

  

## Upravljanje

Pokretanje aplikacije:

SW/Test/test_app/build/ ---> `./rosless_jog`

  

Rukom, spram pocetnih vrednosti, upravljamo na sledeci nacin:

  

- Selektujemo odredjeni motor sa LB i LT (0-3)

- Pokrecemo motore levim dzojstikom

- Menjamo brzinu okretanja sa Button2 i Button4 (1-4)

  
  

## Dodatne funkcionalnosti

Moguce je mapiranje ovih inputa na bilo koje dugme (config.h), odabir formata duzine trajanja impulsa (Wide_duty ili Servo_duty), debug mod, testiranje kontrolera i ostalo.