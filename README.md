# RC Electric Curtain Rod Controller 🏠

[Русская версия](#русская-версия) | [English version](#english-version)

---

<a name="русская-версия"></a>
## 🌟 Управление карнизом Loonas через ESP32
**Совместимость с Apple HomeKit**  
Скетч для радиоуправления моторизированным карнизом с поддержкой сценариев автоматизации.

### 📖 Основные функции
- **Ручное управление**: 3 кнопки (открыть/закрыть/стоп)
- **Таймерные сценарии**:
  - Плавное открытие/закрытие (1.5-2 сек)
  - Режим "Рассвет" (постепенное открытие за 30 мин)
- **Дополнительно**: Управление USB-ночником через MOSFET

### 🛠 Техническая информация
```plaintext
Платформа: ESP32
Библиотеки:
- HomeSpan 1.6.0+ (HomeKit)
- RCSwitch 2.6.7+ (RF-управление)
Конфигурация:
- RF-передатчик: GPIO18
- MOSFET: GPIO19

⚙️ Установка

    Установите Arduino IDE 1.8.19+

    Добавьте поддержку ESP32 через Boards Manager

    Установите библиотеки:
    bash
    Copy

    arduino-cli lib install HomeSpan RCSwitch

    Загрузите скетч в ESP32

<a name="english-version"></a>
🌟 Loonas Curtain Controller via ESP32

Apple HomeKit Compatible
RF control sketch for motorized curtain rod with automation scenarios.
📖 Key Features

    Manual control: 3 buttons (open/close/stop)

    Timed scenarios:

        Soft open/close (1.5-2 sec)

        Dawn mode (30-min gradual open)

    Extra: USB night light control via MOSFET

🛠 Technical Specs
plaintext
Copy

Platform: ESP32
Libraries:
- HomeSpan 1.6.0+ (HomeKit)
- RCSwitch 2.6.7+ (RF control)
Config:
- RF Transmitter: GPIO18
- MOSFET: GPIO19

⚙️ Installation

    Install Arduino IDE 1.8.19+

    Add ESP32 via Boards Manager

    Install libraries:
    bash
    Copy

    arduino-cli lib install HomeSpan RCSwitch
