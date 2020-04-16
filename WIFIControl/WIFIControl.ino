#include <SoftwareSerial.h>
#include "ESP8266.h"

#define SSID        "Home"
#define PASSWORD    "tw2302934"

#define esp8266_TX 9
#define esp8266_RX 8
#define esp8266_buad 9600

#define controlPin_UP 2
#define controlPin_DOWN 3
#define controlPin_STOP 4
#define controlPin_INT 5

char dataHost[] = "my.kusyun.com";

//char dataHost[] = "114.33.203.125";

int port = 80;
String tcpData = "GET /api/tt HTTP/1.1";

SoftwareSerial espSerial(esp8266_RX, esp8266_TX);
ESP8266 wifi(espSerial);

void setup(void) {
  Serial.begin(9600);
  espSerial.begin(esp8266_buad);

  pinMode(controlPin_UP, OUTPUT);
  pinMode(controlPin_DOWN, OUTPUT);
  pinMode(controlPin_STOP, OUTPUT);
  pinMode(controlPin_INT, OUTPUT);


  Serial.println("狀態：begin初始化完成");
  
again:
  if (wifi.setOprToStation()) {
    Serial.println("狀態：station初始化成功");
  } else {
    Serial.println("狀態：station初始化失敗");
    delay(2000);
    goto again;
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.println("狀態：WIFI連接成功"); 
    Serial.println("IP: ");       
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.println("狀態：WIFI連接失敗");
    delay(2000);
    goto again;
  }
}

void loop(void) {
  delay(5000);
  
  
  if (wifi.createTCP(dataHost, port)) {
    Serial.println("狀態：TCP連結成功");
  } else {
    Serial.println("狀態：TCP連結失敗");
    return;
  }

  Serial.println("狀態：讀取資料");

  //Serial.println(wifi.getUrlData(tcpData, dataHost, 64));

  switch(wifi.getCommand(tcpData, dataHost, 66)) {
    case 0:
      Serial.println("狀態：無動作");
      break;

    case 1:
      Serial.println("狀態：開門");
      digitalWrite(controlPin_UP, HIGH);
      delay(1000);
      digitalWrite(controlPin_UP, LOW);
      break;

    case 2:
      Serial.println("狀態：關門");
      digitalWrite(controlPin_DOWN, HIGH);
      delay(1000);
      digitalWrite(controlPin_DOWN, LOW);
      break;

    case 3:
      Serial.println("狀態：停止");
      digitalWrite(controlPin_STOP, HIGH);
      delay(1000);
      digitalWrite(controlPin_STOP, LOW);
      break;

    case 4:
      Serial.println("狀態：斷電");
      digitalWrite(controlPin_INT, HIGH);
      delay(1000);
      digitalWrite(controlPin_INT, LOW);
      break;
  }

  Serial.println("");
}
