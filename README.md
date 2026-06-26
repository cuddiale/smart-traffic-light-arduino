Questo progetto implementa la simulazione di un **sistema semaforico stradale intelligente** di livello medio. Oltre alla classica sequenza temporizzata per i veicoli, il sistema integra un semaforo pedonale indipendente attivabile tramite pulsante (prenotazione) e un segnalatore acustico/visivo per i pedoni.

L'obiettivo principale è dimostrare una gestione efficiente delle temporizzazioni in Arduino senza l'utilizzo di codice bloccante (`delay()`), permettendo al sistema di rimanere costantemente in ascolto dell'input del pedone.

---

## 🚀 Simulazione Interattiva su Tinkercad

Il circuito è stato interamente prototipato e testato online. Puoi interagire direttamente con il simulatore software, premere il pulsante di prenotazione pedonale e vedere il comportamento del sistema in tempo reale senza bisogno di hardware fisico.

▶️ [**Avvia la simulazione interattiva su Tinkercad**]

(https://www.tinkercad.com/things/80Jv7yN3XH6-smart-traffic-light-arduino/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=JEMimNzoJyHrKP6r-3n_ySXuy4qbfUugjN1uczeIJZAE)

---

## 🛠️ Componenti Richiesti

| Componente                 | Quantità | Descrizione / Ruolo nel circuito                 |
| :------------------------- | :------: | :----------------------------------------------- |
| **Arduino Uno R3**         |    1     | Microcontrollore principale                      |
| **Breadboard**             |    1     | Basetta sperimentale per i collegamenti          |
| **LED Rosso**              |    2     | 1x Auto, 1x Pedoni                               |
| **LED Giallo**             |    1     | 1x Auto                                          |
| **LED Verde**              |    2     | 1x Auto, 1x Pedoni                               |
| **Resistenze da 220 Ω**    |    5     | Protezione corrente per i 5 LED                  |
| **Resistenza da 10 kΩ**    |    1     | Resistenza di pull-down per il pulsante pedonale |
| **Pulsante (Push Button)** |    1     | Richiesta di attraversamento pedonale            |
| **Buzzer Piezoelettrico**  |    1     | Segnalazione acustica per pedoni non vedenti     |
| **Fili di collegamento**   |   q.b.   | Jumper per i collegamenti elettrici              |

---

## 🚦 Logica di Funzionamento

Il sistema opera come una **Macchina a Stati Finiti**:

1. **Stato Normale:** Il semaforo stradale è VERDE e quello pedonale è ROSSO.
2. **Prenotazione Pedonale:** Quando un pedone preme il pulsante, l'interruzione non è immediata per non bloccare il traffico improvvisamente. Il sistema registra la richiesta.
3. **Sequenza di Scambio:** Dopo un tempo minimo di sicurezza, il semaforo stradale passa da VERDE a GIALLO, e infine a ROSSO.
4. **Attraversamento Pedone:** Il semaforo pedonale diventa VERDE. Il buzzer emette un suono intermittente lento.
5. **Pre-avviso di Chiusura:** Il semaforo pedonale lampeggia e il buzzer accelera il ritmo del suono.
6. **Ripristino:** Il semaforo pedonale torna ROSSO e la strada riceve nuovamente il VERDE.

---

📂 [Scarica la Presentazione Tecnica del Progetto (PDF)](./docs/Guida_Progetto_Semaforo.pdf)
