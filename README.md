# Управление карнизом Loonas через ESP32 / Loonas Curtain Controller via ESP32
**Совместимость с Apple HomeKit / Apple HomeKit Compatible**

----------------------------
### Русская версия

#### Техническая спецификация
- Платформа: ESP32
- Библиотеки:
  * HomeSpan 1.6.0+ (интеграция с HomeKit)
  * RCSwitch 2.6.7+ (радиоуправление)
- Конфигурация:
  * RF-передатчик: GPIO18
  * MOSFET (ночник): GPIO19

#### Установка
1. Установите Arduino IDE 1.8.19+ с сайта https://www.arduino.cc/
2. Добавьте поддержку ESP32 через Boards Manager:
   Инструменты → Плата → Менеджер плат → esp32
3. Установите библиотеки через терминал:
   arduino-cli lib install HomeSpan RCSwitch
4. Загрузите скетч в микроконтроллер

----------------------------
### English Version

#### Technical Specifications
- Platform: ESP32
- Libraries:
  * HomeSpan 1.6.0+ (HomeKit integration)
  * RCSwitch 2.6.7+ (RF control)
- Configuration:
  * RF Transmitter: GPIO18
  * MOSFET (Night Light): GPIO19

#### Installation
1. Install Arduino IDE 1.8.19+ from https://www.arduino.cc/
2. Add ESP32 support via Boards Manager:
   Tools → Board → Boards Manager → esp32
3. Install libraries via terminal:
   arduino-cli lib install HomeSpan RCSwitch
4. Upload sketch to microcontroller

----------------------------
Лицензия / License: MIT
