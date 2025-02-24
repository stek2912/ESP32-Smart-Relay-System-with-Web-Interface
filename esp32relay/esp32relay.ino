#include <WiFi.h>
#include <WebServer.h>
#include "logo.h"  // Uključujemo logo

const char* ssid = "xs 2.4";
const char* password = "kzskjkmk7026";

const char* authUser = "kontrola";
const char* authPass = "espesp";

WebServer server(8080);

#define RELAY_1 13
#define RELAY_2 12
#define RELAY_3 14
#define RELAY_4 27

void setup() {
    Serial.begin(115200);

    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    pinMode(RELAY_3, OUTPUT);
    pinMode(RELAY_4, OUTPUT);

    digitalWrite(RELAY_1, LOW);
    digitalWrite(RELAY_2, LOW);
    digitalWrite(RELAY_3, LOW);
    digitalWrite(RELAY_4, LOW);

    WiFi.begin(ssid, password);
    Serial.print("Povezivanje na Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi povezan!");
    Serial.print("IP Adresa: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/relay", handleRelay);
    server.on("/logo.png", handleLogo);

    server.begin();
    Serial.println("Server pokrenut!");
}

void loop() {
    server.handleClient();
}

void handleLogo() {
    server.send_P(200, "image/png", (const char*)logo_png, logo_png_len);
}

void handleRoot() {
    if (!server.authenticate(authUser, authPass)) {
        server.requestAuthentication();
        return;
    }

    String html = "<html>\
    <head>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link href='https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap' rel='stylesheet'>\
    <style>\
        body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; display: flex; flex-direction: column; min-height: 100vh; }\
        .logo-container { margin: 20px auto; text-align: center; }\
        .logo { max-width: 300px; height: auto; }\
        .switch-container { display: flex; justify-content: center; align-items: center; margin: 30px; }\
        .switch-container h3 { margin-right: 15px; }\
        .switch-container h2 { color:#012a4a; }\
        .switch { position: relative; display: inline-block; width: 60px; height: 34px; }\
        .switch input { opacity: 0; width: 0; height: 0; }\
        .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px; }\
        .slider:before { position: absolute; content: ''; height: 26px; width: 26px; border-radius: 50%; left: 4px; bottom: 4px; background-color: white; transition: .4s; }\
        input:checked + .slider { background-color: #34C759; }\
        input:checked + .slider:before { transform: translateX(26px); }\
        .content { flex-grow: 1; }\
        .footer { width: 100%; height: 50px; background-color: #012a4a; color: white; font-family: 'Bebas Neue', sans-serif; font-size: 18px; text-align: center; line-height: 50px; position: relative; bottom: 0; }\
    </style>\
    <script>\
        function toggleRelay(relay) {\
            var checkbox = document.getElementById('relay' + relay);\
            var state = checkbox.checked ? 1 : 0;\
            fetch('/relay?number=' + relay + '&state=' + state);\
        }\
    </script>\
    </head>\
    <body>\
        <div class='content'>\
            <div class='logo-container'>\
                <img src='/logo.png' class='logo'>\
            </div>\
            <h2>Kontrolni panel</h2>";

    for (int i = 1; i <= 4; i++) {
        html += "<div class='switch-container'>\
            <h3>Device  " + String(i) + " </h3>\
            <label class='switch'>\
                <input type='checkbox' id='relay" + String(i) + "' onchange='toggleRelay(" + String(i) + ")' " 
                + (digitalRead(i == 1 ? RELAY_1 : i == 2 ? RELAY_2 : i == 3 ? RELAY_3 : RELAY_4) == HIGH ? "checked" : "") + ">\
                <span class='slider'></span>\
            </label>\
        </div>";
    }

    html += "</div><div class='footer'>Stefan Karaga 2025</div>\
    </body></html>";
    server.send(200, "text/html", html);
}

void handleRelay() {
    if (!server.authenticate(authUser, authPass)) {
        server.requestAuthentication();
        return;
    }

    if (server.hasArg("number") && server.hasArg("state")) {
        int relayNum = server.arg("number").toInt();
        int state = server.arg("state").toInt();
        int relayPin = (relayNum == 1) ? RELAY_1 :
                      (relayNum == 2) ? RELAY_2 :
                      (relayNum == 3) ? RELAY_3 : RELAY_4;
        digitalWrite(relayPin, state);
        server.send(200, "text/plain", "Relej " + String(relayNum) + " promenjen.");
    }
}
