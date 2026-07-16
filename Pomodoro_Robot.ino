#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDHT 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDHT, SCREEN_HEIGHT, OLED_RESET);

//Pines 
int pin_trig = 2;
int pin_echo = 3; 
int pin_touch = 4;
int pin_buzzer = 5;
int pin_servo = 8;

//Variables del sensor
Servo dispensador; 
int angulo_bloqueado = 10;
int angulo_dispensar = 170; 

//Variables de tiempo
unsigned long tiempo_estudio = 1500; // cantidad de tiempo para dispensar(segundos)
unsigned long tiempo_restante = tiempo_estudio;
unsigned long ult_millis = 0; // última vez que se tomó el tiempo

//Variables sensor ultrasonico
int distancia_max = 80; // distancia máx para detectar a alguien
unsigned long tiempo_fuera = 0; // crónometro de cuanto tiempo llevas lejos
unisgned long tolerancia_tiempo = 3000 // tolerancia de ausencia (3s)
enum estado {
  ESPERA, 
  ESTUDIANDO, 
  PAUSADO, 
  PREMIO
};

estado estado_actual = ESPERA;

void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
