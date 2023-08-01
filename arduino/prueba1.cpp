#include <ESP8266WiFi.h>

#define LED1 D0
#define LED2 D4

const char *wifi_ssid = "ejemploSSID";
const char *wifi_pass = "ejemploPASS";

void wifi_connect(const char *, const char *);


void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.println("Initializing WiFi");
  wifi_connect(wifi_ssid, wifi_pass);
}

void loop() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(500);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  delay(500);
}


void wifi_connect(const char *ssid, const char *pass) {
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("\t\t[*] Connecting to: ");
  Serial.print(WiFi.SSID());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("\t\t[+] Connection established. [");
  Serial.print(WiFi.localIP());
  Serial.println("]");
}
