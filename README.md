### 📌 **Data Logger Ambiental - Monitoramento de Condições em Espaços Controlados**  

[![Wokwi Simulation](https://img.shields.io/badge/Wokwi-Simulation-blue)](https://wokwi.com/projects/425625739198945281)  
[![YouTube Demo](https://img.shields.io/badge/YouTube-Demonstration-red)](INSERIR_LINK_DO_VIDEO)  

## 📖 **Descrição do Projeto**  
Este projeto consiste em um **data logger ambiental** desenvolvido para monitoramento de **temperatura, umidade e luminosidade** em espaços controlados. Ele utiliza um microcontrolador **ATMEGA 328P**, sensores ambientais e um módulo RTC para registro preciso de dados ao longo do tempo.  

O dispositivo pode ser utilizado em diversas aplicações, como laboratórios, estufas agrícolas, armazenamento de produtos sensíveis e ambientes climatizados.  

---

## 🎯 **Objetivos do Projeto**  
✅ Monitorar e registrar **temperatura, umidade e luminosidade**.  
✅ Armazenar os dados coletados em **EEPROM** para análise posterior.  
✅ Exibir informações em um **LCD I2C** para consulta em tempo real.  
✅ Garantir precisão nos registros através de um **Relógio de Tempo Real (RTC)**.  
✅ Implementar **alertas visuais e sonoros** para indicar condições fora dos padrões estabelecidos.  
✅ Criar duas versões do código: **simulação no Wokwi** e **versão real do hardware**.  

---

## 🔧 **Especificações Técnicas**  

| **Componente**        | **Função** |
|-----------------------|-----------|
| **Microcontrolador**  | ATMEGA 328P (Arduino Uno R3) |
| **Armazenamento**     | Memória EEPROM |
| **RTC**              | Módulo DS3231 |
| **Sensores**         | DHT11 (Temperatura/Umidade) + LDR (Luminosidade) |
| **Display**         | LCD 16x2 - I2C |
| **Controles**       | Botões de navegação e configuração |
| **Indicadores**     | LEDs para status operacional |
| **Alertas Sonoros** | Buzzer para notificações |
| **Alimentação**     | Bateria de 9V |

### 🔥 **Triggers (Gatilhos de Alerta)**  

- **Temperatura:** 15°C < t < 25°C  
- **Luminosidade:** 0% < l < 30%  
- **Umidade:** 30% < u < 50%  

Caso alguma das medições saia desses intervalos, um alerta será acionado por meio de LEDs e do buzzer.  

---

## 📜 **Lista de Materiais**  

- ✅ **1x Arduino Uno R3 (ATMEGA 328P)**  
- ✅ **1x Sensor DHT11**  
- ✅ **1x LDR + Resistor de 10KΩ**  
- ✅ **1x Módulo RTC DS3231**  
- ✅ **1x LCD 16x2 - I2C**  
- ✅ **1x Buzzer**  
- ✅ **1x Bateria de 9V + Suporte**  
- ✅ **Protoboard, jumpers, LEDs e resistores**  

---

## 💾 **Código-Fonte**  
Este projeto possui uma constante chamada IS_REAL_SYSTEM, que pode ser 0, para simulações no Wokwi, e 1 para ser aplicada em ambiente real. Tal constante é usada sempre que há divergências entre tais implementações.

🔗 **Simulação Wokwi:** [Acesse aqui](https://wokwi.com/projects/425956398534649857)  

---

## 🛠 **Como Utilizar o Data Logger**  

1️⃣ **Ligue o dispositivo** utilizando a bateria de 9V ou conexão USB.  
2️⃣ **Os sensores começarão a capturar os dados ambientais** automaticamente.  
3️⃣ O **LCD exibirá as medições em tempo real** (temperatura, umidade e luminosidade).  
4️⃣ Se alguma medição sair da faixa segura, o **buzzer emitirá um som**.  
5️⃣ Os registros são armazenados na **EEPROM**, permitindo consulta posterior.  

---
### 📖 **Manual de Utilização do Data Logger Ambiental**

---

### 1️⃣ **Ligando o Aparelho**

- **Ao ligar o dispositivo**, você verá uma animação inicial com a logo da nossa empresa. Após a animação, o dispositivo entrará automaticamente na tela de **Temperatura**, onde os dados do sensor de temperatura serão exibidos.

---

### 2️⃣ **Navegação entre as Telas**

O dispositivo possui **dois botões principais** para navegação:

- **Botão 1 (Navegação e Configuração)**: Permite acessar o menu de configurações, ou, caso você já esteja em uma tela de configurações, acionar uma opção específica.
- **Botão 2 (Ciclo entre telas)**: Alterna entre as diferentes telas do dispositivo.

As **4 telas** disponíveis no dispositivo são:

1. **Tela de Temperatura**: Exibe os dados de temperatura coletados pelo sensor DHT11.
2. **Tela de Umidade**: Exibe os dados de umidade coletados pelo sensor DHT11.
3. **Tela de Luminosidade**: Exibe os dados de luminosidade coletados pelo sensor LDR.
4. **Tela de Menu**: Tela principal com o ícone de um cacho de uva, onde você pode acessar as configurações do dispositivo.

---

### 3️⃣ **Tela de Configuração**

Existem duas telas que possuem a **possibilidade de configuração**:  
- **Tela de Temperatura**
- **Tela de Menu**

Para acessar as opções de configuração, siga os passos abaixo:

1. **Pressione o Botão de Configuração** quando estiver em uma tela que possua opções de configuração.
2. **A tela de Configuração aparecerá**. As opções configuráveis estarão disponíveis para você.
3. **Para selecionar uma opção**, basta manter a opção desejada por **5 segundos** na tela. Isso fará com que a opção selecionada seja acionada.

---

### 4️⃣ **Exibindo Logs e Lendo o EEPROM**

Se você deseja acessar os **Printar Log** ou **Chamar EEPROM**, siga os passos:

1. **Acesse a tela de Menu** (com o ícone do cacho de uva).
2. **Pressione o Botão de Configuração** para abrir as opções de configuração da tela de Menu.
3. Nas opções de configuração, você encontrará as funções para **Printar Log** ou **Chamar o EEPROM**.
4. **Selecione a opção desejada** e mantenha-o na tela por **5 segundos** para acionar a leitura dos registros.

---

### 5️⃣ **Alertas e Indicações**

O dispositivo utiliza **alertas visuais e sonoros** para notificar quando algum parâmetro ambiental está fora da faixa segura:

- **Temperatura**: Se a temperatura estiver fora do intervalo de 15°C a 25°C.
- **Luminosidade**: Se a luminosidade estiver fora do intervalo de 0% a 30%.
- **Umidade**: Se a umidade estiver fora do intervalo de 30% a 50%.

Quando os parâmetros saírem da faixa estabelecida, o dispositivo emitirá um **alerta sonoro** através do **buzzer**.

---

### 6️⃣ **Resumo das Funções**

| **Função**                        | **Botão de Configuração**                                | **Botão de Navegação**                           |
|-----------------------------------|-----------------------------------------------------------|---------------------------------------------------|
| **Acessar Configuração**          | Pressionar para abrir a tela de configurações.            | -                                                 |
| **Alternar entre Telas**          | -                                                         | Alterna entre as telas de Temperatura, Umidade, Luminosidade e Menu. |
| **Selecionar Opção de Configuração** | Manter a opção por 5 segundos  na tela para confirmar a seleção. | -                                                 |
| **Ler Log Atual**                 | -                                                         | Acesse a tela de Menu e selecione a opção para ler os logs. |
| **Ler EEPROM**                    | -                                                         | Acesse a tela de Menu e selecione a opção para ler os dados armazenados. |

---
## 🎥 **Demonstração em Vídeo**  
📌 https://youtu.be/ZiD1ola8Gg0?feature=shared  


## 🤝 **Créditos e Contribuições**  
Desenvolvido por **[Edgar Camacho](https://github.com/Edgarcsr), [Henrico Birochi](https://github.com/henricobirochi), [Vítor Braghittoni](https://github.com/VBraghittoni), [Nicholas Birochi](https://github.com/nicholasbirochi), [Vinicius Yamaguti](https://github.com/ViniYama)**.  

Se gostou do projeto, ⭐ deixe um star no repositório!  
