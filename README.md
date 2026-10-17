# Impressora de Etiquetas CNC

Plotter de tambor que escreve texto em uma fita adesiva. O rolo de fita é o tambor e gira com um motor de passo. A caneta anda num trilho linear com outro motor de passo, e um servo levanta e abaixa a caneta.

Projeto do IEEE RAS, UEFS.

## Como funciona

- **Eixo do tambor (X):** NEMA17 17HS2408 + A4988. Gira a fita (comprimento da etiqueta).
- **Eixo da caneta (Y):** NEMA17 + A4988 + correia GT2 + polia + trilho. Move a caneta (altura da letra).
- **Servo SG90:** sobe e desce a caneta.
- **Controle:** Arduino Uno R3 + CNC Shield v3.

Para escrever uma letra, o código divide cada letra em retas. Para cada reta ele abaixa a caneta, anda os motores o número de passos certo (os dois ao mesmo tempo nas diagonais) e levanta a caneta.

## Pinos (CNC Shield v3)

| Função | STEP | DIR |
|---|---|---|
| Tambor (eixo X) | 2 | 5 |
| Caneta (eixo Y) | 3 | 6 |

- ENABLE dos drivers: pino 8 (LOW = ligado)
- Servo: pino 11

## Pastas

| Pasta | O que tem |
|---|---|
| `simulacao-wokwi/` | Simulação completa no Wokwi: `sketch.ino`, `diagram.json`, `libraries.txt`. Uma tela OLED faz o papel da fita. |
| `maquina-real/` | `sketch_real.ino`: a mesma lógica para a máquina de verdade (sem tela, medidas em mm). |
| `docs/` | Apostila em PDF explicando o projeto, os componentes, a montagem e o código. A Parte 4 (páginas 27 a 43) explica a simulação. |

## Como rodar a simulação

1. Abra [wokwi.com](https://wokwi.com) e crie um projeto novo de Arduino Uno.
2. Cole o conteúdo de `sketch.ino`, `diagram.json` e `libraries.txt` nas abas de mesmo nome.
3. Clique em play. Ele escreve "UEFS" na tela.
4. Digite uma palavra no Monitor Serial e aperte Enter. Letras aceitas: A B C D E F G H I L M N O P R S T U V e espaço.

## Máquina real

O `sketch_real.ino` **ainda não foi testado na máquina**. Antes de usar, confira as constantes marcadas com `[MEDIR]`:

- `PASSOS_MM_CANETA` e `PASSOS_MM_TAMBOR` (dependem da mecânica e do diâmetro do rolo)
- Os ângulos do servo (`CANETA_CIMA` e `CANETA_BAIXO`)
- Os jumpers de micropasso (1/16) no Shield
- O sentido de giro de cada eixo

Comece com a caneta sem tinta.

## Próximos passos

- Montar a mecânica e medir os passos por mm.
- Testar o `sketch_real.ino`.
- Adicionar as letras que faltam (J, K, Q, W, X, Y, Z e números).
- Decidir entre código próprio e GRBL (com um gerador de texto para G-code).
- Entrada de texto por Bluetooth (HC-05).
