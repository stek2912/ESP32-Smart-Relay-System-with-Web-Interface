# ESP32 Relay Control

ESP32 web server za kontrolu releja putem web interfejsa. Projekat omogućava daljinsko upravljanje relejima sa autentifikacijom.

## 🛠️ Hardverski zahtevi
- ESP32 razvojna ploča
- 4 releja
- 5V napajanje

## 🔧 Podešavanje
1. Ubaci SSID i lozinku Wi-Fi mreže u `ESP32-Relay.ino`
2. Uploadaj kod na ESP32 pomoću Arduino IDE-a ili PlatformIO-a
3. Pristupi serveru putem **http://ESP_IP:8080**

## 🔐 Pristup
Da bi upravljao relejima, unesi:
- **Username:** `kontrola`
- **Password:** `espesp`

## 🖥️ Web interfejs
Web interfejs sadrži **prekidače** za svaki od 4 releja.

![Web interfejs](https://via.placeholder.com/600x300) *(Ubaci pravi screenshot)*

## 🚀 API Endpoints
- `/relay?number=X&state=Y`  
  - `X` = broj releja (1-4)  
  - `Y` = 1 (uključi) ili 0 (isključi)  
  - Primer: `http://ESP_IP:8080/relay?number=1&state=1`

## 📜 Licence
Ovaj projekat je otvorenog koda. Možete ga koristiti i modifikovati.
