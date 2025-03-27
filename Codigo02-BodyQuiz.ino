#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,4);  // set the LCD address to 0x20 for a 16 chars and 4 line display

int pinBotControl = 2;
int pinOrgaos[8] = {3, 4, 5, 6, 7, 8, 9, 10};
int pinLedVermelho = A0;
int pinLedVerde = A1;
int pinBuzzer = A2;

String modoJogo = "Aprendizado";

unsigned long time;//Variável de tempo para a função millis

  char *perguntasDesafio[] = {
    "E o unico musculo que nunca se cansa?",
    "Tem milhoes de 'saquinhos de ar' chamados alveolos?",
    "Destroi globulos vermelhos velhos e ajuda na imunidade?",
    "Pode se regenerar mesmo se perder parte dele?",
    "Produz acido cloridrico para digerir os alimentos?",
    "Regula a quantidade de agua e sal no sangue?",
    "Tem cerca de 7 metros de comprimento e absorve nutrientes?",
    "Produz enzimas que digerem carboidratos, proteinas e gordura"
  };

void setup() {
  lcd.init();// Inicializa o LCD
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Serial Iniciada.");

  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBotControl, INPUT_PULLUP);

  for(int i = 0; i <= 7; i++){//Definição do PinMode de todos os botões
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

  if(digitalRead(pinBotControl) == 0){
    Serial.println("Botao de controle clicado");

    time = millis();
    while(digitalRead(pinBotControl) == 0 && millis()-time <= 3000){}

    if(millis()-time >= 3000){
      Serial.println("Botao de controle pressionado");
      while(digitalRead(pinBotControl) == 0){}
      funcModoJogo();
    }

    if(modoJogo == "Aprendizado"){
      modoJogo = "Desafio";
      lcd.clear();
    }
    else if(modoJogo == "Desafio"){
      modoJogo = "Desafio Dupla";
      lcd.clear();
    }
    else if(modoJogo == "Desafio Dupla"){
      modoJogo = "Aprendizado";
      lcd.clear();
    }
  }
}

void funcModoJogo(){
  lcd.clear();
  if(modoJogo == "Aprendizado"){
    funcAprendizado();
  }
  else if(modoJogo == "Desafio"){
    funcDesafio();
  }
  else if(modoJogo == "Desafio Dupla"){
    funcDesafioDupla();
  }
}

void funcAprendizado() {
  // Array de perguntas em formato de ponteiro
  char *perguntasAprendizado[] = {
    "Bombeia sangue para o corpo?",
    "Troca de oxigenio por CO2?",
    "Filtra sangue e armazena celulas sanguineas?",
    "Produz bile e desintoxica?",
    "Digere alimentos com acidos?",
    "Filtra sangue e produz urina?",
    "Absorve nutrientes dos alimentos?",
    "Produz insulina e regula acucar?"
  };

  int ordemPerguntas[8]; // Array para armazenar a ordem aleatória das perguntas
  int pontuacao = 0;

  // Preencher o array com índices de 0 a 7
  for (int i = 0; i < 8; i++) {
    ordemPerguntas[i] = i;
  }

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

    // Exibir a pergunta no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Qual Orgao:");
    funcExibirNoLCD(perguntasAprendizado[perguntaIndex]);

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
  // Array de perguntas do modo desafio

  int ordemPerguntas[8]; // Array para armazenar a ordem aleatória das perguntas
  int pontuacao = 0;

  // Preencher o array com índices de 0 a 7
  for (int i = 0; i < 8; i++) {
    ordemPerguntas[i] = i;
  }

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

    // Exibir a pergunta no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Desafio:");
    funcExibirNoLCD(perguntasDesafio[perguntaIndex]);

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
            lcd.print("Resposta incorreta!");
            digitalWrite(pinLedVermelho, 1);
            digitalWrite(pinBuzzer, 1);
          }

          // Atualiza a pontuação na linha 4 do LCD
          lcd.setCursor(0, 3);
          lcd.print("Pontuacao: ");
          lcd.print(pontuacao);

          delay(2000); // Pequeno delay para visualização
          digitalWrite(pinLedVerde, 0);
          digitalWrite(pinLedVermelho, 0);
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
  lcd.print("Modo finalizado!");
  Serial.println("Desafio concluído!");
  delay(3000);
}


void funcDesafioDupla() {


  int ordemPerguntas[8]; // Array para armazenar a ordem aleatória das perguntas
  int pontuacaoJogador1 = 0;
  int pontuacaoJogador2 = 0;
  int jogadorAtual = 1; // Alterna entre 1 e 2

  // Preencher o array com índices de 0 a 7
  for (int i = 0; i < 8; i++) {
    ordemPerguntas[i] = i;
  }

  // Embaralhar a ordem das perguntas
  randomSeed(analogRead(A3));
  for (int i = 7; i > 0; i--) {
    int j = random(i + 1);
    int temp = ordemPerguntas[i];
    ordemPerguntas[i] = ordemPerguntas[j];
    ordemPerguntas[j] = temp;
  }

  // Percorrer as perguntas até que um dos jogadores atinja 4 pontos
  for (int q = 0; q < 8; q++) {
    int perguntaIndex = ordemPerguntas[q];

    // Exibir a pergunta no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jogador ");
    lcd.print(jogadorAtual);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print("Qual orgao");
    funcExibirNoLCD(perguntasDesafio[perguntaIndex]);

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
            if (jogadorAtual == 1) {
              pontuacaoJogador1++;
            } else {
              pontuacaoJogador2++;
            }
          } else { // Se for a resposta errada
            lcd.clear();
            lcd.print("Resposta incorreta!");
            digitalWrite(pinLedVermelho, 1);
            digitalWrite(pinBuzzer, 1);
          }

          // Atualiza a pontuação na linha 4 do LCD
          lcd.setCursor(0, 3);
          lcd.print("J1: ");
          lcd.print(pontuacaoJogador1);
          lcd.print("  J2: ");
          lcd.print(pontuacaoJogador2);

          delay(2000); // Pequeno delay para visualização
          digitalWrite(pinLedVerde, 0);
          digitalWrite(pinLedVermelho, 0);
          digitalWrite(pinBuzzer, 0);
          respondeu = true;
          break;
        }
      }
    }

    // Verificar se algum jogador venceu
    if (pontuacaoJogador1 == 4 || pontuacaoJogador2 == 4) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vencedor: Jogador ");
      lcd.print(pontuacaoJogador1 == 4 ? 1 : 2);
      Serial.println("Desafio concluído!");
      delay(3000);
      return; // Sai da função ao encontrar um vencedor
    }

    // Alternar o jogador para a próxima rodada
    jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
  }

  // Se nenhum jogador chegar a 4 pontos, exibe mensagem de fim do jogo
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fim do Desafio!");
  Serial.println("Desafio concluído!");
}



void funcExibirNoLCD(char *mensagem) {//Função auxiliar para impressão no lcd.
  int i = 0, linha = 1;

  while (mensagem[i] != '\0' && linha < 4) { // Enquanto houver caracteres e linhas disponíveis
    lcd.setCursor(0, linha); // Move o cursor para a linha correta
    for (int j = 0; j < 20 && mensagem[i] != '\0'; j++, i++) {
    lcd.print(mensagem[i]); // Escreve caractere por caractere
    }
    linha++; // Vai para a próxima linha após 20 caracteres
    }
}