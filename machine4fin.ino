/*
  WiFiEsp example: WebClientRepeating

  This sketch connects to a web server and makes an HTTP request
  using an Arduino ESP8266 module.
  It repeats the HTTP call each 10 seconds.

  For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(5, 4); // RX, TX
#endif

char ssid[] = "ILYAS2";            // your network SSID (name)
char pass[] = "9884648010";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
//int gv1 =1;
//int gv;
//int rv1;
//int rv;
//int ov1;
//int ov;
//int var1=1;
int onstatus = 0;
int offstatus = 1;
int probstatus = 0;
int probsolvedstatus = 1;
int idlestatus = 0;
int idleoffstatus = 1;
//IPAddress ip(192,168,0,3);//static IP
char server[] = "192.168.1.102";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);

  pinMode(7, INPUT); //GREEN ON
  pinMode(8, INPUT); //RED  PROBLEM
  pinMode(9, INPUT); //ORANGE IDEL-ON BUT NOT WORKING
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.print("IP Address: ");
  //WiFi.config(ip);
  Serial.print(WiFi.localIP());
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");



}

void loop()
{
  //  int onpin = digitalRead(7);
  //  int problempin = digitalRead(8);
  //  int idlepin = digitalRead(9);
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest()
{

  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  int onpin = digitalRead(7);
  int problempin = digitalRead(8);
  int idlepin = digitalRead(9);
  Serial.println("onpin=" + String(onpin) + "probpin=" + String(problempin) + "idlepin=" + String(idlepin));

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  // client.stop();

  // if there's a successful connection
  if (client.connect(server, 8081)) {
    Serial.println("Connecting...");
    // client.print(String("GET /READY HTTP/1.1"));
    // send the HTTP PUT request

    if ((onpin == 1 ) && (onstatus == 0))
    {
      onstatus = 1;
      offstatus = 1;
      client.println(String("GET /ON/AIDA4 HTTP/1.1"));
      Serial.print("on");
      // gv1=1;
    }
    /*  if(gv==1)
      {

       client.print(String("GET /STG HTTP/1.1"));
       gv=0;
        }*/

    if ((onpin == 0) && (onstatus == 1))
    {
      offstatus = 0;
      onstatus = 0;
      client.println(String("GET /OFF/AIDA4 HTTP/1.1"));
      Serial.print("off");
      //gv =1;
    }
    /*  if(var1==1)
//      {
//      client.print(String("GET /ETG HTTP/1.1"));
//      gv1=0;
//       }*/

    if ((problempin == 1) && ( probstatus == 0))
    {
      probstatus = 1;
      probsolvedstatus = 1;
      client.println(String("GET /PROBLEM/AIDA4 HTTP/1.1"));
      Serial.print("problem");
      // rv1=1;
    }
    /* if(rv==1)
      {
      client.print(String("GET /STR HTTP/1.1"));
      rv=0;
       }*/

    if ((problempin == 0) && (probstatus == 1))
    {
      probsolvedstatus = 0;
      probstatus = 0;
      client.println(String("GET /PROBLEMSOLVED/AIDA4 HTTP/1.1"));
      Serial.print("off");
      //  rv =1;
    }
    if ((idlepin == 1) && (idlestatus == 0))
    {
      idlestatus = 1;
      idleoffstatus = 1;
      client.println(String("GET /IDLE/AIDA4 HTTP/1.1"));
      Serial.print("idle");
      //  rv =1;
    }
    if ((idlepin == 0) && ( idlestatus == 1))
    {
      idleoffstatus = 0;
      idlestatus = 0;
      client.println(String("GET /IDLEOFF/AIDA4 HTTP/1.1"));
      Serial.print("idleoff");
      //  rv =1;
    }





    /*if(rv1==1)
      {
      client.print(String("GET /ETR HTTP/1.1"));
      rv1=0;
      }*/


    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}
