
#include "SocketIOClient.h"

SocketIOClient client;
//SocketIoClient socket;
const char* ssid = "Wifi";          //Tên mạng Wifi mà Socket server của bạn đang kết nối
const char* password = "12345678";  //Pass mạng wifi ahihi, anh em rãnh thì share pass cho mình với.

char host[] = "192.168.0.100";  //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
int port = 3000;                  //Cổng dịch vụ socket server do chúng ta tạo!


// ket noi chan cam bien mua
int rainPin = A0;

// DC
#define IN1 D7
#define IN2 D8
#define ENA D4
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0

bool isSend = false;


void motor_1_Dung() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void motor_1_Tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, HIGH);// chân này không có PWM
  analogWrite(IN2, speed);
}

void motor_1_Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, LOW);// chân này không có PWM
  analogWrite(IN2, speed);
}

void run1(String data) {
  Serial.println("ket noi duoc roi ne!!! " + data);
  if (data == "turnOn") {
    motor_1_Tien(MAX_SPEED);
    Serial.println("running");
  } else if (data == "[\"Server-send-turnOff\",\"turnOff\"]") {
    motor_1_Lui(MAX_SPEED);
    Serial.println("running-previous");
  } else if (data == "[\"Server-send-stop\",\"stop\"]") {
    motor_1_Dung();
    Serial.println("stop");
  }
  client.emit("sent", data);
}

void setup(){
  //Bật baudrate ở mức 115200 để giao tiếp với máy tính qua Serial
  Serial.begin(9600);

  //Việc đầu tiên cần làm là kết nối vào mạng Wifi
  Serial.print("Ket noi vao mang ");
  Serial.println(ssid);

  //Kết nối vào mạng Wifi
  WiFi.begin(ssid, password);

  //Chờ đến khi đã được kết nối
  while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.println(F("Da ket noi WiFi"));
  Serial.println(F("Di chi IP cua ESP8266 (Socket Client ESP8266): "));
  Serial.println(WiFi.localIP());

  // set up for dc
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, MAX_SPEED);

  if (!client.connect(host, port)) {
    Serial.println(("Ket noi den socket server that bai!"));
    return;
  }

  client.on("Server-send-beginSend", beginsend);
  client.on("Server-send-turnOn", run1);

  //Khi đã kết nối thành công
  if (client.connected()) {
    client.emit("Client-send-connect", "[\"Node MCU da ket noi\"]");
  }
//  motor_1_Tien(255);
}

void loop(){
  client.monitor();
  
  if(isSend){
    int valueRain = analogRead(rainPin);
    client.emit("valueRain", (String)valueRain);
    delay(1000);
  }


}

void beginsend(String data){
  Serial.println("data from server: " + data);

  isSend = true;
}
