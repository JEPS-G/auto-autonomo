#include <Servo.h>

// Declaración del servo
Servo servo;

// Posiciones del servo
const int centro = 48; // Ajustado a 90 grados para el rango típico
const int izquierda = -150; // Ajustado para girar a la izquierda
const int derecha = 150; // Ajustado para girar a la derecha
const int levementeIzquierda = -50; // Ajustado para girar ligeramente a la izquierda
const int levementeDerecha = 90; // Ajustado para girar ligeramente a la derecha

// Velocidad de los motores
const int velocidadAdelante = 100;

// Pines de los sensores ultrasónicos
const int echoPin = 9;
const int trigPin = 8;
const int echoPin1 = 10;
const int trigPin1 = 11;
const int echoPin2 = 14;
const int trigPin2 = 15;

// Pines de la placa L298N
const int enaPin = 7;
const int in1Pin = 5;
const int in2Pin = 4;
const int in3Pin = 3;
const int in4Pin = 2;
const int enbPin = 6;

// Variables de tiempo
unsigned long previousMillis = 0;
const long intervaloMedida = 10; // Intervalo para la medición del sensor en milisegundos
unsigned long lastServoMove = 0;
const long intervaloServo = 10; // Intervalo entre movimientos del servo en milisegundos

void setup() {
  // Inicializar el monitor serial
  Serial.begin(9600);

  // Configurar el pin del servo
  servo.attach(12);

  // Configurar los pines de los sensores ultrasónicos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Configurar los pines de la placa L298N
  pinMode(enaPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enbPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Controlar la medición del sensor ultrasónico
  if (currentMillis - previousMillis >= intervaloMedida) {
    previousMillis = currentMillis;

    int distance = medirDistancia(trigPin, echoPin);
    int distance1 = medirDistancia(trigPin1, echoPin1);
    int distance2 = medirDistancia(trigPin2, echoPin2);

    // Mostrar las distancias en el monitor serial
    Serial.print("Distancia Sensor 1: ");
    Serial.print(distance);
    Serial.print(" cm, Distancia Sensor 2: ");
    Serial.print(distance1);
    Serial.print(" cm, Distancia Sensor 3: ");
    Serial.print(distance2);
    Serial.println(" cm");

    // Controlar los motores y el servo según la distancia medida
    if (distance < 120) {
      // Si hay un obstáculo frontal, girar a la izquierda y ajustar a la derecha
      moverMotores(velocidadAdelante, true);
      if (currentMillis - lastServoMove >= intervaloServo) {
        servo.write(izquierda);
        lastServoMove = currentMillis;
      }
    } else if (distance1 < distance2 & distance1 < 70 ) {
      // Si hay un obstáculo en el lado izquierdo, girar a la derecha
      moverMotores(velocidadAdelante, true);
      if (currentMillis - lastServoMove >= intervaloServo) {
        servo.write(levementeDerecha);
        lastServoMove = currentMillis;
      }
    } else if (distance2 < distance1 & distance2 < 70 ) {
      // Si hay un obstáculo en el lado derecho, girar a la izquierda
      moverMotores(velocidadAdelante, true);
      if (currentMillis - lastServoMove >= intervaloServo) {
        servo.write(levementeIzquierda);
        lastServoMove = currentMillis;
      }
    } else {
      // Si no hay obstáculos, seguir adelante
      moverMotores(velocidadAdelante, true);
      if (currentMillis - lastServoMove >= intervaloServo) {
        servo.write(centro);
        lastServoMove = currentMillis;
      }
    }
  }
}

int medirDistancia(int trigPin, int echoPin) {
  // Enviar un pulso de 10 microsegundos en el pin TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer el tiempo que tarda el pulso en regresar en el pin ECHO
  long duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  int distance = duration * 0.034 / 2;

  return distance;
}

void moverMotores(int velocidad, bool adelante) {
  velocidad = constrain(velocidad, 0, 255);

  // Configura la velocidad de los motores
  analogWrite(enaPin, velocidad); // Motor A
  analogWrite(enbPin, velocidad); // Motor B

  if (adelante) {
    // Mover hacia adelante
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
  } else {
    // Detener (o mover hacia atrás si deseas)
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);
  }
}
