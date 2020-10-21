#include <WiFi.h> // CHAMA A BIBLIOTECA COM FUNÇÕES PARA CONECTAR COM O WIFI.
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ESPino32CAM.h"
#include "ESPino32CAM_QRCode.h"

WiFiClient espClient; // CRIA O CLIENTE QUE VAI SE CONECTAR AO WIFI.
PubSubClient Client(espClient); // CRIA O CLIENTE QUE VAI SE CONECTAR AO BROKER.
Adafruit_SSD1306 display(128, 64);

ESPino32CAM cam;   //Objeto para captura de imagem
ESPino32QRCode qr; //Objeto para decoficação da imagem
String msg,leitura;
String L[4];

//Define os pinos da câmera
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
#define flash 4

void setup()
{
  Display();
  wifi(); // CHAMA A FUNÇÃO WIFI.
  Client.setServer("192.168.15.3",1883); // PASSA OS PARAMETROS DO SERVIDOR MQTT PARA A FUNÇÃO DE CONFIGURAÇÃO.
  Client.setCallback(Subscribe);
  broker();
  pinMode(flash,OUTPUT);
  digitalWrite(flash,LOW);//Desliga o flash
     
  //Configura os pinos da câmera
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
  config.jpeg_quality = 4;
  config.fb_count = 1;
   
  esp_err_t err = esp_camera_init(&config); //Inicialização da câmera
  if (err != ESP_OK) {
    Serial.printf("O início da câmera falhou com erro 0x%x", err);//Informa erro se a câmera não for iniciada corretamente
    delay(1000);
    ESP.restart();//Reinicia o ESP 
  }
  //Inicializa o objeto de decodificação
  qr.init(&cam);
  sensor_t *s = cam.sensor();
  s->set_framesize(s, FRAMESIZE_CIF);
  s->set_whitebal(s, true);
}

void loop()
{ 
  wifi();
  broker();
  Client.loop();
  camera();
}

void  wifi() // CONECTA COM A INTERNET.
{
  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
  
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin("metralha", "MAXMEL2013");
    delay(500);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 0);
    display.println("Conectando WiFi...");
    display.setCursor(0, 8);
    display.println("                    ");
    display.display(); 
  }
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 8);
  display.println("WiFi = ok");
  display.display();
  delay(1000);
}

void broker()
{
  while(!Client.connected())
  {
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 0);
    display.println("Conectando Broker...");
    display.setCursor(0, 8);
    display.println("                    ");
    display.display(); 
    if (Client.connect("ESP32Client", "broker", "5081"))
    {
      Client.subscribe("envia");
      display.setTextColor(WHITE,BLACK);
      display.setCursor(0, 8);
      display.println("Broker = ok");
      display.display();
      delay(1000); 
    }
    else
    {
      Serial.println("Falha na conexao ao broker - Estado: ");
      Serial.print(Client.state());
      delay(1000);
    }
  }
}

void Subscribe(char* topic, byte* payload, unsigned int length)
{
  int x = 0;
  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i]; // broca;10;10/20/30;12:30:12:00;
    if (c == ';'){
      L[x] = msg;
      msg = "";
      x++; 
    }
    else{
      msg += c;
    }
  } 

  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 16);
  for(int i = 0;i < (sizeof L / sizeof *L);i++){
    display.println(L[i]+"         ");
  }
  display.display(); 
}

void Display()
{
  Wire.begin(14,15);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// Address 0x3D for 128x64
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void camera()
{
  unsigned long pv_time  = millis();
  camera_fb_t *fb = cam.capture(); //Captura a imagem
  if (!fb)
  {
    return;
  }
  dl_matrix3du_t *rgb888, *rgb565;
  if (cam.jpg2rgb(fb, &rgb888))
  {
    rgb565 = cam.rgb565(rgb888);
  }
  cam.clearMemory(rgb888);
  cam.clearMemory(rgb565);
  dl_matrix3du_t *image_rgb;
  if (cam.jpg2rgb(fb, &image_rgb))
  {
    cam.clearMemory(fb);
         
    qrResoult res = qr.recognition(image_rgb); //Faz decodificação da imagem contendo os dados
     
    if (res.status) //Se conseguir decodificar a imagem mostra os dados na tela
    { 
       leitura = res.payload;//Variável para mostrar os dados contidos no QR Code
       Client.publish("recebe",(char*)leitura.c_str());
       display.setTextColor(WHITE,BLACK);
       display.setCursor(0, 8);
       display.println("QR code lido!");
       display.display();
    }
    else{ //Se não aguarda receber código 
       display.setTextColor(WHITE,BLACK);
       display.setCursor(0, 8);
       display.println("                    ");
       display.setCursor(0, 0);
       display.println("Aguardando QR code...");
       display.display(); 
     }
    } 
  cam.clearMemory(image_rgb); //Apaga imagem para receber uma nova imagem
}
