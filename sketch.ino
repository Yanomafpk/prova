#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pinoBotao = 18;  // Pino referente ao botão
int estadoBotao = 0;  // Variável do estado do botão

const int pinoldr = 4;  // Pino referente ao sensor de luminosidade
int threshold=600;

int estusLDR=analogRead(pinoldr);

unsigned long lastDebounceTime = 0; // última vez que o botão foi pressionado
unsigned long debounceDelay = 70;   // O intervalo, igual ao delay do código anterior
int countClicks = 0;     // variável para contar o número de clicks no botão

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  // Inicialização das entradas


  pinMode(pinoBotao, INPUT);

  

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  estadoBotao = digitalRead(pinoBotao);
  if (estadoBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  

  
  if (estusLDR>=threshold && led_vermelho == HIGH && estadoBotao == HIGH) {
    delay(1000);
    digitalWrite(led_verde, HIGH);

  }
  else if(estusLDR<=threshold && estadoBotao == HIGH){
    Serial.print("seu led escuro acende");
    Serial.println(estusLDR);
    digitalWrite(led_amarelo, HIGH); // Liga o LED
    delay(1000);
    digitalWrite(led_amarelo, LOW); // Desliga o LED
    delay(1000);

  }else{
    
    Serial.print("sua luz brilhante apaga");
    Serial.println(estusLDR);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_vermelho, LOW);
    delay(3000);
    digitalWrite(led_amarelo, HIGH);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, LOW);
    delay(2000);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, HIGH);
    delay(5000);
  }

  if (estadoBotao == HIGH) {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    countClicks++;
  }
  lastDebounceTime = millis();
  }
  HTTPClient http;
  String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP
  // enviar uma requisição HTTP
  if (countClicks >= 3 && estusLDR>=threshold && led_vermelho == HIGH) {
    http.begin(serverPath.c_str());
  } 
}