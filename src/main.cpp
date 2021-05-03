#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <Adafruit_NeoPixel.h>
#include "Webpages.h"

ESP8266WebServer server(80);

WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
// Offset de -5 horas (-5 * 60 * 60) y se refresca cada 24 horas (24 * 60 * 60 * 1000)
NTPClient timeClient(ntpUDP, "pa.pool.ntp.org", -18000, 86400000);

const int LED_PIN = D1;
const int LED_COUNT = 21;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Set AP credentials
#define AP_SSID "PASATIEMPO_J1"
#define AP_PASS "123"

IPAddress local_ip(192, 168, 11, 111);
IPAddress gateway(192, 168, 11, 1);
IPAddress netmask(255, 255, 255, 0);

const char *_ssid = "", *_pass = "";

byte brillo = 200;   // Brillo de los leds via wifi
byte brillo_2 = 200; // Brillo de los leds para indicador de hora

int proc_activo = 0;
int max_hue = 65535;
int segmentos = 4;
int grupo = max_hue / (strip.numPixels() * segmentos);
int activo = 0;

String IP_Show, Red_String, Password_String;

String Head_1 = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<link rel=\"icon\" href=\"data:,\"><style>#button1{width: 100px;height: 40px;}</style>\
<title> Ledstival</title></head><body><center><h1>Control de luces Wifi</h1>\
<p><a href=\"/rainbow\"><button id=\"button1\" class=\"button\">Arcoiris</button></a>\
<a href=\"/wave?r=255&g=32&b=10\"><button id=\"button1\" class=\"button\">Movimiento</button></a>\
<a href=\"/on_off_fast\"><button id=\"button1\" class=\"button\">On Off</button></a></p>\
<p><a href=\"/setleds?r=255&g=255&b=255\"><input type=\"button\" style=\"background-color:#FFFFFF;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=255&g=0&b=0\"><input type=\"button\" style=\"background-color:#FF0000;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=153&g=0&b=153\"><input type=\"button\" style=\"background-color:#990099;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=204&g=0&b=204\"><input type=\"button\" style=\"background-color:#CC00CC;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=51&g=51&b=204\"><input type=\"button\" style=\"background-color:#3333CC;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=0&g=102&b=102\"><input type=\"button\" style=\"background-color:#006666;color:white;width:20px; height:20px\"></a>\
</p><p><a href=\"/setleds?r=0&g=204&b=255\"><input type=\"button\" style=\"background-color:#00CCFF;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=0&g=204&b=0\"><input type=\"button\" style=\"background-color:#00CC00;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=102&g=153&b=0\"><input type=\"button\" style=\"background-color:#669900;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=255&g=255&b=0\"><input type=\"button\" style=\"background-color:#FFFF00;color:white;width:20px; height:20px\"></a>\
<a href=\"/setleds?r=255&g=153&b=51\"><input type=\"button\" style=\"background-color:#FF9933;color:white;width:20px; height:20px\"></a></p>\
<form action=\"/setbrightness\"><p><label for=\"brillo\">Brillo:</label>\
<input type=\"range\" id=\"brillo\" name=\"br\" min=\"0\" max=\"200\" value=\"";

String Head_2 = "\"><input type=\"submit\" value=\"Enviar\"></p></form>\
<p><a href=\"/ledsoff\"><button id=\"button1\" class=\"button\">Apagar</button></a></p>";

// ***** FUNCIONES *****
void ConfigLocal();
void handleSettingsUpdate();
void Cargar_Credenciales();
void handleNotFound();
void Inicio();
String Head(int br);
void Arco_page();
void Wave_page();
void Onoff_page();
void Color_page();
void Brillo_page();
void Off_page();
void print_proc(int a);
void arcoiris();
void patron(unsigned int ciclos);
void on_off();
void led_hora();
void avisa_hora();
void avisa_leds();
void read_ip_web();

// ***** FUNCIONES *****

