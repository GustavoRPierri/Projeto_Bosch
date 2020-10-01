#include <WiFi.h> // CHAMA A BIBLIOTECA COM FUNÇÕES PARA CONECTAR COM O WIFI.
#include <PubSubClient.h>

WiFiClient espClient; // CRIA O CLIENTE QUE VAI SE CONECTAR AO WIFI.
PubSubClient Client(espClient); // CRIA O CLIENTE QUE VAI SE CONECTAR AO BROKER.

const char* mqttServer = "192.168.15.3"; // IP DA REDE, PARA CONECTAR COM O LOCALHOST(SERVIDOR LOCAL CRIADO PELO COMPUTOR).
const int mqttPort = 1883; // PORTA DE REDE ONDE IRA SE COMUNICAR COM O SERVIDOR LOCAL.
const char* mqttUser = "broker"; // NOME DO BROKER.
const char* mqttPassword = "5081"; // SENHA DO BROKER.
String msg;

void setup()
{
  Serial.begin(115200); // CONEXÃO COM O MONITOR SERIAL(PARA IR MONITORANDO O QUE ESTA OCORRENDO DENTRO DA PLACA).
  wifi(); // CHAMA A FUNÇÃO WIFI.
  Client.setServer(mqttServer,mqttPort); // PASSA OS PARAMETROS DO SERVIDOR MQTT PARA A FUNÇÃO DE CONFIGURAÇÃO.
  Client.setCallback(Subscribe);
  broker();
}

void loop()
{ 
  wifi();
  broker();
  Client.loop();
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
    Serial.println("Iniciando conexao com a rede WiFi..");
  }
  Serial.println("Conectado na rede WiFi!");
}

void broker()
{
  while(!Client.connected())
  {
    Serial.println("Conectando ao broker...");
    if (Client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("Conectado ao broker!");
      Client.subscribe("topico_teste");
    }
    else
    {
      Serial.println("Falha na conexao ao broker - Estado: ");
      Serial.print(Client.state());
      delay(2000);
      Serial.println("Nova tentativa de conexao:");
      delay(3000);
    }
  }
}

void Subscribe(char* topic, byte* payload, unsigned int length)
{
  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.println(msg);
}
