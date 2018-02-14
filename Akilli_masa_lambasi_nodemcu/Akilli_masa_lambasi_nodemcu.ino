/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>   
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "bilinmez"  //wifi kullanıcı adı
#define WLAN_PASS       "şifre" //wifi şifresi

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com" //mqtt sunucu adresi (biz adafruit kullanacağımız için adafruit.io'nun bize vermiş olduğu adresi girdik)
#define AIO_SERVERPORT  1883                 // mqtt sunucu portu (varsayılan port numarası 1883) 
#define AIO_USERNAME    "ADAFRUIT IO KULLANICI ADI" // oluşturduğumuz  adafruit kullanıcı adı 
#define AIO_KEY         "ADAFRUIT IO KEY" // Adafruit io key (Resim 10 üzerinde nasıl alacağınız mevcut mevcut)


WiFiClient client; // wifi istemcisi oluşturuldu

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); // adafruit mqtt istemcisi oluşturuldu

Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/deneme");// adafruit için mqtt publisher oluşturuldu (Şu anda bize lazım değil ama ileriki projelerde gerekli olacak)


Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/deneme1"); // veri dinleyicisi oluşturuldu ( Tırnak içerisindeki bölüme resim 2-4 arasında oluşturduğumuz ismi yazıyoruz)

void MQTT_connect();

void setup() {
  pinMode(D6,OUTPUT); // 6 numaralı pin çıkış olarak ayarlandı
  Serial.begin(115200);// serial haberleşme başlatıldı (Bu gerekli değil ama  veri akışını ekranda izlemek için lazım olacak)
  delay(10);

  Serial.println(F("Adafruit MQTT akıllı ev başlangıç 1 demo"));

 
  Serial.println(); Serial.println();
  Serial.print("Buraya bağlanılıyor ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS); // wifi bağlantısı kuruluyor
  while (WiFi.status() != WL_CONNECTED) {  // wifi bağlantısı kurulana kadar ekranda . yazdırıldı (Serial console üzerinde sürekli .... yazıyorsa wifi bağlantısı başarılı olmamıştır)
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi Bağlandı");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&onoffbutton); // mqtt veri dinleyicisi başlatıldı
}

uint32_t x=0;

void loop() {

  MQTT_connect();// adafruit mqtt sunucusuna bağlanıyor 


  Adafruit_MQTT_Subscribe *subscription;    // Buradan döngü sonuna kadar   adafruit üzerinden gelen veriler dinleniyor(Aç butonuna basınca Açık verisinin gelmesi gibi)
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      String Gelenveri=String((char *)onoffbutton.lastread); // Gelen veri string formata çevrildi
      if(Gelenveri=="Açık") // Gelen veri Açık ise (Buradaki açık yazısı bizim  resim 8 üzerinde oluşturduğumuz butona verdiğimiz isim)
      {
        digitalWrite(D6,1); // Gelen veri açık ise   6 numaralı pini yani   lambanın bağlı olduğu röleyi aç
        }
        else
        {
          digitalWrite(D6,0); //  Gelen veri açık değil ise lambayı kapat
          }
      
    }
  }

 
 // Serial.print(F("\nSending photocell val "));
 // Serial.print(x);
 // Serial.print("...");
 // if (! photocell.publish(x++)) {
 //   Serial.println(F("Failed"));
 // } else {
 //   Serial.println(F("OK!"));
 // }

}

void MQTT_connect() { //  Burası aşağıya doğru mqtt bağlantısını kurmak için. Burayı hazır olarak kullanabilirsiniz
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Bağlandı!");
}
