/* 
 *  Управление карнизом и ночником через HomeKit
 *  С использованием ESP32 и RF-передатчика
 */

// Подключение необходимых библиотек
#include "HomeSpan.h"         // Библиотека для интеграции с Apple HomeKit
#include <RCSwitch.h>         // Библиотека для работы с RF-передатчиком

// Определение пинов
#define TX_PIN 18             // Пин для управления RF-передатчиком
#define MOSFET_PIN 19         // Пин для управления MOSFET-транзистором (ночник)

// Инициализация объекта RF-передатчика
RCSwitch mySwitch = RCSwitch();

// Буферы с временными интервалами для сигналов кнопок (в микросекундах)
unsigned long button1Buffer[] = {
  // RAW-данные сигнала для кнопки 1
  5023,1458,378,721,377,723,377,722,377,722,716,385,376,722,377,723,376,727,376,724,376,722,715,387,375,725,375,724,713,387,377,721,716,389,376,724,375,723,376,724,376,725,375,724,713,387,377,722,377,725,377,724,376,723,376,725,375,723,376,724,376,725,374,723,716,389,377,722,377,723,715,385,715,385,376,725,376,724,715,383,716,2092,5028,1456,377,724,376,724,375,725,374,725,713,387,377,723,376,722,377,726,377,722,378,721,715,386,376,723,377,723,715,385,376,725,713,390,376,724,375,724,377,724,376,722,376,723,716,385,376,724,376,727,377,723,376,724,376,723,376,725,375,725,374,724,376,723,717,388,377,722,377,722,716,384,716,387,376,722,377,723,714,385,716,2092,5027,1459,377,723,376,722,377,723,377,723,715,385,376,723,376,723,376,728,376,723,377,723,714,387,377,721,377,723,714,386,376,724,714,391,375,723
};

unsigned long button2Buffer[] = {
  // RAW-данные сигнала для кнопки 2
  5027,1458,378,722,376,724,376,725,375,724,715,385,377,723,377,722,377,727,376,725,375,722,716,387,376,722,377,723,716,386,375,723,715,391,374,724,377,723,377,722,376,726,374,725,713,386,376,724,378,725,377,722,377,723,377,725,377,722,378,723,375,723,377,723,715,388,377,722,715,387,377,723,716,384,377,722,715,385,379,723,713,2055,5029,1460,375,722,376,725,375,724,377,722,716,385,377,723,376,724,377,727,375,725,376,724,713,386,376,723,377,723,715,387,377,722,715,388,377,724,377,724,376,722,377,726,372,724,716,384,377,723,376,729,376,723,377,722,377,723,376,724,376,722,377,726,375,722,716,390,376,722,716,385,377,721,716,385,376,723,715,386,377,722,716,2055,5027,1457,377,722,377,723,377,723,376,723,715,387,375,724,376,724,376,726,376,724,376,723,716,386,375,724,377,723,714,386,377,721,716,388,376,724
};

unsigned long button3Buffer[] = {
  // RAW-данные сигнала для кнопки 3
  5029,1459,377,722,378,722,377,723,376,724,715,387,377,722,377,723,377,729,376,722,377,722,716,387,375,725,376,723,716,384,377,725,713,390,377,722,377,723,377,724,376,724,377,723,716,384,377,725,377,725,377,723,377,725,377,722,378,723,376,723,376,726,374,726,713,390,377,723,376,725,375,725,714,386,377,722,377,725,377,722,716,2020,5031,1459,375,725,375,725,377,722,377,723,715,387,376,725,374,726,375,728,376,723,378,723,715,387,375,724,377,722,718,384,377,723,713,390,377,723,377,723,379,722,378,722,378,721,716,387,375,724,377,726,377,723,377,723,377,725,376,722,377,723,377,725,375,723,716,389,377,725,374,726,376,722,716,384,377,725,375,726,376,723,715,2021,5030,1457,377,723,376,725,377,722,377,722,715,386,378,722,377,723,376,728,376,725,374,724,715,385,377,725,377,722,714,386,377,723,714,391,376,723
};

// Расчет длины массивов сигналов
int button1Length = sizeof(button1Buffer)/sizeof(button1Buffer[0]);
int button2Length = sizeof(button2Buffer)/sizeof(button2Buffer[0]);
int button3Length = sizeof(button3Buffer)/sizeof(button3Buffer[0]);

// Прототип функции для отправки сигналов
void sendButtonSignal(int button);

// Глобальные переменные состояния
unsigned long lastActionTime = 0;  // Время последнего действия
int pendingAction = 0;            // Ожидающее действие для обработки
int dawnCounter = 0;              // Счетчик циклов режима рассвета
bool needStop = false;            // Флаг необходимости остановки

// Класс для управления карнизом через HomeKit
struct DEV_Cornice : Service::Switch { 
    SpanCharacteristic *power;    // Характеристика включения/выключения
    int buttonNumber;             // Номер связанной кнопки

    // Конструктор класса
    DEV_Cornice(int btnNumber) : Service::Switch() {
        buttonNumber = btnNumber; // Инициализация номера кнопки
        power = new Characteristic::On(0); // Создание характеристики
    }