void setup()
{
  Serial.begin(115200);
  WiFi.hostname("BOTELLA_WIFI");

  // Begin Access Point
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(local_ip, gateway, netmask);
  //  WiFi.softAP(AP_SSID,AP_PASS);
  WiFi.softAP(AP_SSID);

  Serial.println("Paso 1");

  //  Serial.println("Formatting LittleFS filesystem");
  //  LittleFS.format();
  //Serial.println("Mount LittleFS");
  if (!LittleFS.begin())
  {
    Serial.println("LittleFS mount failed");
    return;
  }
  delay(5000);
  Serial.println("Paso 2");

  //Cargar credenciales desde archivo
  Cargar_Credenciales();
  Serial.println("Paso 3");

  // Begin WiFi
  WiFi.begin(_ssid, _pass);
  //WiFi.begin();
  if (WiFi.status() == WL_CONNECTED)
  {
    IP_Show = WiFi.localIP().toString().c_str();
    ;
  }
  else
  {
    IP_Show = "Sin Valor";
  }
  //  // Connecting to WiFi...
  //  Serial.print("Connecting to ");
  //  Serial.print(WIFI_SSID);

  // Inicia NTP client
  timeClient.begin();

  // Display static text
  //  display.println(WIFI_SSID);
  //  display.println("Pendiente...");
  //  display.display();

  //  // Loop continuously while WiFi is not connected
  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    delay(100);
  //    Serial.print(".");
  //  }
  Serial.println("Paso 4");

  // Connected to WiFi
  Serial.print("Listo! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Local: ");
  Serial.println(AP_SSID);
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.getMode());

  server.on("/", Inicio);
  server.on("/configurar", ConfigLocal);
  server.on("/settings", HTTP_POST, handleSettingsUpdate);
  server.on("/loadip", read_ip_web);

  server.on("/rainbow", []() {
    Arco_page();
    proc_activo = 1;
    // print_proc(proc_activo);
  });

  server.on("/wave", []() {
    Wave_page();
    proc_activo = 2;
    // print_proc(proc_activo);
  });

  server.on("/on_off_fast", []() {
    Onoff_page();
    proc_activo = 3;
    // print_proc(proc_activo);
  });

  server.on("/setleds", []() {
    Color_page();
    proc_activo = 4;
    // print_proc(proc_activo);
    // Serial.println("-----------");
    // Serial.print(server.arg(0).toInt());
    // Serial.print(" ");
    // Serial.print(server.arg(1).toInt());
    // Serial.print(" ");
    // Serial.println(server.arg(2).toInt());
    uint32_t nuevo_color = strip.Color(server.arg(0).toInt(), server.arg(1).toInt(), server.arg(2).toInt());
    strip.setBrightness(brillo);
    strip.fill(nuevo_color);
    strip.show();
  });

  server.on("/setbrightness", []() {
    brillo = server.arg(0).toInt();
    Brillo_page();
    strip.setBrightness(brillo);
    strip.show();
  });

  server.on("/ledsoff", []() {
    Off_page();
    proc_activo = 0;
    //print_proc(proc_activo);
    strip.fill(strip.Color(0, 0, 0));
    strip.show();
  });

  server.onNotFound(handleNotFound);

  server.begin();

  //Serial.println("HTTP Server iniciado");

  strip.begin();
  for (int i = 0; i < 20; i++)
  {
    strip.setPixelColor(i, 200, 0, 0);
  }
  strip.show();
  delay(2000);
  strip.fill(strip.Color(0, 0, 0));
  strip.show();

  Serial.println("Paso 5");
}

void loop()
{
  server.handleClient();

  // Actualiza hora
  timeClient.update();

  if (proc_activo == 1)
  {
    arcoiris();
  }
  if (proc_activo == 2)
  {
    patron(1);
  }
  if (proc_activo == 3)
  {
    on_off();
  }
  // Revisamos si se debe encender el indicador de hora
  avisa_hora();
}

void ConfigLocal()
{
  server.send_P(200, "text/html", Configpage_1);
  // Serial.println("Entraron a Config");
  // Cargar_Credenciales();
  // String IP_Show_1, IP_Show_2;
  // IP_Show_1 = "El IP asignado por su network es: ";
  // IP_Show_2 = "<p>La red y password que se estan usando son: " + Red_String + " / " + Password_String + "</p>";
  // if (WiFi.status() == WL_CONNECTED)
  // {
  //   IP_Show = WiFi.localIP().toString().c_str();
  // }
  // else
  // {
  //   IP_Show = "IP NO ASIGNADO";
  // }
  // String message = ConfigPage_1 + IP_Show_1 + IP_Show + IP_Show_2 + IP_Show_html;
  // server.send(200, "text/html", message);
}

void handleSettingsUpdate()
{
  avisa_leds();
  String data = server.arg("plain");
  DynamicJsonBuffer jBuffer;
  JsonObject &jObject = jBuffer.parseObject(data);

  File configFile = LittleFS.open("/config.json", "w");
  jObject.printTo(configFile);
  configFile.close();

  server.send(200, "application/json", "{\"status\" : \"ok\"}");
  Serial.println("**Actualizan datos**");
  Serial.println(data);
  //  display.display();
  delay(500);
}

