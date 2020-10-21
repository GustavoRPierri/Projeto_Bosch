#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <string.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 61);
const char* server = "192.168.1.68";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

  void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{
    // Open serial communications and wait for port to open:

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  //mqttClient.setServer(server, 1883);
  mqttClient.setServer("192.168.1.68", 1883);
  mqttClient.setCallback(callback);


  if (mqttClient.connect("arduino-1")) {
    // connection succeeded
    Serial.println("Connected ");
    boolean r= mqttClient.subscribe("test");
    Serial.println("subscribe ");
    Serial.println(r);
  } 
  else {
    // connection failed
    // mqttClient.state() will provide more information
    // on why it failed.
    Serial.println("Connection failed ");
  }

}

  void loop()
  {
    //

  while (true)
  {

Serial.println("publishing string");
  boolean rc = mqttClient.publish("test", "test message");
  byte outmsg[]={0xff,0xfe};
  Serial.println("publishing bytes");
   rc = mqttClient.publish("testbyte", outmsg,2);


delay(1000);
    mqttClient.loop();
  }
    
  }
