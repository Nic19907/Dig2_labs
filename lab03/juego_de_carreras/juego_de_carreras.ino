//juego de carreras
//contador de 8 LEDs en decadas para cada jugador
//semaforo en un LED RGB que viene en la tiva

char start = 0;

//leds para el rgb
#define LEDR  PF_1 
#define LEDB  PF_2
#define LEDG  PF_3

//botones
#define J1  PF_4
#define J2  PF_0

//LEDS jugador 1
#define J1_L0 PB_3
#define J1_L1 PC_4
#define J1_L2 PC_5
#define J1_L3 PC_6
#define J1_L4 PC_7
#define J1_L5 PD_6
#define J1_L6 PD_7
#define J1_L7 PA_2

 //LEDS jugador 2
#define J2_L0 PB_5
#define J2_L1 PB_0
#define J2_L2 PB_1
#define J2_L3 PE_4
#define J2_L4 PE_5
#define J2_L5 PB_4
#define J2_L6 PA_5
#define J2_L7 PA_6

struct estado {
  char jug1 = 0;
  char jug2 = 0;
} boton;

char contador1 = 0;
char contador2 = 0;

void setup() {
  // put your setup code here, to run once:

  //leds RGB como salida
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  //leds del jugador 1 como salida
  pinMode(J1_L0, OUTPUT);
  pinMode(J1_L1, OUTPUT);
  pinMode(J1_L2, OUTPUT);
  pinMode(J1_L3, OUTPUT);
  pinMode(J1_L4, OUTPUT);
  pinMode(J1_L5, OUTPUT);
  pinMode(J1_L6, OUTPUT);
  pinMode(J1_L7, OUTPUT);

  //leds del jugador 2 como salida
  pinMode(J2_L0, OUTPUT);
  pinMode(J2_L1, OUTPUT);
  pinMode(J2_L2, OUTPUT);
  pinMode(J2_L3, OUTPUT);
  pinMode(J2_L4, OUTPUT);
  pinMode(J2_L5, OUTPUT);
  pinMode(J2_L6, OUTPUT);
  pinMode(J2_L7, OUTPUT);

  //botones 
  pinMode(J1, INPUT_PULLUP);
  pinMode(J2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  if ((!digitalRead(J1) || !digitalRead(J2)) && !start) {
    digitalWrite(LEDB, 0);
    digitalWrite(LEDG, 0);
    digitalWrite(LEDR, 255); //luz roja
    delay (1000);

    digitalWrite(LEDG, 255); //luz amarilla
    delay (1000);

    digitalWrite(LEDR, 0); //luz verde
    delay(1000);
    digitalWrite(LEDG, 0); 
    start = 1;
    
    contador1 = 1;
    contador2 = 1;

    estado_inicialLEDS();

    digitalWrite(LEDR, 50);
    digitalWrite(LEDG, 100);
    digitalWrite(LEDB, 167);
  }

  while (start){

    if (!digitalRead(J1)){
      boton.jug1 = 1;
    }


    //cambio de decadas para las LEDS
    if (digitalRead(J1) && boton.jug1){

      //funcion para contador de leds
      leds1();

      boton.jug1 = 0;
    }

    
    if (!digitalRead(J2)){
      boton.jug2 = 1;
    }

  


    //contador para el jugador 2
    if (digitalRead(J2) && boton.jug2){

      leds2();

      boton.jug2 = 0;
    }


    //gano jugador 1
    if (digitalRead(J1_L7)){
      digitalWrite(LEDR, 100);
      digitalWrite(LEDG, 0);
      digitalWrite(LEDB, 255);
      start = 0;
    }

    //gano jugador 2
    if (digitalRead(J2_L7)){
      digitalWrite(LEDR, 0);
      digitalWrite(LEDG, 0);
      digitalWrite(LEDB, 255);
      start = 0;
    }

    if (!start) {
      break;
    }
    //en el caso de victoria J1

    
  } 
}

void estado_inicialLEDS () {
  //jugador 1
  digitalWrite(J1_L0, HIGH);
  digitalWrite(J1_L1, LOW);
  digitalWrite(J1_L2, LOW);
  digitalWrite(J1_L3, LOW);
  digitalWrite(J1_L4, LOW);
  digitalWrite(J1_L5, LOW);
  digitalWrite(J1_L6, LOW);
  digitalWrite(J1_L7, LOW);

  //jugador 2
  digitalWrite(J2_L0, HIGH);
  digitalWrite(J2_L1, LOW);
  digitalWrite(J2_L2, LOW);
  digitalWrite(J2_L3, LOW);
  digitalWrite(J2_L4, LOW);
  digitalWrite(J2_L5, LOW);
  digitalWrite(J2_L6, LOW);
  digitalWrite(J2_L7, LOW);
}

void leds2 () {
    switch (contador1){ //enciende y apaga las LEDs en orden
        case 0:
          digitalWrite(J1_L0, HIGH);
          contador1++;
          break;

        case 1:
          digitalWrite(J1_L0, LOW);
          digitalWrite(J1_L1, HIGH);
          contador1++;
          break;

        case 2:
          digitalWrite(J1_L1, LOW);
          digitalWrite(J1_L2, HIGH);
          contador1++;
          break;

        case 3:
          digitalWrite(J1_L2, LOW);
          digitalWrite(J1_L3, HIGH);
          contador1++;
          break;

        case 4:
          digitalWrite(J1_L3, LOW);
          digitalWrite(J1_L4, HIGH);
          contador1++;
          break;

        case 5:
          digitalWrite(J1_L4, LOW);
          digitalWrite(J1_L5, HIGH);
          contador1++;
          break;

        case 6:
          digitalWrite(J1_L5, LOW);
          digitalWrite(J1_L6, HIGH);
          contador1++;
          break;

        case 7:
          digitalWrite(J1_L6, LOW);
          digitalWrite(J1_L7, HIGH);
          contador1++;
          break;

        default:
          digitalWrite(J1_L2, HIGH);
          digitalWrite(J1_L3, HIGH);
          contador1 = 0;
      }
}

void leds1 () {
    switch (contador2){ //enciende y apaga las LEDs en orden
        case 0:
          digitalWrite(J2_L0, HIGH);
          contador2++;
          break;

        case 1:
          digitalWrite(J2_L0, LOW);
          digitalWrite(J2_L1, HIGH);
          contador2++;
          break;

        case 2:
          digitalWrite(J2_L1, LOW);
          digitalWrite(J2_L2, HIGH);
          contador2++;
          break;

        case 3:
          digitalWrite(J2_L2, LOW);
          digitalWrite(J2_L3, HIGH);
          contador2++;
          break;

        case 4:
          digitalWrite(J2_L3, LOW);
          digitalWrite(J2_L4, HIGH);
          contador2++;
          break;

        case 5:
          digitalWrite(J2_L4, LOW);
          digitalWrite(J2_L5, HIGH);
          contador2++;
          break;

        case 6:
          digitalWrite(J2_L5, LOW);
          digitalWrite(J2_L6, HIGH);
          contador2++;
          break;

        case 7:
          digitalWrite(J2_L6, LOW);
          digitalWrite(J2_L7, HIGH);
          contador2++;
          break;

        default:
          digitalWrite(J2_L2, HIGH);
          digitalWrite(J2_L3, HIGH);
          contador2 = 0;
      }
}
