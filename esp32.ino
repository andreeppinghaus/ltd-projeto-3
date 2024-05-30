#include <Arduino.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SSD1306.h>

// Definir pinos para trigger e echo do sensor ultrassônico
#define PINO_TRIGGER 18
#define PINO_ECHO 5

// Definir dimensões do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Pode usar outro pino se necessário

class SensorManager {
public:
    SensorManager(int triggerPin, int echoPin)
        : ultrasonic(triggerPin, echoPin), bmp(), display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    void begin() {
        Serial.begin(921600);
        Serial.println("Inicializando sensores...");

        // Inicializa o sensor BMP180
        if (!bmp.begin()) {
            Serial.println("Não foi possível encontrar o sensor BMP180!");
            while (1) {} // Loop infinito se o sensor não for encontrado
        } else {
            Serial.println("Sensor BMP180 encontrado.");
        }

        // Inicializa o display OLED
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println("Não foi possível inicializar o display OLED!");
            while (1) {} // Loop infinito se o display não for encontrado
        } else {
            Serial.println("Display OLED encontrado.");
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.display();
    }

    void read() {
        distance = ultrasonic.read();
        temperature = bmp.readTemperature();
        pressure = bmp.readPressure();
    }

    void write() {
        // Exibir no Serial Monitor
        Serial.println("\nLendo dados dos sensores...");
        Serial.print("Distância (cm): ");
        Serial.println(distance);
        Serial.print("Temperatura (C): ");
        Serial.println(temperature);
        Serial.print("Pressão (Pa): ");
        Serial.println(pressure);

        // Exibir no display OLED
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Dados dos Sensores:");
        display.print("Distancia (cm): ");
        display.println(distance);
        display.print("Temp (C): ");
        display.println(temperature);
        display.print("Pressao (Pa): ");
        display.println(pressure);
        display.display();
    }

private:
    Ultrasonic ultrasonic;
    Adafruit_BMP085 bmp;
    Adafruit_SSD1306 display;
    float distance;
    float temperature;
    float pressure;
};

SensorManager sensorManager(PINO_TRIGGER, PINO_ECHO);

void setup() {
    sensorManager.begin();
}

void loop() {
    sensorManager.read();
    sensorManager.write();
    delay(1000); // Adiciona um pequeno delay para evitar muitas leituras consecutivas
}
