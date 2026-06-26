// --- CONFIGURAZIONE PIN ---
const int PIN_AUTO_ROSSO = 12;
const int PIN_AUTO_GIALLO = 11;
const int PIN_AUTO_VERDE = 10;

const int PIN_PEDONI_ROSSO = 9;
const int PIN_PEDONI_VERDE = 8;

const int PIN_PULSANTE = 7;
const int PIN_BUZZER = 6;

// --- STATI DELLA MACCHINA ---
enum StatiSemaforo {
  STATO_NORMALE,          // Auto VERDE, Pedoni ROSSO
  STATO_PREAVVISO_AUTO,   // Auto GIALLO, Pedoni ROSSO
  STATO_ATTRAVERSAMENTO,  // Auto ROSSO, Pedoni VERDE + Buzzer Lento
  STATO_LAMPEGGIO_PEDONI, // Auto ROSSO, Pedoni VERDE Lampeggiante + Buzzer Rapido
  STATO_ROSSO_SGOMBERO    // Auto ROSSO, Pedoni ROSSO (Sicurezza pedoni) <- NUOVO STATO
};

StatiSemaforo statoAttuale = STATO_NORMALE;

// --- VARIABILI DI TEMPORIZZAZIONE (millis) ---
unsigned long timerStato = 0;
unsigned long timerBuzzer = 0;
unsigned long timerLampeggioPedoni = 0;

// Durate degli stati (in millisecondi)
const unsigned long TEMPO_GIALLO_AUTO = 3000;      // 3 secondi
const unsigned long TEMPO_ATTRAVERSAMENTO = 7000;   // 7 secondi
const unsigned long TEMPO_LAMPEGGIO_PEDONI = 4000; // 4 secondi
const unsigned long TEMPO_ROSSO_SGOMBERO = 2000;   // 2 secondi di sicurezza <- NUOVA COSTANTE
const unsigned long TEMPO_MINIMO_VERDE_AUTO = 5000;// Minimo tempo di verde prima di poter prenotare

unsigned long tempoUltimoCambioStato = 0;
bool richiestaPedone = false;
bool statoLedLampeggiante = HIGH;
bool statoBuzzeracustico = false;

void setup() {
  // Configurazione Output
  pinMode(PIN_AUTO_ROSSO, OUTPUT);
  pinMode(PIN_AUTO_GIALLO, OUTPUT);
  pinMode(PIN_AUTO_VERDE, OUTPUT);
  pinMode(PIN_PEDONI_ROSSO, OUTPUT);
  pinMode(PIN_PEDONI_VERDE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  // Configurazione Input
  pinMode(PIN_PULSANTE, INPUT);
  
  Serial.begin(9600);
  inizializzaStatoNormale();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 1. ASCOLTO COSTANTE DEL PULSANTE
  if (digitalRead(PIN_PULSANTE) == HIGH && statoAttuale == STATO_NORMALE) {
    if (currentMillis - tempoUltimoCambioStato > TEMPO_MINIMO_VERDE_AUTO) {
      if (!richiestaPedone) {
        richiestaPedone = true;
        Serial.println("-> Richiesta pedone registrata!");
      }
    }
  }

  // 2. GESTIONE DELLA MACCHINA A STATI
  switch (statoAttuale) {
    
    case STATO_NORMALE:
      if (richiestaPedone) {
        statoAttuale = STATO_PREAVVISO_AUTO;
        tempoUltimoCambioStato = currentMillis;
        Serial.println("Stato: Preavviso Auto (Giallo)");
        
        digitalWrite(PIN_AUTO_VERDE, LOW);
        digitalWrite(PIN_AUTO_GIALLO, HIGH);
      }
      break;
      
    case STATO_PREAVVISO_AUTO:
      if (currentMillis - tempoUltimoCambioStato >= TEMPO_GIALLO_AUTO) {
        statoAttuale = STATO_ATTRAVERSAMENTO;
        tempoUltimoCambioStato = currentMillis;
        timerBuzzer = currentMillis;
        Serial.println("Stato: Attraversamento Pedoni (Verde)");
        
        digitalWrite(PIN_AUTO_GIALLO, LOW);
        digitalWrite(PIN_AUTO_ROSSO, HIGH);
        digitalWrite(PIN_PEDONI_ROSSO, LOW);
        digitalWrite(PIN_PEDONI_VERDE, HIGH);
      }
      break;
      
    case STATO_ATTRAVERSAMENTO:
      if (currentMillis - timerBuzzer >= 500) {
        timerBuzzer = currentMillis;
        statoBuzzeracustico = !statoBuzzeracustico;
        if(statoBuzzeracustico) tone(PIN_BUZZER, 440, 100);
      }
      
      if (currentMillis - tempoUltimoCambioStato >= TEMPO_ATTRAVERSAMENTO) {
        statoAttuale = STATO_LAMPEGGIO_PEDONI;
        tempoUltimoCambioStato = currentMillis;
        timerLampeggioPedoni = currentMillis;
        timerBuzzer = currentMillis;
        Serial.println("Stato: Preavviso Chiusura Pedoni (Lampeggio)");
      }
      break;
      
    case STATO_LAMPEGGIO_PEDONI:
      if (currentMillis - timerLampeggioPedoni >= 250) {
        timerLampeggioPedoni = currentMillis;
        statoLedLampeggiante = !statoLedLampeggiante;
        digitalWrite(PIN_PEDONI_VERDE, statoLedLampeggiante);
        tone(PIN_BUZZER, 550, 80); 
      }
      
      if (currentMillis - tempoUltimoCambioStato >= TEMPO_LAMPEGGIO_PEDONI) {
        // MODIFICA: Invece di tornare subito allo stato normale, andiamo in sgombero
        statoAttuale = STATO_ROSSO_SGOMBERO;
        tempoUltimoCambioStato = currentMillis;
        Serial.println("Stato: Rosso Sgombero (Tutto Rosso per 2s)");
        
        digitalWrite(PIN_PEDONI_VERDE, LOW);
        digitalWrite(PIN_PEDONI_ROSSO, HIGH); // Pedoni tornano ROSSO
        noTone(PIN_BUZZER);                  // Spegne definitivamente il buzzer
      }
      break;

    case STATO_ROSSO_SGOMBERO:
      // Attendi 2 secondi mantenendo Auto ROSSO e Pedoni ROSSO
      if (currentMillis - tempoUltimoCambioStato >= TEMPO_ROSSO_SGOMBERO) {
        richiestaPedone = false;
        inizializzaStatoNormale(); // Riapre il traffico auto portandolo a VERDE
      }
      break;
  }
}

void inizializzaStatoNormale() {
  statoAttuale = STATO_NORMALE;
  tempoUltimoCambioStato = millis();
  Serial.println("Stato: Traffico Regolare (Verde Auto)");
  
  digitalWrite(PIN_AUTO_ROSSO, LOW);
  digitalWrite(PIN_AUTO_GIALLO, LOW);
  digitalWrite(PIN_AUTO_VERDE, HIGH);
  
  digitalWrite(PIN_PEDONI_ROSSO, HIGH);
  digitalWrite(PIN_PEDONI_VERDE, LOW);
  noTone(PIN_BUZZER);
}