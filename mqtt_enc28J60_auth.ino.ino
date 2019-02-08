/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

//#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

// MAC FISICA ETHERNET
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//IPAddress ip(172, 16, 0, 100);
const char* mqtt_server = "broker.shiftr.io";


///////////////////////AUTENTICACION BROKER MQTT///////////////////////////////////////////
const char clientID[]="MARKO";  //identificador unico de para cada dispositivo iot
const char username[]="marko-caballero";       //usuario configurado en broker
const char passwords[]="marko-caballero";      //contraseña usuario broker
const char willTopic[]= "";
int willQoS=2 ;                             //0-1-2
int  willRetain=1 ;                         //0-1  //si se activa o no la retencion de data
const char willMessage[]="";    //mensaje cuando device este desconectado de broker
////////////////////////////////////////////////////////////////////////////////////////////////

long lastMsg = 0;
char msg[50];
int value = 0;

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
 
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Esperando..");
    
    // Attempt to connect
   // if (client.connect(clientId.c_str())) {
if(client.connect(clientID,username,passwords,willTopic,willQoS,willRetain,willMessage)){
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  Serial.begin(115200);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //Ethernet.begin(mac, ip); //si se requiere IP fija
  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
  delay(1500);
Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());
  // Allow the hardware to sort itself out

   Serial.print("Connecting to broker: ");
   Serial.print(mqtt_server);
   client.setServer(mqtt_server, 1883);
   client.setCallback(callback);

 Serial.println(" ");
 
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    //lectura de sensor////////////////////////////////////////////////////
    float temp =random(20, 25); //simulamos que leemos el dato de un sensor
    float hum = random(80,98);   
  /////////////////////////////////////////////////////////////////////////
    
    char sense[]=""; //almacenar dato en char array
 
    String my_payload=String(temp); //convierto luego a string
    my_payload.toCharArray(sense, (my_payload.length() + 1)); //luego convierto a char array 
 
    client.publish("temperatura", sense); //siempre recibe un char array 
    Serial.println(sense);
    //////////////////////////////////////////////////////////////////////
    my_payload=String(hum); //convierto luego a string
    my_payload.toCharArray(sense, (my_payload.length() + 1)); //luego convierto a char array 
 
    client.publish("humedad", sense); //siempre recibe un char array 
    Serial.println(sense);
  }
}
