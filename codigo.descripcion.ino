#include <Servo.h>  // Incluye la librería para controlar servomotores.

Servo servo;  // Declara un objeto de tipo Servo que se usará para controlar el servomotor.

const int centro = 100;  // Posición central del servo (ajustado para 90 grados).
const int izquierda = 70;  // Posición del servo para girar a la izquierda.
const int derecha = 280;  // Posición del servo para girar a la derecha.
const int levementeIzquierda = 75;  // Posición para un giro leve a la izquierda.
const int levementeDerecha = 180;  // Posición para un giro leve a la derecha.

const int velocidadAdelante = 120;  // Velocidad de los motores cuando el robot avanza.

const int echoPin = 9;  // Pin del eco del sensor ultrasónico 1.
const int trigPin = 8;  // Pin del trigger del sensor ultrasónico 1.
const int echoPin1 = 10;  // Pin del eco del sensor ultrasónico 2.
const int trigPin1 = 11;  // Pin del trigger del sensor ultrasónico 2.
const int echoPin2 = 14;  // Pin del eco del sensor ultrasónico 3.
const int trigPin2 = 15;  // Pin del trigger del sensor ultrasónico 3.

const int enaPin = 7;  // Pin para habilitar el motor A.
const int in1Pin = 5;  // Pin para controlar la dirección del motor A (adelante).
const int in2Pin = 4;  // Pin para controlar la dirección del motor A (atrás).
const int in3Pin = 3;  // Pin para controlar la dirección del motor B (adelante).
const int in4Pin = 2;  // Pin para controlar la dirección del motor B (atrás).
const int enbPin = 6;  // Pin para habilitar el motor B.

unsigned long previousMillis = 0;  // Variable para almacenar el tiempo anterior para medición.
const long intervaloMedida = 120;  // Intervalo entre mediciones de los sensores ultrasónicos.
unsigned long lastServoMove = 0;  // Variable para almacenar el último momento en que se movió el servo.
const long intervaloServo = 120;  // Intervalo entre movimientos del servo.

float distanciaRecorrida = 0.0;  // Distancia recorrida por el robot en metros.
unsigned long lastUpdateDistancia = 0;  // Última actualización de la distancia recorrida.
const float tiempoPorMedicion = 0.01;  // Intervalo de tiempo por medición en segundos.

void setup() {  
  Serial.begin(9600);  // Inicializa la comunicación serial a 9600 baudios.

  servo.attach(12);  // Conecta el servo al pin 12.

  pinMode(trigPin, OUTPUT);  // Configura el pin trig del sensor 1 como salida.
  pinMode(echoPin, INPUT);  // Configura el pin echo del sensor 1 como entrada.
  pinMode(trigPin1, OUTPUT);  // Configura el pin trig del sensor 2 como salida.
  pinMode(echoPin1, INPUT);  // Configura el pin echo del sensor 2 como entrada.
  pinMode(trigPin2, OUTPUT);  // Configura el pin trig del sensor 3 como salida.
  pinMode(echoPin2, INPUT);  // Configura el pin echo del sensor 3 como entrada.

  pinMode(enaPin, OUTPUT);  // Configura el pin para habilitar el motor A.
  pinMode(in1Pin, OUTPUT);  // Configura el pin para controlar la dirección del motor A (adelante).
  pinMode(in2Pin, OUTPUT);  // Configura el pin para controlar la dirección del motor A (atrás).
  pinMode(in3Pin, OUTPUT);  // Configura el pin para controlar la dirección del motor B (adelante).
  pinMode(in4Pin, OUTPUT);  // Configura el pin para controlar la dirección del motor B (atrás).
  pinMode(enbPin, OUTPUT);  // Configura el pin para habilitar el motor B.
}

