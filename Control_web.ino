#include <ESP8266WiFi.h>


const char* ssid = "tu ssid";
const char* password = "xxxxxxxxxxx";

WiFiServer server(8000);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(D0, OUTPUT);
   pinMode(D7, OUTPUT);
  digitalWrite(D0, HIGH); // EL LED INCORPORAT TE LOGICA INVERSA
   digitalWrite(D7, LOW );
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192,168,1,177), IPAddress(192,168,1,1), IPAddress(255,255,255,0),IPAddress(192,168,1,1));
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  int count=0;
  while(!client.available()){
    delay(1);
    Serial.println(count++);
    if(count>20) break;
  }

 if (count<21){
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(D7, HIGH);
    digitalWrite(D0, LOW);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(D7, LOW);
    digitalWrite(D0, HIGH);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<style>");
  client.println(".buto {background-color: #000000;color: #FFFFFF;padding: 10px;border-radius: 10px;-moz-border-radius: 10px;-webkit-border-radius: 10px;margin:10px;width: 90px;height: 40px;}");
  client.println(".roig {color:white;background:red}");
  client.println(".verd {color:black;background:green}");
  client.println("</style>");
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button class='buto verd'>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class='buto roig'>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 }
 
}
 