    // Обработчик обновления состояния
    boolean update() override {
        if(power->getNewVal()) {
            switch(buttonNumber) {
                case 1: sendButtonSignal(1); break;  // Кнопка 1
                case 2: sendButtonSignal(2); break;  // Кнопка 2
                case 3: sendButtonSignal(3); break;  // Кнопка 3
                case 4:  // Автоматическое открытие
                    sendButtonSignal(1);
                    pendingAction = 1;
                    lastActionTime = millis();
                    break;
                case 5:  // Автоматическое закрытие
                    sendButtonSignal(3);
                    pendingAction = 2;
                    lastActionTime = millis();
                    break;
                case 6:  // Режим рассвета
                    dawnCounter = 0;
                    pendingAction = 3;
                    lastActionTime = 0;
                    Serial.println("Старт режима рассвета");
                    break;
            }
            power->setVal(0);  // Сброс переключателя
        }
        return true;
    }
};

// Класс для управления ночником
struct DEV_NightLight : Service::Switch {
    SpanCharacteristic *power;    // Характеристика включения
    
    // Конструктор класса
    DEV_NightLight() : Service::Switch() {
        pinMode(MOSFET_PIN, OUTPUT);  // Настройка пина как выхода
        digitalWrite(MOSFET_PIN, LOW); // Начальное состояние
        power = new Characteristic::On(); // Создание характеристики
    }

    // Обработчик обновления состояния
    boolean update() override {
        digitalWrite(MOSFET_PIN, power->getNewVal()); // Управление MOSFET
        return true;
    }
};

// Настройка устройства
void setup() {
  Serial.begin(115200);  // Инициализация последовательного порта
  
  // Настройка RF-модуля
  mySwitch.enableTransmit(TX_PIN);  // Активация передатчика
  mySwitch.setPulseLength(320);     // Установка длительности импульса
  mySwitch.setRepeatTransmit(10);   // Количество повторов

  // Инициализация HomeSpan
  homeSpan.begin(Category::WindowCoverings, "Управление карнизом");

  // Создание аксессуара для карниза
  new SpanAccessory();
    new Service::AccessoryInformation();  // Информация об устройстве
      new Characteristic::Name("Карниз");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-456");
      new Characteristic::Model("Cornice Control");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();

    // Создание переключателей управления
    new DEV_Cornice(1);  // Кнопка 1
    new DEV_Cornice(2);  // Кнопка 2
    new DEV_Cornice(3);  // Кнопка 3
    new DEV_Cornice(4);  // Автооткрытие
    new DEV_Cornice(5);  // Автозакрытие
    new DEV_Cornice(6);  // Режим рассвета

  // Создание аксессуара для ночника
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Ночник");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("789-012");
      new Characteristic::Model("Night Light");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();
    
    new DEV_NightLight();  // Добавление сервиса ночника
}

// Основной цикл программы
void loop() {
    homeSpan.poll();  // Обработка HomeSpan
    unsigned long currentTime = millis();  // Текущее время

    // Обработка автоматической остановки
    if((pendingAction == 1 || pendingAction == 2) && 
      (currentTime - lastActionTime >= 1500)) {
        sendButtonSignal(2);  // Отправка сигнала остановки
        pendingAction = 0;
        Serial.println("Автостоп выполнен");
    }

    // Управление режимом рассвета
    if(pendingAction == 3) {
        if(dawnCounter < 4) {
            if(lastActionTime == 0 || 
              (currentTime - lastActionTime >= 600000)) {
                sendButtonSignal(3);  // Открытие
                lastActionTime = currentTime;
                needStop = true;
                dawnCounter++;
                Serial.printf("Рассвет: цикл %d/4 - открытие\n", dawnCounter);
            }
            else if(needStop && 
              (currentTime - lastActionTime >= 1500)) {
                sendButtonSignal(2);  // Остановка
                needStop = false;
                Serial.printf("Рассвет: цикл %d/4 - остановка\n", dawnCounter);
                lastActionTime = currentTime;
            }
        }
        else {
            pendingAction = 0;  // Завершение режима
            dawnCounter = 0;
            Serial.println("Режим рассвета завершен");
        }
    }
}

// Функция отправки RF-сигнала
void sendButtonSignal(int button) {
    unsigned long* buffer = nullptr;
    int length = 0;

    // Выбор соответствующего буфера
    switch(button) {
        case 1: 
            buffer = button1Buffer; 
            length = button1Length; 
            break;
        case 2: 
            buffer = button2Buffer; 
            length = button2Length; 
            break;
        case 3: 
            buffer = button3Buffer; 
            length = button3Length; 
            break;
    }

    // Отправка сигнала
    if(buffer && length > 0) {
        Serial.printf("Передача сигнала кнопки %d...\n", button);
        for(int i=0; i<length; i++) {
            digitalWrite(TX_PIN, HIGH);
            delayMicroseconds(buffer[i]);
            digitalWrite(TX_PIN, LOW);
            if(++i < length) delayMicroseconds(buffer[i]);
        }
        Serial.println("Сигнал передан");
    }
}
