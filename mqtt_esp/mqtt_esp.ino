//Programa: Modulo ESP32 Wifi com MQTT
//Autor: Arduino e Cia
#include <WiFi.h> // CHAMA A BIBLIOTECA COM FUNÇÕES PARA CONECTAR COM O WIFI.
#include <PubSubClient.h> // CHAMA A BIBLIOTECA COM FUNÇÕES PARA CONECTAR COM O BROKER COMO SUBSCRIBER OU PUBLISHER.
const char* ssid = "metralha"; // NOME DA REDE WIFI QUE DESEJA SE CONECTAR.
const char* password =  "MAXMEL2013"; // SENHA DA REDE WIFI QUE DESEJA SE CONECTAR.
const char* mqttServer = "192.168.15.3"; // IP DA REDE, PARA CONECTAR COM O LOCALHOST(SERVIDOR LOCAL CRIADO PELO COMPUTOR).
const int mqttPort = 1883; // PORTA DE REDE ONDE IRA SE COMUNICAR COM O SERVIDOR LOCAL.
const char* mqttUser = "broker"; // NOME DO BROKER.
const char* mqttPassword = "5081"; // SENHA DO BROKER.
WiFiClient espClient;
PubSubClient client(espClient);
String msg,n = "chegou";

void setup()
{
  Serial.begin(115200); // CONEXÃO COM O MONITOR SERIAL(PARA IR MONITORANDO O QUE ESTA OCORRENDO DENTRO DA PLACA).
  
  wifi(); // CHAMA A FUNÇÃO WIFI.
  
  client.setServer(mqttServer, mqttPort); // PASSA OS PARAMETROS DO SERVIDOR MQTT PARA A FUNÇÃO DE CONFIGURAÇÃO.
}

void loop()
{
  if(n == "chegou")
  {
    conecta_publisher();
    
    if (!client.connected()) {
      conecta_publisher(); //Faz a conexao com o broker MQTT
    }
  
    Publisher();
  }
  
  if(n == "enviou")
  {
    conecta_subscribe();
    
    if (!client.connected()) 
    {
      conecta_subscribe(); //Faz a conexao com o broker MQTT
    }
     
    client.setCallback(Subscribe);
    client.loop();
  }
}

void  wifi() // CONECTA COM A INTERNET.
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }
  Serial.println("Conectado na rede WiFi!");
}

String Subscribe(char* topic, byte* payload, unsigned int length) 
{ 
  //obtem a string do payload recebido
  for(int i = 0; i < length; i++) 
  {
    char c = (char)payload[i];
    msg += c;
  }

  n = "chegou";
  Serial.println(msg);
  return msg,n;
}

String Publisher()
{
  client.publish("topico_teste", "oi");
  Serial.println("Mensagem enviada com sucesso...");
  n = "enviou";

  return n;
  delay(3000); //Aguarda 30 segundos para enviar uma nova mensagem
}

void conecta_subscribe()
{
  Serial.println("Conectando ao broker MQTT como Subscribe...");
  if (client.connect("ESP32Client", mqttUser, mqttPassword ))
  {
    Serial.println("Conectado ao broker!");
    client.subscribe("topico_teste");
  }
  else
  {
    Serial.println("Falha na conexao ao broker - Estado: ");
    Serial.print(client.state());
    delay(2000);
  }
}

void conecta_publisher()
{
  Serial.println("Conectando ao broker MQTT como Publisher...");
  if (client.connect("ESP32Client", mqttUser, mqttPassword ))
  {
    Serial.println("Conectado ao broker!");
  }
  else
  {
    Serial.println("Falha na conexao ao broker - Estado: ");
    Serial.print(client.state());
    delay(2000);
  }
}
