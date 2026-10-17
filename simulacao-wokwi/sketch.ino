/*
  Simulacao da Impressora de Etiquetas CNC (Wokwi)

  - Motor do TAMBOR (gira a fita)   -> driver no eixo X do Shield CNC
  - Motor da CANETA (eixo linear)   -> driver no eixo Y do Shield CNC
  - Servo SG90 sobe/desce a caneta
  - Tela OLED faz o papel da FITA: quando a caneta esta abaixada,
    o ponto onde ela esta fica aceso na tela

  Na tela:  horizontal = tambor girando (comprimento da etiqueta)
            vertical   = caneta andando (altura da letra)

  Como usar: ao iniciar, ele escreve "UEFS".
  Depois, digite uma palavra no Serial Monitor e aperte Enter.
  Letras aceitas: A B C D E F G H I L M N O P R S T U V e espaco.

  No Arduino de verdade: e so apagar as linhas da tela (tela.xxx)
  que o resto continua funcionando igual.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// ---------- Pinos (mesmos do Shield CNC v3) ----------
const int TAMBOR_STEP = 2;   // eixo X do shield
const int TAMBOR_DIR  = 5;
const int CANETA_STEP = 3;   // eixo Y do shield
const int CANETA_DIR  = 6;
const int ENABLE_PIN  = 8;   // LOW = drivers ligados
const int SERVO_PIN   = 11;

// ---------- Ajustes ----------
const int CANETA_CIMA  = 90;   // angulo do servo com a caneta levantada
const int CANETA_BAIXO = 30;   // angulo do servo com a caneta encostando na fita

const int ESCALA = 3;          // quantos passos (pixels) vale 1 quadradinho da letra
const int ESPACO_LETRA = 6;    // cada letra ocupa 6 quadradinhos (4 de letra + 2 de espaco)
const int MAX_LETRAS = 7;      // cabe na tela de 128 pixels
const int BASE = 42;           // linha de baixo das letras na tela
const int TEMPO_PASSO_US = 1500;

// ---------- Objetos ----------
Adafruit_SSD1306 tela(128, 64, &Wire, -1);
Servo servo;

// ---------- Posicao atual da maquina ----------
int posTambor = 0;        // em passos (= pixel horizontal na tela)
int posCaneta = BASE;     // em passos (= pixel vertical na tela)
bool canetaAbaixada = false;
int inicioLetra = 0;      // onde comeca a letra que esta sendo escrita

void setup() {
  pinMode(TAMBOR_STEP, OUTPUT);
  pinMode(TAMBOR_DIR, OUTPUT);
  pinMode(CANETA_STEP, OUTPUT);
  pinMode(CANETA_DIR, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);   // liga os dois drivers

  servo.attach(SERVO_PIN);
  servo.write(CANETA_CIMA);

  Serial.begin(9600);

  tela.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  tela.clearDisplay();
  tela.display();

  Serial.println("Impressora de etiquetas pronta.");
  escreverTexto("UEFS");
  Serial.println("Digite uma palavra e aperte Enter:");
}

void loop() {
  if (Serial.available() > 0) {
    String texto = Serial.readStringUntil('\n');
    texto.trim();
    texto.toUpperCase();

    if (texto.length() > 0) {
      tela.clearDisplay();   // "fita nova"
      tela.display();
      escreverTexto(texto);
      Serial.println("Digite outra palavra:");
    }
  }
}

// ================== TEXTO ==================

void escreverTexto(String texto) {
  if (texto.length() > MAX_LETRAS) {
    Serial.println("Texto grande demais, vou escrever so as 7 primeiras letras.");
  }

  Serial.print("Escrevendo: ");
  Serial.println(texto);

  int quantidade = texto.length();
  for (int i = 0; i < quantidade && i < MAX_LETRAS; i++) {
    inicioLetra = i * ESPACO_LETRA * ESCALA;
    desenharLetra(texto.charAt(i));
  }

  // terminou: levanta a caneta e volta pro comeco
  levantarCaneta();
  irPara(0, BASE);
  Serial.println("Pronto!");
}

// Cada letra e desenhada numa grade de 4 de largura por 6 de altura.
// (0,0) = canto de baixo a esquerda, (4,6) = canto de cima a direita.
void desenharLetra(char letra) {
  switch (letra) {
    case 'A':
      linha(0, 0, 0, 4); linha(0, 4, 2, 6); linha(2, 6, 4, 4); linha(4, 4, 4, 0);
      linha(0, 3, 4, 3);
      break;
    case 'B':
      linha(0, 0, 0, 6); linha(0, 6, 3, 6); linha(3, 6, 4, 5); linha(4, 5, 4, 4);
      linha(4, 4, 3, 3); linha(3, 3, 0, 3);
      linha(3, 3, 4, 2); linha(4, 2, 4, 1); linha(4, 1, 3, 0); linha(3, 0, 0, 0);
      break;
    case 'C':
      linha(4, 6, 0, 6); linha(0, 6, 0, 0); linha(0, 0, 4, 0);
      break;
    case 'D':
      linha(0, 0, 0, 6); linha(0, 6, 2, 6); linha(2, 6, 4, 4); linha(4, 4, 4, 2);
      linha(4, 2, 2, 0); linha(2, 0, 0, 0);
      break;
    case 'E':
      linha(4, 6, 0, 6); linha(0, 6, 0, 0); linha(0, 0, 4, 0);
      linha(0, 3, 3, 3);
      break;
    case 'F':
      linha(4, 6, 0, 6); linha(0, 6, 0, 0);
      linha(0, 3, 3, 3);
      break;
    case 'G':
      linha(4, 6, 0, 6); linha(0, 6, 0, 0); linha(0, 0, 4, 0); linha(4, 0, 4, 3);
      linha(4, 3, 2, 3);
      break;
    case 'H':
      linha(0, 0, 0, 6);
      linha(4, 0, 4, 6);
      linha(0, 3, 4, 3);
      break;
    case 'I':
      linha(1, 6, 3, 6);
      linha(2, 6, 2, 0);
      linha(1, 0, 3, 0);
      break;
    case 'L':
      linha(0, 6, 0, 0); linha(0, 0, 4, 0);
      break;
    case 'M':
      linha(0, 0, 0, 6); linha(0, 6, 2, 3); linha(2, 3, 4, 6); linha(4, 6, 4, 0);
      break;
    case 'N':
      linha(0, 0, 0, 6); linha(0, 6, 4, 0); linha(4, 0, 4, 6);
      break;
    case 'O':
      linha(0, 0, 0, 6); linha(0, 6, 4, 6); linha(4, 6, 4, 0); linha(4, 0, 0, 0);
      break;
    case 'P':
      linha(0, 0, 0, 6); linha(0, 6, 4, 6); linha(4, 6, 4, 3); linha(4, 3, 0, 3);
      break;
    case 'R':
      linha(0, 0, 0, 6); linha(0, 6, 4, 6); linha(4, 6, 4, 3); linha(4, 3, 0, 3);
      linha(2, 3, 4, 0);
      break;
    case 'S':
      linha(4, 6, 0, 6); linha(0, 6, 0, 3); linha(0, 3, 4, 3); linha(4, 3, 4, 0);
      linha(4, 0, 0, 0);
      break;
    case 'T':
      linha(0, 6, 4, 6);
      linha(2, 6, 2, 0);
      break;
    case 'U':
      linha(0, 6, 0, 0); linha(0, 0, 4, 0); linha(4, 0, 4, 6);
      break;
    case 'V':
      linha(0, 6, 2, 0); linha(2, 0, 4, 6);
      break;
    case ' ':
      break;   // espaco: nao desenha nada
    default:
      Serial.print("Letra nao suportada: ");
      Serial.println(letra);
      break;
  }
}

// Desenha uma reta entre dois pontos da grade da letra
void linha(int x1, int y1, int x2, int y2) {
  // converte da grade da letra para passos dos motores
  int inicioX = inicioLetra + x1 * ESCALA;
  int inicioY = BASE - y1 * ESCALA;
  int fimX = inicioLetra + x2 * ESCALA;
  int fimY = BASE - y2 * ESCALA;

  // se a caneta nao esta no comeco da linha, levanta e vai ate la
  if (posTambor != inicioX || posCaneta != inicioY || !canetaAbaixada) {
    levantarCaneta();
    irPara(inicioX, inicioY);
    abaixarCaneta();
  }

  irPara(fimX, fimY);
}

// ================== MOVIMENTO ==================

// Move os dois motores juntos ate o ponto (xAlvo, yAlvo) em linha reta
void irPara(int xAlvo, int yAlvo) {
  int xInicio = posTambor;
  int yInicio = posCaneta;
  int dx = xAlvo - xInicio;
  int dy = yAlvo - yInicio;

  // quantos pontinhos tem no caminho (o maior dos dois deslocamentos)
  int pontos = max(abs(dx), abs(dy));

  for (int i = 1; i <= pontos; i++) {
    int x = xInicio + round((float)dx * i / pontos);
    int y = yInicio + round((float)dy * i / pontos);

    while (posTambor < x) passoTambor(1);
    while (posTambor > x) passoTambor(-1);
    while (posCaneta < y) passoCaneta(1);
    while (posCaneta > y) passoCaneta(-1);

    if (canetaAbaixada) {
      marcarFita();
    }
  }
}

void passoTambor(int direcao) {
  if (direcao > 0) {
    digitalWrite(TAMBOR_DIR, HIGH);
  } else {
    digitalWrite(TAMBOR_DIR, LOW);
  }
  pulso(TAMBOR_STEP);
  posTambor = posTambor + direcao;
}

void passoCaneta(int direcao) {
  if (direcao > 0) {
    digitalWrite(CANETA_DIR, HIGH);
  } else {
    digitalWrite(CANETA_DIR, LOW);
  }
  pulso(CANETA_STEP);
  posCaneta = posCaneta + direcao;
}

// Um pulso no STEP = o motor anda 1 passo
void pulso(int pinoStep) {
  digitalWrite(pinoStep, HIGH);
  delayMicroseconds(TEMPO_PASSO_US);
  digitalWrite(pinoStep, LOW);
  delayMicroseconds(TEMPO_PASSO_US);
}

// ================== CANETA ==================

void abaixarCaneta() {
  if (!canetaAbaixada) {
    servo.write(CANETA_BAIXO);
    delay(200);
    canetaAbaixada = true;
    marcarFita();
  }
}

void levantarCaneta() {
  if (canetaAbaixada) {
    servo.write(CANETA_CIMA);
    delay(200);
    canetaAbaixada = false;
  }
}

// So existe na simulacao: acende o ponto onde a caneta esta
void marcarFita() {
  tela.drawPixel(posTambor, posCaneta, SSD1306_WHITE);
  tela.display();
}
