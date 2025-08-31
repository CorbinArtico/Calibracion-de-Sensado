#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_BMP280.h>
#include <Adafruit_SI1145.h>

#define UV_PIN 34

Adafruit_BMP280 bmp;
Adafruit_SI1145 uv_digital = Adafruit_SI1145();

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Inicializando todos los sensores...");

  if (!bmp.begin(0x76)) {
    Serial.println("No se pudo encontrar el sensor BMP280!");
    while (1) delay(10);
  }
  Serial.println("-- Sensor BMP280 encontrado.");

  if (!uv_digital.begin()) {
    Serial.println("No se pudo encontrar el sensor SI1145.");
    while (1) delay(10);
  }
  Serial.println("-- Sensor SI1145 (UV Digital) encontrado.");

  Serial.println("-- Sensor ML8511 (UV Analógico) listo para leer.");

  Serial.println("\nLectura de sensores iniciada:");
  Serial.println("------------------------------------");
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void loop() {
  Serial.print("Temperatura = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Presion = ");
  Serial.print(bmp.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.println();

  Serial.print("Luz Visible (SI1145): "); Serial.println(uv_digital.readVisible());
  Serial.print("Luz Infrarroja (SI1145): "); Serial.println(uv_digital.readIR());
  float UVindex_digital = uv_digital.readUV() / 100.0;
  Serial.print("Indice UV (SI1145): "); Serial.println(UVindex_digital);
  Serial.println();

  int uv_adc = analogRead(UV_PIN);

  float uv_voltage = (uv_adc / 4095.0) * 3.3;
  
  float uv_index_analog = mapfloat(uv_voltage, 0.99, 2.9, 0.0, 15.0);
  if (uv_index_analog < 0) {
    uv_index_analog = 0;
  }

  Serial.print("Voltaje UV (ML8511): "); Serial.print(uv_voltage); Serial.println(" V");
  Serial.print("Indice UV (ML8511): "); Serial.println(uv_index_analog);

  Serial.println("------------------------------------");
  
  delay(2000); 
}