// Librerías para la cámara
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
// Librerías para la trajeta MicroSD
#include "FS.h"
#include "SD_MMC.h"
// Librerías para me memoria EEPROM 
#include "EEPROM.h"
// Utilice 1 byte de espacio EEPROM
#define EEPROM_SIZE 1
// Contador de número de imagen
unsigned int contadorFoto = 0;
// definiciones de pines para CAMERA MODELO AI THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
int pin = 4;
void configuracionCamara() {
  //Configurar los parámetros de la cámara
  // Objeto para almacenar los parámetros de configuración de la cámara.
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
  config.pixel_format = PIXFORMAT_JPEG; // Las opciones son YUV422, ESCALA DE GRISES, RGB565, JPEG
// Seleccione un tamaño de fotograma más bajo si la cámara no es compatible con PSRAM
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // TAMAÑO DEL MARCO_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10; //10-63 un número más bajo significa mayor calidad
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
// Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("El inicio de la cámara falló con el error 0x%x", err);
    return;
  }
// Ajustes de calidad de la cámara.
  sensor_t * seteoCamara = esp_camera_sensor_get();
  // BRILLO (-2 to 2)
  seteoCamara->set_brightness(seteoCamara, 0);
  // CONTRASTE (-2 to 2)
  seteoCamara->set_contrast(seteoCamara, 0);
  // SATURACIÓN (-2 to 2)
  seteoCamara->set_saturation(seteoCamara, 0);
  // EFECTOS ESPECIALES (0 - Sin efecto, 1 - Negativo, 2 - Escala de grises, 3 - Tinte rojo, 4 - Tinte verde, 5 - Tinte azul, 6 - Sepia)
  seteoCamara->set_special_effect(seteoCamara, 0);
  //BALANCE DE BLANCOS (0 = Desactivar, 1 = Activar)
  seteoCamara->set_whitebal(seteoCamara, 1);
  // GANANCIA AWB (0 = Desactivar, 1 = Activar)
  seteoCamara->set_awb_gain(seteoCamara, 1);
  // MODOS WB (0 - Automático, 1 - Soleado, 2 - Nublado, 3 - Oficina, 4 - Hogar)
  seteoCamara->set_wb_mode(seteoCamara, 0);
  // CONTROLES DE EXPOSICIÓN (0 = Desactivar, 1 = Activar)
  seteoCamara->set_exposure_ctrl(seteoCamara, 1);
  // AEC2 (0 = Desactivar, 1 = Activar)
  seteoCamara->set_aec2(seteoCamara, 0);
  // NIVELES DE AE (-2 a 2)
  seteoCamara->set_ae_level(seteoCamara, 0);
  // VALORES AEC (0 a 1200)
  seteoCamara->set_aec_value(seteoCamara, 300);
  // CONTROLES DE GANANCIA (0 = Desactivar, 1 = Activar)
  seteoCamara->set_gain_ctrl(seteoCamara, 1);
  // GANANCIA AGC (0 a 30)
  seteoCamara->set_agc_gain(seteoCamara, 0);
  // GANANCIA TECHO (0 a 6)
  seteoCamara->set_gainceiling(seteoCamara, (gainceiling_t)0);
  // BPC (0 = Desactivar, 1 = Activar)
  seteoCamara->set_bpc(seteoCamara, 0);
  // WPC (0 = Desactivar, 1 = Activar)
  seteoCamara->set_wpc(seteoCamara, 1);
  // GMA RAW (0 = Desactivar, 1 = Activar)
  seteoCamara->set_raw_gma(seteoCamara, 1);
  // LENC (0 = Desactivar, 1 = Activar)
  seteoCamara->set_lenc(seteoCamara, 1);
  // ESPEJO ORIZ (0 = Desactivar, 1 = Activar)
  seteoCamara->set_hmirror(seteoCamara, 0);
  // VERT FLIP (0 = Desactivar, 1 = Activar)
  seteoCamara->set_vflip(seteoCamara, 0);
  // DCW (0 = Desactivar, 1 = Activar)
  seteoCamara->set_dcw(seteoCamara, 1);
  // PATRÓN DE BARRA DE COLOR (0 = Desactivar, 1 = Activar)
  seteoCamara->set_colorbar(seteoCamara, 0);
}
void initMicroSDCard() {
  // Inicia la tarjeta MicroSD
 
  Serial.println("Montaje de la tarjeta MicroSD");
  if (!SD_MMC.begin()) {
    Serial.println("Error al montar la tarjeta MicroSD");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No se encontró ninguna tarjeta MicroSD");
    return;
  }
}
void takeNewPhoto(String path) {
  // Tomar una foto con la cámara Configurar frame buffer
  camera_fb_t  * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Falló la captura de la cámara");
    return;
  }
  // Guardar imagen en la tarjeta microSD
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("No se pudo abrir el archivo en modo de escritura");
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Archivo guardado en la ruta: %s\n", path.c_str());
  }
  // Cerrar el archivo
  file.close();
  // Devolver el frame buffer al controlador para su reutilización
  esp_camera_fb_return(fb);
}
void setup() {
   pinMode(pin, OUTPUT);   // declaramos el pin como salida ( OUTPUT )
   digitalWrite (pin,LOW); 
  // Desactivar el detector de caída de tensión
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  // Iniciar monitor serie
  Serial.begin(115200);
  // Inicializa la cámara
  Serial.print("Inicializando el módulo de la cámara...");
  configuracionCamara();
  Serial.println("Camera OK!");
  // Inicializa la MicroSD
  Serial.print("Inicializando el módulo de la tarjeta MicroSD... ");
  initMicroSDCard();
  // inicializar EEPROM con tamaño predefinido
  EEPROM.begin(EEPROM_SIZE);
  contadorFoto = EEPROM.read(0) + 1;
  // Ruta donde se guardará la nueva imagen en la tarjeta SD
  String path = "/foto" + String(contadorFoto) + ".jpg";
  Serial.printf("Nombre del archivo de imagen: %s\n", path.c_str());
  // Tomar y guardar foto
  takeNewPhoto(path);
  // Actualizar el contador de números de imagen EEPROM
  EEPROM.write(0, contadorFoto);
  EEPROM.commit();
  digitalWrite (pin,LOW); 
  // Vincular Wakeup a GPIO13 en estado HIGH
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
  digitalWrite (pin,LOW); 
  Serial.println("Entrar en modo de suspensión");
  delay(1000);
  // Ingrese al modo de sueño profundo
  esp_deep_sleep_start();
}
void loop() {
}