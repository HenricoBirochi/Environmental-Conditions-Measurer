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

---

## 📏 **Unidades de Medida e Precisão dos Sensores**  

Este projeto utiliza três tipos principais de sensores para monitoramento ambiental: **temperatura**, **umidade** e **luminosidade**. Abaixo estão as unidades de medida utilizadas e a precisão dos sensores em questão:

### 1️⃣ **Sensor de Temperatura (DHT11)**  
- **Unidade de Medida**: **Celsius (°C)**  
- **Faixa de Medição**: 0°C a 50°C  
- **Precisão**:  
  - **±2°C** na faixa de 0°C a 50°C  
  - A precisão do DHT11 pode ser influenciada por variações de umidade e pela temperatura ambiente em que ele está inserido.
  - No programa, a medição de temperatura pode ser convertida para Fahrenheit (°F) e para Kelvin (K), de acordo com a configuração.

### 2️⃣ **Sensor de Umidade (DHT11)**  
- **Unidade de Medida**: **Percentual de Umidade (%)**  
- **Faixa de Medição**: 20% a 90% de umidade relativa  
- **Precisão**:  
  - **±5%** de umidade relativa (RH)  
  - O DHT11 possui uma precisão moderada para aplicações gerais, mas pode ter variações dependendo das condições ambientais.

### 3️⃣ **Sensor de Luminosidade (LDR)**  
- **Unidade de Medida**: **Lux (lx)**  
- **Faixa de Medição**: Depende da intensidade de luz disponível no ambiente.  
- **Precisão**:  
  - A precisão do LDR depende diretamente da resistência do sensor e da quantidade de luz incidente sobre ele. Como a medição é feita de forma indireta, a precisão pode variar em ambientes com diferentes tipos de iluminação.  
  - **Variante de precisão**: A medição não é linear e pode ser influenciada por fatores como cor da superfície, distância da fonte de luz e intensidade da luz.

### **Considerações sobre Precisão**  
Embora os sensores utilizados no projeto tenham boa precisão dentro de suas faixas de medição, é importante lembrar que a precisão de cada sensor pode variar dependendo do ambiente em que ele está sendo utilizado. Para um controle mais rigoroso, é possível calibrar os sensores ou utilizar versões mais precisas, como o **DHT22** para temperatura e umidade, ou sensores LDR de maior qualidade.

---

## 🔧 **Especificações Técnicas e Lista de Materiais**  

O Data Logger Ambiental utiliza os seguintes componentes para monitoramento e controle das condições ambientais:

| **Componente**              | **Função/Descrição**                                                                                           |
|-----------------------------|-----------------------------------------------------------------------------------------------------------------|
| **Microcontrolador**         | **ATMEGA 328P (Arduino Uno R3)** - Processa os dados coletados pelos sensores.                                  |
| **Armazenamento**            | **Memória EEPROM** - Armazena os dados coletados para análise posterior.                                        |
| **RTC (Relógio de Tempo Real)** | **Módulo DS3231** - Garantia de registros precisos de tempo para os dados coletados.                            |
| **Sensor de Temperatura/Umidade** | **DHT11** - Sensores para medição de temperatura e umidade. Precisão moderada, com faixa de 0°C a 50°C para temperatura e 20% a 90% para umidade. |
| **Sensor de Luminosidade**  | **LDR (Resistor Dependente de Luz)** - Sensores para medir a intensidade de luz no ambiente.                    |
| **Display LCD**             | **LCD 16x2 - I2C** - Exibe os dados em tempo real, como temperatura, umidade e luminosidade.                   |
| **Botões de Controle**      | **Botões de navegação e configuração** - Permitem interação com o dispositivo e configuração de parâmetros.     |
| **Indicadores LED**         | **LEDs** - Indicadores visuais de status operacional.                                                           |
| **Alertas Sonoros**         | **Buzzer** - Emitirá alertas sonoros quando algum parâmetro estiver fora da faixa segura.                       |
| **Alimentação**             | **Bateria de 9V** - Fornece energia para o sistema.                                                            |

Esses componentes juntos permitem o monitoramento eficiente das condições ambientais, com registro de dados, exibição em tempo real e alertas visuais e sonoros quando os parâmetros se afastam das faixas seguras.

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

## 🤝 **Créditos e Contribuições**  
Desenvolvido por **[Edgar Camacho](https://github.com/Edgarcsr), [Henrico Birochi](https://github.com/henricobirochi), [Vítor Braghittoni](https://github.com/VBraghittoni), [Nicholas Birochi](https://github.com/nicholasbirochi), [Vinicius Yamaguti](https://github.com/ViniYama)**.  

Se gostou do projeto, ⭐ deixe um star no repositório!  
