# 📘 Tarea 5 – Sistemas Embebidos  
**Sistemas Embebidos IIPAO 2025**

Este repositorio contiene **cuatro ejercicios prácticos** desarrollados sobre el **ESP32**, cuyo objetivo es reforzar los conocimientos relacionados con:

- Comunicación serial mediante **UART**
- Programación **multitarea con FreeRTOS**
- **Ahorro de energía** en sistemas embebidos
- Integración de **UART + FreeRTOS** en un sistema completo y funcional

Cada ejercicio aborda un concepto específico y aumenta progresivamente el nivel de integración y complejidad del sistema.

---

## 🧩 Contenido del repositorio

Los ejercicios desarrollados permiten:

- Implementar comunicación UART mediante comandos
- Crear y gestionar múltiples tareas concurrentes con FreeRTOS
- Sincronizar tareas usando mecanismos del RTOS
- Aplicar modos de bajo consumo del ESP32
- Diseñar un sistema embebido modular, estable y verificable

---

## ⚙️ Requisitos técnicos

- **Microcontrolador:** ESP32  
- **Framework:** ESP-IDF  
- **RTOS:** FreeRTOS  
- **Comunicación:** UART  
- **Lenguaje:** C  
- **Entorno de desarrollo:** VS Code + ESP-IDF  
- **Extensiones recomendadas:**
  - PlatformIO
  - Serial Monitor
  - Wokwi para VS Code

---

## ▶️ Cómo ejecutar el proyecto

1. Clona el repositorio:
   ```bash
   git clone https://github.com/chris816wui/Tarea-5-Lema

2. Instalar las extensiones necesarias en VS Code

- PlatformIO
- Serial Monitor
- Wokwi

3. Compilar el proyecto

Compila el proyecto utilizando PlatformIO.

4. Configurar el monitor serial

Configura el monitor serial con los siguientes parámetros:

- Modo: TCP
- Host: localhost
- Puerto: 4000

5. Ejecutar la simulación

Ejecuta el archivo diagram.json y simula cada ejercicio desde el entorno de Wokwi en VS Code.

##👤 Autor

Christopher Lema
Proyecto académico – Sistemas Embebidos
IIPAO 2025

##📄 Licencia

Este proyecto se distribuye bajo la licencia MIT.
Puedes utilizarlo, modificarlo y adaptarlo libremente para fines académicos y educativos.
