#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "esp_camera.h"
#include <HTTPClient.h>

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

const char* ssid = "MUHAMMED";
const char* password = "MUHAMMED579fatihsahin";
String ip = "";

AsyncWebServer server(80);

bool initCamera() {

  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t result = esp_camera_init(&config);

  if (result != ESP_OK) {
    return false;
  }

  return true;
}

void setup() {
  Serial.begin(115200);

  if (!initCamera()) {

    Serial.printf("Failed to initialize camera...");
    return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  ip = String(WiFi.localIP());
  Serial.println(ip);

  server.on("/picture.jpg", HTTP_GET, [](AsyncWebServerRequest* request) {
    camera_fb_t* frame = NULL;
    frame = esp_camera_fb_get();

    request->send_P(200, "image/jpeg", (const uint8_t*)frame->buf, frame->len);

    esp_camera_fb_return(frame);
  });
}

void loop() {
  server.begin();

  HTTPClient http;
  http.begin("http://192.168.1.21/esp32/api/add_img.php?img=http://"+ip+"/picture.jpg");                        // POST isteği yapılacak adres
  int httpResponseCode = http.sendRequest("POST");            // POST isteği gönderme
  if (httpResponseCode > 0) {
    Serial.print("HTTP başarı kodu: ");
    //Serial.println(httpResponseCode);
    //String response = http.getString();  // Sunucudan gelen yanıtı almak
    //Serial.println(response);
  } else {
    Serial.print("HTTP hatası: ");
    Serial.println(httpResponseCode);
  }
  http.end();  // HTTP isteği sonlandırma
  delay(10000);
}
