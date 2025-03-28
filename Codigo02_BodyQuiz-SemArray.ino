#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 4); // Endereço 0x20, 16 caracteres, 4 linhas

int pinBotControl = 2;
int pinOrgaos[8] = {3, 4, 5, 6, 7, 8, 9, 10};
int pinLedVermelho = A0;
int pinLedVerde = A1;
int pinBuzzer = A2;

String modoJogo = "Aprendizado";
unsigned long time; // Variável de tempo para a função millis()

// Perguntas como variáveis individuais
const char* pergunta1 = "Bombeia sangue para o corpo?";
const char* pergunta2 = "Troca de oxigenio por CO2?";
const char* pergunta3 = "Filtra sangue e armazena celulas sanguineas?";
const char* pergunta4 = "Produz bile e desintoxica?";
const char* pergunta5 = "Digere alimentos com acidos?";
const char* pergunta6 = "Filtra sangue e produz urina?";
const char* pergunta7 = "Absorve nutrientes dos alimentos?";
const char* pergunta8 = "Produz insulina e regula acucar?";

void setup() {
  lcd.init(); // Inicializa o LCD
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Serial Iniciada.");

  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBotControl, INPUT_PULLUP);

  for (int i = 0; i < 8; i++) { // Definição do PinMode de todos os botões
    pinMode(pinOrgaos[i], INPUT_PULLUP);
  }
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Modo: ");
  lcd.print(modoJogo);
  lcd.setCursor(0, 2);
  lcd.print("-Clique para passar");
  lcd.setCursor(0, 3);
  lcd.print("-Segure para entrar");

  if (digitalRead(pinBotControl) == 0) {
    Serial.println("Botao de controle clicado");

    time = millis();
    while (digitalRead(pinBotControl) == 0 && millis() - time <= 3000) {}

    if (millis() - time >= 3000) {
      Serial.println("Botao de controle pressionado");
      while (digitalRead(pinBotControl) == 0) {}
      funcModoJogo();
    }

    if (modoJogo == "Aprendizado") {
      modoJogo = "Desafio";
      lcd.clear();
    } else if (modoJogo == "Desafio") {
      modoJogo = "Desafio Dupla";
      lcd.clear();
    } else if (modoJogo == "Desafio Dupla") {
      modoJogo = "Aprendizado";
      lcd.clear();
    }
  }
}

void funcModoJogo() {
  lcd.clear();
  if (modoJogo == "Aprendizado") {
    funcAprendizado();
  } else if (modoJogo == "Desafio") {
    funcDesafio();
  } else if (modoJogo == "Desafio Dupla") {
    funcDesafioDupla();
  }
}

void funcAprendizado() {
  int ordemPerguntas[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // Ordem das perguntas
  int pontuacao = 0;

  // Embaralhar a ordem das perguntas
  randomSeed(analogRead(A3));
  for (int i = 7; i > 0; i--) {
    int j = random(i + 1);
    int temp = ordemPerguntas[i];
    ordemPerguntas[i] = ordemPerguntas[j];
    ordemPerguntas[j] = temp;
  }

  // Percorrer as perguntas na ordem aleatória
  for (int q = 0; q < 8; q++) {
    int perguntaIndex = ordemPerguntas[q];

    // Selecionar a pergunta correta sem array de strings
    const char* perguntaAtual;
    switch (perguntaIndex) {
      case 0: perguntaAtual = pergunta1; break;
      case 1: perguntaAtual = pergunta2; break;
      case 2: perguntaAtual = pergunta3; break;
      case 3: perguntaAtual = pergunta4; break;
      case 4: perguntaAtual = pergunta5; break;
      case 5: perguntaAtual = pergunta6; break;
      case 6: perguntaAtual = pergunta7; break;
      case 7: perguntaAtual = pergunta8; break;
      default: perguntaAtual = "Erro"; break;
    }

    // Exibir a pergunta no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qual Orgao:");
    funcExibirNoLCD(perguntaAtual);

    // Aguardar resposta correta ou incorreta
    bool respondeu = false;
    while (!respondeu) {
      for (int i = 0; i < 8; i++) {
        if (digitalRead(pinOrgaos[i]) == 0) { // Se algum botão for pressionado
          lcd.setCursor(0, 2);

          if (i == perguntaIndex) { // Se for a resposta correta
            lcd.clear();
            lcd.print("Resposta correta!");
            digitalWrite(pinLedVerde, 1);
            pontuacao++;
          } else { // Se for a resposta errada
            lcd.clear();
            lcd.print("Respota incorreta!");
            digitalWrite(pinLedVermelho, 1);
            digitalWrite(pinBuzzer, 1);
          }

          // Atualiza a pontuação na linha 4 do LCD
          lcd.setCursor(0, 3);
          lcd.print("Pontuacao: ");
          lcd.print(pontuacao);

          delay(2000); // Pequeno delay para visualização
          digitalWrite(pinLedVermelho, 0);
          digitalWrite(pinLedVerde, 0);
          digitalWrite(pinBuzzer, 0);
          respondeu = true;
          break;
        }
      }
    }
  }

  // Mensagem final ao completar as 8 perguntas
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parabens!");
  lcd.setCursor(0, 1);
  lcd.print("Modo concluido!");
  Serial.println("Aprendizado concluido!");
  delay(3000);
}

void funcDesafio() {
  // Implementação futura
}

void funcDesafioDupla() {
  // Implementação futura
}

void funcExibirNoLCD(const char* mensagem) { // Função auxiliar para impressão no LCD.
  int i = 0, linha = 1;

  while (mensagem[i] != '\0' && linha < 4) { // Enquanto houver caracteres e linhas disponíveis
    lcd.setCursor(0, linha); // Move o cursor para a linha correta
    for (int j = 0; j < 16 && mensagem[i] != '\0'; j++, i++) {
      lcd.print(mensagem[i]); // Escreve caractere por caractere
    }
    linha++; // Vai para a próxima linha após 16 caracteres
  }
}
