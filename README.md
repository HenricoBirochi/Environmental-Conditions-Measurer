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
4️⃣ Se alguma medição sair da faixa segura, um **LED de alerta piscará** e o **buzzer emitirá um som**.  
5️⃣ Os registros são armazenados na **EEPROM**, permitindo consulta posterior.  
6️⃣ Utilize os **botões de navegação** para visualizar registros salvos e configurar parâmetros.  

---

## 🎥 **Demonstração em Vídeo**  
📌 *(link do vídeo a ser adicionado no YouTube.)*  


## 🤝 **Créditos e Contribuições**  
Desenvolvido por **[Edgar Camacho](https://github.com/Edgarcsr), [Henrico Birochi](https://github.com/henricobirochi), [Vítor Braghittoni](https://github.com/VBraghittoni), [Nicholas Birochi](https://github.com/nicholasbirochi), [Vinicius Yamaguti](https://github.com/ViniYama)**.  

Se gostou do projeto, ⭐ deixe um star no repositório!  
