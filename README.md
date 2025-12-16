# 🌡️ Temperature-Controlled Fan System

**Tech Stack:** Arduino UNO, LM35 Temperature Sensor, L298N Motor Driver, DC Fan, I2C 16×2 LCD  
**Domain:** Embedded Systems · Control Systems · Firmware Development

---

## 📌 Project Overview

This project implements a **temperature-controlled DC fan system** using Arduino.  
The system continuously monitors ambient temperature using an LM35 sensor and automatically adjusts the fan speed using **PWM-based closed-loop control**.

To ensure smooth and safe operation, **soft PWM ramping** is implemented to prevent sudden fan speed changes.  
Live temperature and fan speed information is displayed on a **16×2 I2C LCD**.

---

## ⚙️ What I Built

- Developed **Embedded C firmware** for real-time temperature sensing  
- Converted ADC readings into calibrated temperature values (°C)  
- Implemented **PWM-based fan speed control** using Arduino timers  
- Interfaced **L298N H-bridge motor driver** to drive a 12V DC fan safely  
- Displayed live temperature and PWM status on an **I2C LCD**  
- Implemented **smooth PWM ramping** to avoid sudden speed changes  
- Reduced ADC noise using sensor sampling and averaging techniques  

---

## 🧠 How the System Works

1. LM35 outputs an analog voltage proportional to temperature  
2. Arduino reads the voltage using its ADC  
3. Firmware converts voltage into temperature (°C)  
4. Temperature is mapped to a PWM duty cycle  
5. PWM signal controls fan speed via L298N  
6. LCD displays real-time temperature and fan speed  

---

## 🔧 Hardware Components

- Arduino UNO  
- LM35 Temperature Sensor  
- L298N Motor Driver  
- 12V DC Fan  
- 16×2 I2C LCD  
- External 12V Power Supply  
- Jumper Wires  

---

## 🔌 Pin Connections

| Component | Arduino Pin |
|---------  |-------------|
| LM35 OUT  |     A0      |
| LCD SDA   |     A4      |
| LCD SCL   |     A5      |
| L298N ENA |     D11     |
| L298N IN1 |     D9      |
| L298N IN2 |     D8      |

> **Note:** All grounds must be common. Motor supply (12V) is separate from Arduino 5V.

---

## 🖥️ Circuit Diagram

The complete Proteus schematic is available in this repository:

