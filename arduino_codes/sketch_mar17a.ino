#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MAC addy of ethernet shield

char server[] = "tosinproject.herokuapp.com";    // Server address (wamp over wifi)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 137, 3);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

String relayData = "";

int relay[4]; //Relay output pins

int relayCheck[4]; //Input from contactor

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long checkInterval = 1L * 1000L; // delay between updates, in milliseconds

void setup() {
  //Relay output pins
  relay[0] = 2;
  relay[1] = 3;
  relay[2] = 4;
  relay[3] = 5;

  //Contactor input pins
  relayCheck[0] = 6;
  relayCheck[1] = 7;
  relayCheck[2] = 8;
  relayCheck[3] = 9;
  
  //Output pins initialization to Off
  for (int i=0; i<4; i++){
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], LOW);
  }

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing...");
  Serial.println("configuring using static IP...");
  // try to congifure using IP address instead of DHCP:
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

// this method makes a HTTP connection to the server:
void checkRelay() {
  // close any connection before send a new request.
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("Connected");
    // send the HTTP GET request:
    client.println("GET /api/arduino/receive.php HTTP/1.1");
    client.println("Host: tosinproject.herokuapp.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");    
  }

  // note the time that the connection was made:
  lastConnectionTime = millis();  
}

void updateServer(){
  // close any connection before send a new request.
  client.stop();
  //Build up data
  String relayParams = "A=" + String(digitalRead(relayCheck[0])) + "&" + "B=" + String(digitalRead(relayCheck[1])) + "&" + "C=" + String(digitalRead(relayCheck[2])) + "&" + "D=" + String(digitalRead(relayCheck[3]));
  Serial.println(relayParams);
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP POST request:
    client.println("POST /api/arduino/send.php HTTP/1.1");
    client.println("Host: tosinproject.herokuapp.com");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("User-Agent: arduino-ethernet");
    client.println(relayParams);
    client.println("Connection: close");
    client.println();
  } else {
    // if you couldn't make a connection:
    Serial.println("Update failed");
  }
}

void loop() {  
  if (millis() - lastConnectionTime > checkInterval) {
    checkRelay();
    /*if (relayTripped() == true){
      updateServer();
    }*/
  }
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();    
    if (c == '\n'){
      if (relayData.startsWith("cct")){
        relayData.replace("cct", "");
        Serial.println(relayData);
        triggerRelay(relayData);        
      }
      relayData = "";
    } else {
      relayData += c;
    }
  }
}

void triggerRelay(String relayCtrl){
  for (int i=0; i<relayCtrl.length(); i++){
    if (i % 4 == 0){
      int relayPin = i / 4;
      char relayStatus = relayCtrl.charAt(i + 2);
      Serial.print("RelayPin: ");
      Serial.println(relayPin);
      Serial.print("RelayStatus: ");
      Serial.println(relayStatus);
      if (relayStatus == '1'){
        digitalWrite(relay[relayPin], LOW);
      } else if (relayStatus == '0'){
        digitalWrite(relay[relayPin], HIGH);
      }
    }
  }
}

bool relayTripped(){  
  for (int i=0; i<4; i++){    
    if (digitalRead(relayCheck[i]) == HIGH){
      return true;
    }
  }
  return false;
}

