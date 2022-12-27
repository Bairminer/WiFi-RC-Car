#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "network";
const char* password = "password";

ESP8266WebServer server(80);

const byte relay1 = D5; // GPI05
const byte relay2 = D6; //GPI04
const byte relay3 = D7; //GPI0
const byte relay4 = D8; // GPI02

//
void stop_motion(){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
}

void forward(){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
}

void back(){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void stop_turn(){
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, LOW);
}

void left(){
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}

void right(){
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
}

//

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
 }

void handleCar() {
 String message = "";
 int BtnValue = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="a")
    {
      String s = server.arg(i);
      BtnValue = s.toInt();
    }
    Serial.println(server.argName(i) + ": " + server.arg(i) + "\n");
  }

  switch (BtnValue) {
   case 1: // forward left
      left();
      forward();
      break;
   case 2: // forward
     stop_turn();
     forward();
     break;
  case  3:// right
     right();
     forward();
     break;
  case  4:// left
     left();
     stop_motion();
     break;
  case 5: // stop   
     stop_turn();
     stop_motion();
     break;     
  case 6:// right
    right();
    stop_motion();
    break;
  case 7:// back left
    left();
    back();
    break;
  case 8:// back
    stop_turn();
    back();  
    break;    
  case 9:// back right
    right();
    back();
  default:
    break;
  }
 
  message += "<html> <head> <title>Wifi Hummer</title><head>";
  message += "<body><h3>Car Controls</h1>";
  message += "<table> "; 
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=1\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "<td><p><a href=\"/car?a=2\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">^</button></a></p> ";
  message += "<td><p><a href=\"/car?a=3\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=4\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> < </button></a></p> ";
  message += "<td><p><a href=\"/car?a=5\"><button style=\"width:100;height:100;font-size:40px;\" class=\"button\">Stop</button></a></p> ";
  message += "<td><p><a href=\"/car?a=6\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> > </button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=7\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<td><p><a href=\"/car?a=8\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">v</button></a></p> ";
  message += "<td><p><a href=\"/car?a=9\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "</table> "; 
  message += "</body></html>";
  server.send(200, "text/html", message);
 }

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 

void setup() {
 
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);

  pinMode(2, OUTPUT); 

  stop_turn();
  stop_motion();
  
  Serial.begin(115200); 
 
  WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/car", handleCar);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
