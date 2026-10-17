/*
  Impressora de Etiquetas CNC - VERSAO PARA A MAQUINA REAL

  Mesma logica da simulacao do Wokwi, com 4 mudancas:
    1. Sem tela OLED (a fita de verdade e a tela).
    2. Medidas em milimetros: cada eixo tem seu "passos por mm".
    3. O ponto zero e onde voce deixa a caneta ao ligar (sem fim de curso).
    4. Velocidade mais rapida (os motores reais aguentam).

  Eixo X do Shield = motor do TAMBOR (gira a fita)
  Eixo Y do Shield = motor da CANETA (trilho)
  Servo no pino 11 sobe/desce a caneta.

  ANTES DE USAR, confira as 6 constantes marcadas com [MEDIR].
  Nao foi testado na maquina de verdade: comece com a caneta sem tinta
  e a fita parada, so para ver se o movimento esta certo.

  Como usar: abra o Monitor Serial (9600), digite a palavra e Enter.
  Letras aceitas: A B C D E F G H I L M N O P R S T U V e espaco.
*/

#include <Arduino.h>
#include <Servo.h>

// ---------- Pinos (Shield CNC v3) ----------
const int TAMBOR_STEP = 2;   // eixo X do shield
const int TAMBOR_DIR  = 5;
const int CANETA_STEP = 3;   // eixo Y do shield
const int CANETA_DIR  = 6;
const int ENABLE_PIN  = 8;   // LOW = drivers ligados
const int SERVO_PIN   = 11;

// ---------- Ajustes da MAQUINA ----------
const int CANETA_CIMA  = 90;   // [MEDIR] angulo do servo com a caneta levantada
const int CANETA_BAIXO = 30;   // [MEDIR] angulo do servo com a caneta encostando

// Passos por milimetro de cada eixo (depende da mecanica e do micropasso)
const float PASSOS_MM_CANETA = 80.0;   // [MEDIR] correia GT2 + polia 20 dentes, micropasso 1/16
const float PASSOS_MM_TAMBOR = 17.0;   // [MEDIR] 3200 / (3.1416 x diametro do rolo em mm); aqui rolo de 60 mm

// Tamanho da letra: 1 quadradinho da grade = QUADRADO_MM milimetros
const float QUADRADO_MM = 1.5;         // letra fica com 6 mm de largura e 9 mm de altura
const int ESPACO_LETRA = 6;            // cada letra ocupa 6 quadradinhos (4 de letra + 2 de espaco)
const int MAX_LETRAS = 20;             // limite de seguranca

// Tempo de cada meio-pulso. Menor = mais rapido. Se o motor vibrar e nao girar, AUMENTE.
const int TEMPO_PASSO_US = 500;
const int TEMPO_SERVO_MS = 300;        // tempo para o servo chegar na posicao

// ---------- Objetos ----------
Servo servo;

// ---------- Posicao atual da maquina (em passos) ----------
int posTambor = 0;
int posCaneta = 0;
bool canetaAbaixada = false;
int inicioLetra = 0;      // onde comeca a letra que esta sendo escrita (em quadradinhos)

// Declaracoes antecipadas (o PlatformIO exige)
void escreverTexto(String texto);
void desenharLetra(char letra);
void linha(int x1, int y1, int x2, int y2);
void irPara(int xAlvo, int yAlvo);
void passoTambor(int direcao);
void passoCaneta(int direcao);
void pulso(int pinoStep);
void abaixarCaneta();
void levantarCaneta();

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
  Serial.println("Impressora de etiquetas pronta.");
  Serial.println("Deixe a caneta no canto inferior esquerdo da etiqueta.");
  Serial.println("Digite uma palavra e aperte Enter:");
}

void loop() {
  if (Serial.available() > 0) {
    String texto = Serial.readStringUntil('\n');
    texto.trim();
    texto.toUpperCase();

    if (texto.length() > 0) {
      escreverTexto(texto);
      Serial.println("Digite outra palavra:");
    }
  }
}

// ================== TEXTO ==================

void escreverTexto(String texto) {
  if (texto.length() > MAX_LETRAS) {
    Serial.println("Texto grande demais, vou escrever so as primeiras letras.");
  }

  Serial.print("Escrevendo: ");
  Serial.println(texto);

  int quantidade = texto.length();
  for (int i = 0; i < quantidade && i < MAX_LETRAS; i++) {
    inicioLetra = i * ESPACO_LETRA;
    desenharLetra(texto.charAt(i));
  }

  // terminou: levanta a caneta e volta pro comeco
  levantarCaneta();
  irPara(0, 0);
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
  // converte da grade da letra (quadradinhos) para passos dos motores
  int inicioX = round((inicioLetra + x1) * QUADRADO_MM * PASSOS_MM_TAMBOR);
  int inicioY = round(y1 * QUADRADO_MM * PASSOS_MM_CANETA);
  int fimX = round((inicioLetra + x2) * QUADRADO_MM * PASSOS_MM_TAMBOR);
  int fimY = round(y2 * QUADRADO_MM * PASSOS_MM_CANETA);

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
  }
}

// Se um eixo andar para o lado errado, troque HIGH por LOW (e LOW por HIGH) na linha dele
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
    delay(TEMPO_SERVO_MS);
    canetaAbaixada = true;
  }
}

void levantarCaneta() {
  if (canetaAbaixada) {
    servo.write(CANETA_CIMA);
    delay(TEMPO_SERVO_MS);
    canetaAbaixada = false;
  }
}
