#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "MPU6050.h" //IMU

#ifndef STASSID
#define STASSID "YJJKDS"
#define STAPSK  "18689479617"
#endif

#define PC_IP "192.168.1.100"//WIFI: YJJKDS
#define PC_PORT 23333
#define BottomIO 2

unsigned int localPort = 23333;
WiFiUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";// a string to send back

void setup() {
  
  //Serial
  Serial.begin(115200);
  //Wire
  Wire.begin();
  Wire.setClock(400000);
  //UDP
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);

  pinMode(BottomIO, INPUT);

  //IMU
  setupMPU( MPU_9250_ADDRESS );

   //Fin
  delay(100);
  Serial.println("setup fin!");
}

int oldMillis = 0;
int newMillis = 0;
void loop() {
  if( digitalRead(BottomIO)==1 ){
    recordAccelRegister( MPU_9250_ADDRESS );    //获取加速度计数据
    processAccelData();       //计算加速度计数据
    udpPrint(); 
  }else{
    Udp.beginPacket(PC_IP, PC_PORT);
    Udp.print("0");
    Udp.endPacket();
  }
  //udpRead();
}


//=================================================================

void udpRead()
{
    //print string if put in
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(PC_IP, PC_PORT);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}
void IMUoffset()
{
  G_FORCE_X = G_FORCE_X - 0.1460297;
  G_FORCE_Y = G_FORCE_Y - 0.1398246;
  G_FORCE_Z = G_FORCE_Z + 0.5811982;
}
void udpPrint()
{
  Udp.beginPacket(PC_IP, PC_PORT);
  //Udp.print(newMillis - oldMillis,6);Udp.print(",");
  Udp.print(G_FORCE_X,6);Udp.print(",");
  Udp.print(G_FORCE_Y,6);Udp.print(",");
  Udp.print(G_FORCE_Z,6);//Udp.print(",");
  Udp.endPacket();
}
void printAxis()
{
  Serial.print(G_FORCE_X);Serial.print("\t");
  Serial.print(G_FORCE_Y);Serial.print("\t");
  Serial.print(G_FORCE_Z);Serial.print("\t");
}