void Cargar_Credenciales()
{
  if (LittleFS.exists("/config.json"))
  {
    File configFile = LittleFS.open("/config.json", "r");
    if (configFile)
    {
      size_t size = configFile.size();
      std::unique_ptr<char[]> buf(new char[size]);
      configFile.readBytes(buf.get(), size);
      configFile.close();
      DynamicJsonBuffer jsonBuffer;
      JsonObject &jObject = jsonBuffer.parseObject(buf.get());
      if (jObject.success())
      {
        _ssid = jObject["ssid"];
        _pass = jObject["password"];
        Red_String = String(_ssid);
        Password_String = String(_pass);
        Serial.println("Succesful!");
        Serial.print("Red: ");
        Serial.println(Red_String);
        Serial.print("Password: ");
        Serial.println(Password_String);
      }
      else
      {
        Serial.println("No se logró extraer");
      }
    }
  }
  else
  {
    Serial.println("No hay archivo");
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//*****FUNCIONES DE LUCES SERVER*****
void Inicio()
{
  String message = Head(brillo) + "</center></body></html>";
  server.send(200, "text/html", message);
}

String Head(int br)
{
  String constructor = Head_1 + String(br) + Head_2;
  return constructor;
}

void Arco_page()
{
  String message = Head(brillo) + "<h2>Arcoiris Activo</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void Wave_page()
{
  String message = Head(brillo) + "<h2>Movimiento Activo</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void Onoff_page()
{
  String message = Head(brillo) + "<h2>On Off Activo</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void Color_page()
{
  String message = Head(brillo) + "<h2>Color fijo</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void Brillo_page()
{
  String message = Head(brillo) + "<h2>Brillo ajustado</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void Off_page()
{
  String message = Head(brillo) + "<h2>Leds Apagados</h2></center></body></html>";
  server.send(200, "text/html", message);
}

void read_ip_web()
{
  String web_1, web_2, web_3, web_4;
  if (WiFi.status() == WL_CONNECTED)
  {
    web_1 = WiFi.localIP().toString().c_str();
  }
  else
  {
    web_1 = "IP NO ASIGNADO";
  }
  web_2 = String(_ssid);
  web_3 = String(_pass);
  web_4 = web_1 + "***" + Red_String + "***" + Password_String;
  Serial.print("Datos:");

  Serial.println(web_4);
  Serial.println(Red_String);
  Serial.println(Password_String);

  server.send(200, "text/plain", web_4);
}

//*****FUNCIONES DE LUCES CONTROL*****
void print_proc(int a)
{
  // Serial.println("----------------");
  // Serial.print("Proceso activo: ");
  // Serial.println(a);
}

void arcoiris()
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    int color_led = activo + grupo * i;
    int color = strip.gamma32(strip.ColorHSV(color_led, 150, brillo));
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(20);
  activo += grupo;
}

void patron(unsigned int ciclos)
{
  int color, hue_val;
  for (unsigned int j = 0; j < ciclos; j++)
  {
    for (int a = 0; a < strip.numPixels(); a += 1)
    {
      hue_val = random(0, 65535);
      color = strip.gamma32(strip.ColorHSV(hue_val, 150, brillo));
      strip.setPixelColor(a, color);
    }
    strip.show();
    delay(500);
  }
}

void on_off()
{
  uint32_t nuevo_color = strip.Color(255, 255, 255);
  strip.fill(nuevo_color);
  strip.show();
  delay(200);
  strip.fill(0);
  strip.show();
  delay(200);
}

void led_hora()
{
  int color, hue_val;
  for (unsigned int j = 0; j < 1; j++)
  {
    for (int a = 0; a < strip.numPixels(); a++)
    {
      hue_val = random(0, 65535);
      color = strip.gamma32(strip.ColorHSV(hue_val, 150, brillo_2));
      strip.setPixelColor(a, color);
    }
    strip.show();
    delay(500);
  }
}

// Enciende luces por 15 segundos a la hora en los sigueintes horarios:
// Sabados y domingos de 5am a 11pm
// De lunes a viernes de 6am a 8am y de 6pm a 9pm
void avisa_hora()
{
  //print_proc(proc_activo);
  //  Serial.print("Tiempo: ");
  //  Serial.print(timeClient.getHours());
  //  Serial.print(":");
  //  Serial.print(timeClient.getMinutes());
  //  Serial.print(":");
  //  Serial.println(timeClient.getSeconds());
  if (proc_activo == 0 or proc_activo == 100)
  {
    if (timeClient.getHours() >= 5 and timeClient.getHours() <= 23 and timeClient.getMinutes() == 0)
    {
      //    if(timeClient.getHours() >= 5 and timeClient.getHours() <= 23){
      if (timeClient.getSeconds() <= 15)
      {
        proc_activo = 100; // Se asigna proc_activo = 100 para utilizar la misma variable
        led_hora();
        delay(100);
      }
      else
      {
        if (timeClient.getSeconds() > 15 and timeClient.getSeconds() <= 18)
        {
          proc_activo = 0;
          strip.clear();
          strip.show();
        }
      }
    }
  }
}

void avisa_leds()
{
  for (int a = 0; a < 3; a++)
  {
    strip.fill(strip.Color(0, 255, 0));
    strip.show();
    delay(500);
    strip.fill(0);
    strip.show();
    delay(500);
  }
}