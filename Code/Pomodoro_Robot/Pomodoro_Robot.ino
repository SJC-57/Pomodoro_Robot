#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
unsigned long tiempo_estudio = 1500; // cantidad de tiempo para dispensar(tiempo de la sesión(segundos))
unsigned long tiempo_restante = tiempo_estudio;
unsigned long ult_millis = 0; // última vez que se tomó el tiempo

//Variables sensor ultrasonico
int distancia_max = 80; // distancia máx para detectar a alguien
unsigned long tiempo_fuera = 0; // crónometro de cuanto tiempo llevas lejos
unsigned long tolerancia_tiempo = 5000; // tolerancia de ausencia (5s)

enum estado {
  ESPERA, 
  ESTUDIANDO, 
  PAUSADO, 
  PREMIO
};

estado estado_actual = ESPERA;

int calcular_distancia() {

  digitalWrite(pin_trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW); 

  long duracion = pulseIn(pin_echo, HIGH, 20000);

  int distancia = duracion*0.034/2;

  return distancia;

}
void setup() {
  Serial.begin(9600); 
  pinMode(pin_trig, OUTPUT);
  pinMode(pin_echo, INPUT);
  pinMode(pin_touch, INPUT);
  pinMode(pin_buzzer, OUTPUT);

  dispensador.attach(pin_servo);
  dispensador.write(angulo_bloqueado);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println(F("error al iniciar la OLED."));
    for(;;); //detener el programa
  }

  display.clearDisplay(); 
  display.display();
}

void cara_feliz(){

  display.setTextColor(SSD1306_WHITE);

  // Ojo izquierdo cerrado (guiño)
  display.drawFastHLine(25, 22, 20, SSD1306_WHITE);

  // Ojo derecho abierto 
  display.drawCircle(93, 22, 8, SSD1306_WHITE);
  display.fillCircle(93, 22, 4, SSD1306_WHITE);

  // Boca de alegría (círculo)
  display.fillCircle(64, 38, 7, SSD1306_WHITE);

}

void cara_enojada(){

  display.setTextColor(SSD1306_WHITE);

  // Cejas inclinadas
  display.drawLine(25, 12, 45, 22, SSD1306_WHITE);  // izquierda 
  display.drawLine(103, 12, 83, 22, SSD1306_WHITE); // derecha 

  // Ojos molestos (rectángulos)
 
  display.fillRect(25, 26, 15, 5, SSD1306_WHITE);  // izquierdo
  display.fillRect(88, 26, 15, 5, SSD1306_WHITE);  // derecho

  // Boca disgustada
  display.drawLine(54, 42, 74, 38, SSD1306_WHITE);
}  


void cara_seria(){

  display.setTextColor(SSD1306_WHITE);

  // Ojos
  display.fillCircle(35, 22, 8, SSD1306_WHITE); // izquierdo
  display.fillCircle(93, 22, 8, SSD1306_WHITE); // derecho

  // Boca (línea horizontal)
  display.drawFastHLine(54, 38, 20, SSD1306_WHITE);

}

void estudio(){
  int distancia = calcular_distancia(); 

  if(distancia > distancia_max || distancia == 0){

    //Si es que te vas
    if (tiempo_fuera == 0) {
      tiempo_fuera = millis(); 
    }

    //Si pasaron más de 5 segundos
    else if (millis() - tiempo_fuera > tolerancia_tiempo) {
      tiempo_fuera = 0; 
      estado_actual = PAUSADO; 
      return;
    }
    
  }

  else {
    tiempo_fuera = 0; 
  }


  if (millis() - ult_millis >= 1000) {
    
    ult_millis = millis();

    if (tiempo_restante > 0) {
      tiempo_restante--;
    }
    else { 
      estado_actual = PREMIO;
      return;
    }
  }

  display.clearDisplay(); 
  cara_seria(); 

  //Calcular minutos y segundos para mostralos ordenados
  int minutos = tiempo_restante/60;
  int segundos = tiempo_restante%60;

  display.setTextSize(2);
  display.setCursor(35,45);

  if (minutos < 10) { 
    display.print("0");
  }
  display.print(minutos);
  display.print(":");
  
  if(segundos < 10){
    display.print("0");
  }
  display.print(segundos);

  display.display();

}

void pausa() {

  display.clearDisplay();
  cara_enojada();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 50);
  display.print("VUELVE A ESTUDIAR!");
  display.display(); 

  tone(pin_buzzer, 350, 200);
  delay(250);

  int distancia = calcular_distancia();
  delay(50);
  distancia = calcular_distancia();

  if (distancia > 0 && distancia <= distancia_max) {
    ult_millis = millis(); 

    estado_actual = ESTUDIANDO;
  }
}

void premio(){
  display.clearDisplay(); 
  cara_feliz();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 50); 
  display.print("¡DULCE!");

  display.display();

  //Tocamos melodía de victoria
  tone(pin_buzzer, 523, 150);
  delay(150);
  tone(pin_buzzer, 659, 150);
  delay(150);
  tone(pin_buzzer, 784, 300);
  delay(300);

  dispensador.write(angulo_dispensar);
  delay(1000);
  dispensador.write(angulo_bloqueado);

  estado_actual = ESPERA;

}

void loop() {

  switch (estado_actual) {

    case ESPERA:

      display.clearDisplay(); 
      cara_seria();
      display.display();

      if (digitalRead(pin_touch) == HIGH) { 
        
        //Sonido de start
        tone(pin_buzzer, 1000, 200);
        delay(150);
        tone(pin_buzzer, 1500, 400); 
        delay(400); 

        tiempo_restante = tiempo_estudio; 
        ult_millis = millis();
        estado_actual = ESTUDIANDO;
        delay(300);
      }

      break;

    case ESTUDIANDO:
      estudio();

      break;

    case PAUSADO:
      pausa(); 

      break; 
    
    case PREMIO:
      premio(); 

      break;

    
  }
}