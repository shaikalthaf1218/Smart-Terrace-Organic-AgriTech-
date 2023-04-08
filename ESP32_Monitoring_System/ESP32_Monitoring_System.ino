#define BLYNK_TEMPLATE_ID           "TMPL3QhznlFSl"
#define BLYNK_TEMPLATE_NAME         "Terrace AgriTech"
#define BLYNK_AUTH_TOKEN            "Q43UaKt6Pwalqp_0WGM7p6zSczMgM1Y4"

#define BLYNK_PRINT Serial

#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "DESKTOP-5D0U0VN 3036";
char pass[] = "Uy82,210";

WiFiClient client;

WidgetLCD lcd(V6);

#define DHTPIN 5          
#define DHTTYPE DHT11
#define Soil 34
#define Rain 35  

float h;
float t;
float s;
float r;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  s = 100 - ((analogRead(34)/4024)*100);
  r = 100 - ((analogRead(35)/4024)*100);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V0, s);
  Blynk.virtualWrite(V3, r);
  LCD();
}

void LCD()
{
  if ((t < 32.00) and (h > 45.00) and (s > 45.00) and (r > 40.00))
  {
    lcd.clear();
    lcd.print(0, 0, "Crop Has");
    lcd.print(0, 1, "Enough Water");
    delay(2000);
  }

  else if ((t < 32.00) and (h > 45.00) and (s < 43.00))
  {
    lcd.clear();
    lcd.print(0, 0, "Temp = ");
    lcd.print(7, 0, t);
    lcd.print(11, 0, "*C");
    lcd.print(0, 1, "Soil Is Dry");
    delay(2000);

    lcd.clear();
    lcd.print(0, 0, "Humid = ");
    lcd.print(7, 0, h);
    lcd.print(12, 0, "%");
    lcd.print(0, 1, "Soil Is Dry");
    delay(2000);
  }

  else
  {
    lcd.clear();
    lcd.print(0, 0, "Temp Is High");
    lcd.print(4, 1, "Alert !");
    delay(1500);

    lcd.clear();
    lcd.print(0, 0, "Humid Is Low");
    lcd.print(4, 1, "Alert !");
    delay(1500);

    lcd.clear();
    lcd.print(0, 0, "Soil Is Dry");
    lcd.print(4, 1, "Alert !");
    delay(1500);

    lcd.clear();
    lcd.print(0, 0, "No Rain");
    lcd.print(4, 1, "Alert !");
    delay(1500);

    lcd.clear();
    lcd.print(0, 0, "Watering Has");
    lcd.print(0, 1, "To Be Done");
    delay(1500);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(34,INPUT);
  pinMode(35,INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();

  timer.setInterval(1000L, sendSensor);

  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(2000);

}

void loop()
{
  Blynk.run();
  timer.run();
}