void loop() {
  unsigned long currentMillis = millis();  // Obtiene el tiempo actual en milisegundos.

  if (currentMillis - previousMillis >= intervaloMedida) {  // Si ha pasado el intervalo, realiza la medición.
    previousMillis = currentMillis;  // Actualiza el tiempo de la última medición.

    int distance = medirDistancia(trigPin, echoPin);  // Mide la distancia con el sensor 1.
    int distance1 = medirDistancia(trigPin1, echoPin1);  // Mide la distancia con el sensor 2.
    int distance2 = medirDistancia(trigPin2, echoPin2);  // Mide la distancia con el sensor 3.

    Serial.print("Distancia Sensor 1: ");  // Imprime el texto "Distancia Sensor 1:".
    Serial.print(distance);  // Imprime la distancia medida por el sensor 1.
    Serial.print(" cm, Distancia Sensor 2: ");  // Imprime el texto "Distancia Sensor 2:".
    Serial.print(distance1);  // Imprime la distancia medida por el sensor 2.
    Serial.print(" cm, Distancia Sensor 3: ");  // Imprime el texto "Distancia Sensor 3:".
    Serial.print(distance2);  // Imprime la distancia medida por el sensor 3.
    Serial.println(" cm");  // Finaliza la línea con un salto.

    if (currentMillis - lastUpdateDistancia >= intervaloMedida) {  // Verifica si es momento de actualizar la distancia.
      distanciaRecorrida += velocidadAdelante * (255.0 / velocidadAdelante) * tiempoPorMedicion / 100.0;  
      // Calcula la distancia recorrida.
      lastUpdateDistancia = currentMillis;  // Actualiza el tiempo de la última actualización de distancia.
    }

    if (distanciaRecorrida >= 11.57) {  // Si la distancia recorrida es 11.57 metros (aproximadamente 9 metros reales).
      moverMotores(0, false);  // Detiene los motores.
      servo.write(centro);  // Centra el servo.
      Serial.println("Robot detenido. Ha recorrido 9 metros.");  // Imprime un mensaje indicando que el robot se detuvo.
      while (true);  // Detiene el programa indefinidamente.
    }

    if (distance < 110 & distance2 < distance1) {  // Si el sensor 1 detecta un obstáculo cercano y el sensor 3 también.
      moverMotores(velocidadAdelante, true);  // Mueve los motores hacia adelante.
      if (currentMillis - lastServoMove >= intervaloServo) {  // Si es momento de mover el servo.
        servo.write(derecha);  // Gira el servo a la derecha.
        lastServoMove = currentMillis;  // Actualiza el tiempo de la última acción del servo.
      }
    } else if (distance1 < 75 & distance1 < distance2) {  // Si el sensor 2 detecta un obstáculo cercano.
      moverMotores(velocidadAdelante, true);  // Mueve los motores hacia adelante.
      if (currentMillis - lastServoMove >= intervaloServo) {  // Si es momento de mover el servo.
        servo.write(levementeDerecha);  // Gira el servo levemente a la derecha.
        lastServoMove = currentMillis;  // Actualiza el tiempo de la última acción del servo.
      }
    } else if (distance2 < 75 & distance2 < distance1) {  // Si el sensor 3 detecta un obstáculo cercano.
      moverMotores(velocidadAdelante, true);  // Mueve los motores hacia adelante.
      if (currentMillis - lastServoMove >= intervaloServo) {  // Si es momento de mover el servo.
        servo.write(levementeIzquierda);  // Gira el servo levemente a la izquierda.
        lastServoMove = currentMillis;  // Actualiza el tiempo de la última acción del servo.
      }
    } else {  // Si no hay obstáculos cercanos.
      moverMotores(velocidadAdelante, true);  // Mueve los motores hacia adelante.
      if (currentMillis - lastServoMove >= intervaloServo) {  // Si es momento de mover el servo.
        servo.write(centro);  // Centra el servo.
        lastServoMove = currentMillis;  // Actualiza el tiempo de la última acción del servo.
      }
    }
  }
}

int medirDistancia(int trigPin, int echoPin) {  
  // Función que mide la distancia usando un sensor ultrasónico.
  digitalWrite(trigPin, LOW);  // Establece el pin de disparo en LOW para iniciar la medición.
  delayMicroseconds(2);  // Espera 2 microsegundos.
  digitalWrite(trigPin, HIGH);  // Envía una señal de disparo HIGH durante 10 microsegundos.
  delayMicroseconds(10);  // Espera 10 microsegundos.
  digitalWrite(trigPin, LOW);  // Vuelve a poner el pin de disparo en LOW.

  long duration = pulseIn(echoPin, HIGH);  // Mide el tiempo que toma el eco en volver.
  int distancia = duration * 0.034 / 2;  // Calcula la distancia en centímetros.
  return distancia;  // Retorna la distancia medida.
}

void moverMotores(int velocidad, bool adelante) {
  // Función para controlar los motores.
  analogWrite(enaPin, velocidad);  // Ajusta la velocidad del motor A.
  analogWrite(enbPin, velocidad);  // Ajusta la velocidad del motor B.

  if (adelante) {
    digitalWrite(in1Pin, HIGH);  // Establece la dirección del motor A hacia adelante.
    digitalWrite(in2Pin, LOW);  // Apaga la dirección inversa del motor A.
    digitalWrite(in3Pin, HIGH);  // Establece la dirección del motor B hacia adelante.
    digitalWrite(in4Pin, LOW);  // Apaga la dirección inversa del motor B.
  } else {
    digitalWrite(in1Pin, LOW);  // Apaga la dirección hacia adelante del motor A.
    digitalWrite(in2Pin, HIGH);  // Establece la dirección inversa del motor A.
    digitalWrite(in3Pin, LOW);  // Apaga la dirección hacia adelante del motor B.
    digitalWrite(in4Pin, HIGH);  // Establece la dirección inversa del motor B.
  }
}
