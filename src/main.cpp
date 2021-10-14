#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>


typedef struct struct_receive_message{
  float accumulatedValue100times;
  int counter;

}struct_receive_message;

char chReceiveFlag;

typedef struct struct_send_message{
  char flag;

}struct_send_message;

struct_send_message message2send;
struct_receive_message incomingMessage;

String success;

float fAccumulatedValue100times;
int iCounterReceived;

bool isDataReceived = false;

// MAC address of our sending device
uint8_t broadcastAddress[] = {0xE0, 0xE2, 0xE6, 0x4F, 0x29, 0xA8};


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}


// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  fAccumulatedValue100times = incomingMessage.accumulatedValue100times;
  iCounterReceived = incomingMessage.counter;
  isDataReceived = true;
}





void setup() {
  Serial.begin(230400);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

} //void setup();




void loop() {
  if(isDataReceived){
    Serial.println(iCounterReceived);
    Serial.println(fAccumulatedValue100times);
    isDataReceived = false;
  }
  
}