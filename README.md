# 🤖 Diário de Desenvolvimento

**Equipe: IEEE RAS - UEFS**

## 📝 Visão Geral

Este repositório serve como o diário de desenvolvimento oficial do projeto **Impressora de Etiquetas CNC**. Nosso objetivo é construir um robô que imprima etiquetas na fita crepe. Aqui, documentamos nosso progresso, desafios e aprendizados em todas as frentes: software, hardware, eletrônica e design mecânico.

## 👥 Equipe

| Nome | Cargo no projeto | GitHub |
|---|---|---|
| Julian | Líder de Projeto | [@jubrand701-ops](https://github.com/jubrand701-ops) |
| Bruno | Secretário de Mesa | [@brunoqwe54-bit](https://github.com/brunoqwe54-bit) |

## 🎯 Metas do Projeto

### Metas atuais

- **Hardware:** Definição dos componentes e do planejamento da montagem física da impressora ainda pendentes.
- **Software:** Utilizar o Wokwi para simular o circuito da impressora e compreender o funcionamento dos componentes.
- **Modelagem 3D:** Definição do modelo e desenvolvimento dos desenhos das peças necessárias à montagem ainda pendentes.

### Objetivos Gerais

**Hardware:**
- [ ] Montar e integrar os motores de passo, os drivers A4988 e o Arduino com o Shield CNC.
- [ ] Construir a estrutura mecânica da impressora, incluindo o tambor, o trilho, a correia e o suporte da caneta.

**Software:**
- [ ] Desenvolver o código de controle dos motores e do servo responsável pelo movimento da caneta.
- [ ] Utilizar o GRBL e o G-code para controlar e calibrar os movimentos da impressora.
- [ ] Desenvolver uma forma de transformar textos em comandos para a impressão de etiquetas.
- [ ] Implementar futuramente a comunicação Bluetooth para controlar a impressora pelo celular.

## 🛠️ Tecnologias e Ferramentas

| Categoria | Ferramentas |
|---|---|
| Linguagens | C++ (Arduino) |
| Hardware | Arduino Uno R3, CNC Shield v3, drivers A4988, motores de passo NEMA17 (17HS2408), servo SG90, módulo Bluetooth HC-05 (futuro) |
| Mecânica | Correia GT2, polia, trilho linear, rolo de fita crepe como tambor |
| Simulação | Wokwi |
| Programação | VS Code com PlatformIO |
| Controle (futuro) | GRBL, G-code |
| Modelagem 3D | A definir |

## 📁 Estrutura do Repositório

| Pasta | O que tem |
|---|---|
| `simulacao-wokwi/` | Simulação no Wokwi: `sketch.ino`, `diagram.json` e `libraries.txt`. Uma tela OLED faz o papel da fita. |
| `maquina-real/` | `sketch_real.ino`: a mesma lógica da simulação, para a máquina de verdade (sem tela, medidas em mm). Ainda não testado na máquina. |
| `docs/` | Apostila em PDF sobre o projeto. A Parte 4 (páginas 27 a 43) explica a simulação. |

**Como rodar a simulação:** crie um projeto de Arduino Uno no [Wokwi](https://wokwi.com), cole o conteúdo dos três arquivos de `simulacao-wokwi/` nas abas de mesmo nome e clique em play. Ele escreve "UEFS" e depois aceita uma palavra no Monitor Serial.

## 📓 Diário de Desenvolvimento

Aqui registramos nosso progresso semanalmente. Cada entrada inclui atualizações de hardware e software, os desafios que encontramos e as decisões que tomamos.

### Semana 1: (12/10/2026) - (19/10/2026)

**Foco da Semana:** Simulação do Circuito Geral no Wokwi

**Software:**
- **Arquitetura:** Desenvolvemos a simulação do circuito geral no Wokwi para visualizar o funcionamento dos componentes e ter uma base para o desenvolvimento do projeto.
- **Código:** Iniciamos a estruturação da base do código que será utilizada no controle da impressora.

**Hardware & Modelagem 3D:**
- **Etapa atual:** Estamos trabalhando na representação visual e no planejamento do projeto, deixando a montagem física do hardware para uma etapa posterior.

**Desafios:**
- O Wokwi não disponibiliza todos os componentes necessários para realizar a simulação completa do circuito (não existe o CNC Shield), o que impossibilitou testar integralmente o funcionamento do sistema nessa etapa.

**Decisões:**
- **Utilização do Wokwi:** Escolhemos a plataforma para simular o circuito e compreender melhor o funcionamento do sistema antes de iniciar a montagem física.

## 🚀 Como Contribuir

Somos um projeto de código e hardware abertos! Se você deseja contribuir:

1. Faça um Fork deste repositório.
2. Crie uma Branch para sua feature (`git checkout -b feature/SuaFeature`).
3. Faça o Commit de suas mudanças (`git commit -m 'Adiciona SuaFeature'`).
4. Faça o Push para a Branch (`git push origin feature/SuaFeature`).
5. Abra um Pull Request.

Para problemas ou sugestões, por favor, abra uma Issue.

## 📄 Licença

Este projeto está licenciado sob a Licença GNU GPL v2.0. Veja o arquivo `LICENSE` para mais detalhes.
