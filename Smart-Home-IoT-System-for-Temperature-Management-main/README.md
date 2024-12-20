# Smart Home IoT System for Temperature Management
#### Watch Demo: https://drive.google.com/file/d/1v4iRNhsKFiLxRUcpIKwGafjgqwKgiEzB/view?usp=sharing

## Project Description
The Smart Home Temperature Management System is designed to enhance indoor comfort by intelligently regulating the temperature based on the **Heat Index (HI)**. The system employs sensors to continuously monitor ambient temperature (T) and relative humidity (RH) within the home environment. Using these measurements, the Heat Index (HI) is calculated using a specified formula.
Once the HI is determined, the system manipulates the windows automatically to optimize indoor climate control. In hot and humid conditions, windows may be opened to facilitate ventilation and cooling.Conversely, during colder or drier periods, windows may be closed to maintain comfort. Furthermore, the system features a user-friendly dashboard built on Node-RED, providing occupants with real-time visualization of current HI data as well as predicted HI values for the next 12 months. Leveraging the ARIMA model, the dashboard displays past and future HI trends, empowering users to make informed decisions about temperature management and usage.

### Features:
- **Automatic Window Control**: 
   - In hot and humid conditions, the system opens windows for ventilation and cooling.
   - During colder or drier periods, windows are closed to maintain comfort.
- **User-Friendly Dashboard**:
   - Built using **Node-RED**, the dashboard provides real-time visualization of the current Heat Index.
   - Displays historical and predicted HI values (up to 12 months) using the **ARIMA model**.
   - Enables users to monitor trends and make informed decisions for optimal temperature management.

---

## Architecture Diagram

![image](https://github.com/user-attachments/assets/8327f202-26c1-4621-bee3-564c0569006a)


## Architecture Description

### Sensors:
- **Temperature Sensor**: Monitors ambient temperature.
- **Humidity Sensor**: Measures relative humidity (e.g., DHT11, DHT22).

### Microcontroller:
- **ESP8266**: Acts as the processing unit, collecting data from sensors, calculating the Heat Index, and controlling window actuators.

### Communication Protocol:
- **MQTT**: Used by the microcontroller to broadcast computed HI values to an **MQTT Broker**, ensuring reliable and lightweight device-to-device communication.

### MQTT Broker:
- Serves as the central hub for distributing Heat Index data to connected clients, such as the Node-RED dashboard.

### Node-RED Dashboard:
- Provides an intuitive interface for real-time and historical data visualization:
  - **Charts and Gauges**: Display current and projected Heat Index values.
  - **Control Options**: Allow users to monitor trends and manage temperature intelligently.

### Automatic Window Control:
- The microcontroller adjusts the position of windows based on the calculated Heat Index, enabling automated climate control to maintain a comfortable indoor environment.

---

## List of Hardware

### Sensors:
- **Temperature Sensor** (e.g., DHT11, DHT22)
- **Humidity Sensor** (e.g., DHT11, DHT22)

### Actuators:
- **LED Lights** (for visual alerts or system status indication)

### Other Components:
- **WiFi Module** or Ethernet Shield
- **Power Supply** (5V, 2.5A DC)
- **Breadboard**
- **Jumper Wires**
- **Cables & Connectors**

---

This project integrates advanced IoT technologies to create a smart, efficient, and user-friendly temperature management system, ensuring a comfortable and controlled indoor environment.

