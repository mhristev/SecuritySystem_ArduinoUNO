#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>      

const char* ssid = "";
const char* password = "";

const char* http_username = "admin";
const char* http_password = "admin";

bool opening = false;
const int output = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Closed...</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.6rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 10px;}
    
  </style>
  
</head>
<body>
  <h2>Safe Opener Web Server</h2>
  <button onclick="logoutButton()">Logout</button>
  <p>STATE: <span>CLOSED</span></p>
  <button onclick="test()" style="background-color: #195B6A; border: none; color: white; padding: 16px 40px;"> OPEN HERE </button>


<script>

function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}


function test() {
  console.log("hew");
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/2", true);
  xhr.send();
  setTimeout(function(){ window.open("/2","_self"); }, 1000);
  
}
</script>
</body>
</html>
)rawliteral";

const char index2_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Opening...</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">

</head>
<body>
  <h2>We are opening the safe...</h2>
  <!--<button onclick="logoutButton()">Logout</button>-->
  <p>STATE:<span>OPEN</span></p>
<script>
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}


function opa(){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/", true);
  xhr.send();
  setTimeout(function(){ window.open("/","_self"); }, 1000);
}

setTimeout(opa, 7000);


</script>
</body>
</html>
)rawliteral";




const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
</head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
</body>
</html>
)rawliteral";


void setup(){
  Serial.begin(9600);

  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  
  Serial.println(WiFi.localIP());

  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    
    opening = false;

    request->send_P(200, "text/html", index_html, nullptr);
  });

  server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
     
    opening = true;
    
    request->send_P(200, "text/html", index2_html, nullptr);
  });
    
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, nullptr);
  });

  server.begin();
}
  
void loop() {
  if(opening) {
    Serial.write("opening");
    delay(5000);
    opening = false;
    Serial.write("closing");
  }
}
