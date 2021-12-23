#include <ESP8266WiFi.h>
#include <stdio.h>
#include <WiFiManager.h>
#include "api.h"
#include "udp.h"

#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50
#define MAX_ENCODERS_NUM 5
#define MAX_BUTTON_NUM 5

WiFiManager wifiManager;

enum direction { CW, CCW, N };

typedef struct {
    int left;
    int right;
    direction state;
} encoder;



int prevButtonState[MAX_BUTTON_NUM];
encoder prevEncoderState[MAX_ENCODERS_NUM];

encoder encoders[MAX_ENCODERS_NUM];
unsigned int buttons[MAX_BUTTON_NUM];
unsigned int encoders_num = 1;
unsigned int buttons_num = 1;

unsigned long previousMillis = 0;

IPAddress listener_ip;
unsigned int listener_port;

bool send_udp = false;

void setup() {
    encoders[0].left = 0;
    encoders[0].right = 4;
    encoders[0].state = direction::N;

    buttons[0] = 5;
    Serial.begin(115200);
    Serial.println("Starting initilization...");
    wifiManager.autoConnect();
    init_api(register_listener);
    init_buttons();
    init_encoders();
    Serial.println("Initilization completed");
}

void init_buttons() {
    for (int i = 0; i < buttons_num; i++) {
        Serial.println("Button pin: " + String(buttons[i]));
        pinMode(buttons[i], INPUT_PULLUP);
        digitalWrite(buttons[i], HIGH);
    }
}

void init_encoders() {
    for (int i = 0; i < encoders_num; i++) {
        Serial.println("Encoder pin: " + String(encoders[i].left) + ',' + String(encoders[i].right));
        pinMode(encoders[i].left, INPUT_PULLUP);
        digitalWrite(encoders[i].left, HIGH);
        pinMode(encoders[i].right, INPUT_PULLUP);
        digitalWrite(encoders[i].right, HIGH);
    }
}

void loop() {
    http_rest_server.handleClient();
    int currButtonState[buttons_num];
    encoder currEncoderState[encoders_num];
    bool change = false;
    get_buttons_state(currButtonState);

    if (isChangeButtonState(currButtonState, prevButtonState)) {
        memcpy(prevButtonState, currButtonState, sizeof(currButtonState));
        change = true;
    }

    if (get_encoder_state(currEncoderState, prevEncoderState)) {
        if (updateEncoders(currEncoderState, prevEncoderState)) {
            change = true;
        }
        memcpy(prevEncoderState, currEncoderState, sizeof(currEncoderState));
    }

    if (change) {
        send_packet(buttonStateToString(currButtonState) + "," + encoderStateToString(currEncoderState, prevEncoderState));
    }
    delay(1);
}

bool updateEncoders(encoder *curState, encoder *privState) {
    bool changes = false;
    direction dir;
    for (int i = 0; i < encoders_num; i++) {
        dir = getEncoderDirection(curState[i], privState[i]);
        if (encoders[i].state != dir) {
            encoders[i].state = dir;
            changes = true;
        }
    }
    return changes;
}

bool get_encoder_state(encoder *state, encoder *prev_state) {
    bool change = false;
    for (int i = 0; i < encoders_num; i++) {
        state[i].left = digitalRead(encoders[i].left);
        state[i].right = digitalRead(encoders[i].right);
        state[i].state = direction::N;
        if ((state[i].left != prev_state[i].left) || (state[i].right != prev_state[i].right)) {
            change = true;
        }
    }
    return change;
}

void get_buttons_state(int *state) {
    for (int i = 0; i < buttons_num; i++) {
        state[i] = digitalRead(buttons[i]);
    }
}

bool isChangeButtonState(int *state1, int *state2) {
    for (int i = 0; i < buttons_num; i++) {
        if (state1[i] != state2[i]) {
            return true;
        }
    }
    return false;
}

void send_packet(String msg) {
    if (send_udp) {
        sendUDP(msg, listener_ip, listener_port);
    }
}

String buttonStateToString(int *state) {
    String res;
    int s;
    for (int i = 0; i < buttons_num; i++) {
        s = state[i];
        if (s == 0) {
            res += String(1);
        }
        else {
            res += String(0);
        }
        res += String(',');
    }
    res.remove(res.length() - 1);
    return res;
}

String encoderStateToString(encoder *curState, encoder *privState) {
    String res;
    for (int i = 0; i < encoders_num; i++) {
        if (encoders[i].state == direction::CW) {
            res += "0,1";
        } else if (encoders[i].state == direction::CCW) {
            res += "1,0";
        } else {
            res += "0,0";
        }
        res += String(',');
    }
    res.remove(res.length() - 1);
    return res;
}

direction getEncoderDirection(encoder curState, encoder privState) {
    if (privState.left != curState.left && curState.left == 0) {
        if (curState.left == curState.right) {
            return direction::CW;
        } else {
            return direction::CCW;
        }
    }
    return direction::N;
}

void register_listener(IPAddress ip, unsigned int port) {
    Serial.println("Register listener: " + ip.toString() + ":" + port);
    listener_ip = ip;
    listener_port = port;
    send_udp = true;
}