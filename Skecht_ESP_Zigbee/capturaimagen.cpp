#include "capturaimagen.h"
//incluir liberia para manejar la camara
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include "base64.h"

camera_config_t config; //Stores the camera configuration parameters

capturaimagen::capturaimagen(/* args */)
{
    statusError = 0;
}

unsigned char capturaimagen::getStatusError(){
    return statusError;
}

capturaimagen::~capturaimagen()
{
}

void capturaimagen::begin(){
    // --> Configuracoes da camera
  //camera_config_t config;
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


  // --> Frame size:
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_HQVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  }

  // --> Inicialize Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    statusError = 1;
  }
}

void capturaimagen::capturaImagen(){
    
  camera_fb_t * fb = NULL;
  // Take Picture with Camera
  fb = esp_camera_fb_get();
  statusError = 0;
  if (!fb) {
    Serial.println("Camera capture failed");
    statusError = 1;
  }
  else {
    Serial.println("Imagen capturada");
    
    dataImageEncodingBase64 = encodingB64Imagen(fb->buf,fb->len);
    /*
    Serial.print("-------------------------------------");
    Serial.print("Tamaño del encoding : "); Serial.println(dataImageEncodingBase64.length());
    Serial.println(dataImageEncodingBase64);
    Serial.print("-------------------------------------");
    */
    
  }
  esp_camera_fb_return(fb);


}


String capturaimagen::encodingB64Imagen(uint8_t *data_pic,size_t size_pic){
  //will be allocated on the heap. Takes about 4/3 of the input size, so basically it doubles your memory requirements
  String imgDataB64 = base64::encode(data_pic, size_pic);
  imgDataB64 = "data:image/jpeg;base64," + imgDataB64;
  dataImageEncodingBase64Status = 0xff;
  return imgDataB64;
}


unsigned char capturaimagen::getAvailableEncodingB64Imagen(){
  return dataImageEncodingBase64Status;
}

String capturaimagen::getdataEncodingB64Imagen(){
  dataImageEncodingBase64Status = 0;
  return dataImageEncodingBase64;
}