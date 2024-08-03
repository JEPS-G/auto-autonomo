#include <Servo.h>

Servo servo; // Creamos un objeto servo para controlar nuestro servo
int centro = 31; // Variable para almacenar la posición del servo
int derecha = 45;
int izquierda =17.50;

// Tiempo de espera 
int time = 1000;

//giro de los motores controlado por la placa L298N
int adelante = 255;
int atras = -500;

// Variable para almacenar la duración del pulso y la distancia
long duration;
int distance;

// Definir el pin del sensor ir





//definir pines para el sensor ultrasonico 
const int echoPin = 9;
const int trigPin = 8;
//definimos las variables para el ultrasonico 


//defnir pines de la placa l298n
const int enaPin = 3;
const int in1Pin = 4;
const int in2Pin = 5;
const int in3Pin = 6;
const int in4Pin = 7;
const int enbPin = 2;

void setup() {
  //DEFINICION  DEL PIN DEL SERVO MOTOR
  servo.attach(12); // Conecta el servo al pin 11
  
  //DEFINICION DE ENTRADAS Y SALIDAS DE LOS PINES DEL SENSOR ULTRASONICO
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  

 
 //SALIDAS DE LOS PINES DIGITALES PARA LA PLACA L298N
  pinMode(enaPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enbPin, OUTPUT);

}

void loop() {


 // Limpiar el pin TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Enviar un pulso de 10 microsegundos en el pin TRIG
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Leer el tiempo que tarda el pulso en regresar en el pin ECHO
  duration = pulseIn(echoPin, HIGH);
  
  // Calcular la distancia en centímetros
  distance = duration * 0.034 / 2;

   // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  //establecer la velocidad de los motores
   analogWrite(enaPin, adelante); // Motor A a velocidad definida
  analogWrite(enbPin, adelante); // Motor B a velocidad definida


  if (distance > 20) {
    //si la distancia es mayor a 20cm entonces continuar
    analogWrite(enaPin, adelante); // Motor A a velocidad definida
  analogWrite(enbPin, adelante); // Motor B a velocidad definida

    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
     //mantener el servo al centro 
     servo.write(centro);
     delay(1000);
  } else  {
     analogWrite(enaPin, adelante); // Motor A a velocidad definida
  analogWrite(enbPin, adelante); // Motor B a velocidad definida

    // si la distancia es menor a 20 cm
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
    servo.write(izquierda);
    
    delay(1000);

    }

}
   




  




