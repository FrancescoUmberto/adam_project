# Relazione Thrust Stand

Il progetto "Distributed Measurement / Data Acquisition" mira a realizzare un *thrust stand* specifico per motori di droni, strumento indispensabile per una completa caratterizzazione delle loro prestazioni. L’obiettivo è raccogliere dati fondamentali per supportare diverse analisi progettuali, quali il monitoraggio del rumore, l’analisi dei consumi, la valutazione della spinta verticale e il controllo della temperatura.

A tal fine, il sistema acquisirà i seguenti parametri:

- **Voltaggio:** rilevato tramite power module
- **Amperaggio:** misurato con un sensore Hall lineare
- **Temperatura:** monitorata mediante un sensore a infrarossi
- **Rumore:** registrato tramite microfono
- **Spinta verticale:** valutata grazie a una cella di carico
- **RPM**: ottenuto trmite sensore di rilevamento di linea

Per garantire il successo del prototipo, è stato essenziale definire una roadmap progettuale articolata in diverse categorie chiave:

- **Scelte strutturali:** Decisioni mirate a garantire un design sicuro e affidabile durante l’uso.
- **Scelte hardware:** Analisi approfondite per selezionare i sensori più adatti, considerando fattori quali risoluzione e velocità di acquisizione, unitamente alla valutazione della scheda Arduino più idonea alle esigenze del progetto.
- **Scelte software:** Scelte orientate a sviluppare un codice leggibile, modulare e facilmente modificabile, ottimizzato per prestazioni elevate e per assicurare un controllo sicuro del motore.

Gli aspetti cruciali che hanno guidato le scelte progettuali sono:

- La velocità del motore e la sua potenza, capace di raggiungere oltre i 16 mila giri e un assorbimento superiore ai 40W.
- La disponibilità ridotta di timer a 16bit sulla scheda Arduino

Il banco di prova supporta due modalità per analisi differenziate:

1. **Modalità a Regime** 
    - **Impostazione** : Duty cycle fisso per mantenere il motore a velocità costante.
    - **Applicazioni** : Monitoraggio di parametri stabili (temperatura, rumore) e rilevazione di anomalie a lungo termine.
    - **Parametri configurabili** : Duty cycle target, durata del test, frequenza di invio.
2. **Modalità a Rampa** 
    - **Impostazione** : Aumento graduale della velocità da un duty cycle iniziale a uno finale, entro un intervallo di tempo definito.
    - **Applicazioni** : Studio di consumi, RPM e spinta durante transitori dinamici (es. accelerazioni tipiche dei droni).
    - **Parametri configurabili** : Duty cycle iniziale/finali, durata della rampa, frequenza di invio.

Tali aspetti saranno sviluppati nei prossimi capitoli nel dettaglio.

## Architettura software

Il sistema di acquisizione sviluppato è suddiviso in due sotto-progetti:

- **Progetto di sviluppo in ambiente Arduino** che comprende l’implementazione del software necessario per gestire la ricezione dei comandi da parte di LabVIEW, l’acquisizione dei dati dai sensori e l’invio dei dati al pannello di visualizzazione sviluppato su LabVIEW.
- **Progetto di sviluppo in ambiente LabVIEW** che gestisce tutta la logica di visualizzazione dei dati e di interfaccia con l’Arduino.

## Protocollo di comunicazione testuale

Prima di parlare dell’architettura del progetto Arduino, è necessario soffermarsi su un aspetto comune ai due progetti che è rappresentato dalla comunicazione tra i sistemi.

Dunque, la fase iniziale è stata quella di definire la sintassi di un *protocollo testuale,* necessario per inviare comandi al controllore Arduino e dati all’interfaccia LabVIEW, e basato su comunicazione seriale.

Il requisito più importante è quello relativo la semplicità dei messaggi (o comandi) testuali inviati e ricevuti. Questo permette di effettuare il parsing dei comandi in maniera rapida, nonché una composizione standard dei messaggi da inviare all’interfaccia LabVIEW.
I vari parametri contenuti nel messaggio sono separati da una `$,$`, in modo da poter operare il suddetto parsing.

Di seguito viene indicata la struttura base dei comandi da inviare all’Arduino.

| Field | **Type** | **Unit** | **Value** | **Description** |
| --- | --- | --- | --- | --- |
| CODE | Enum | - | START | Inizio acquisizione dati  |
|  |  |  | STOP | Fine acquisizione dati |
| MODE | Enum | - | SINGLE | Modalità con velocità costante |
|  |  |  | SWEEP | Modalità di velocità curvilinea |
| DURATION | Float | $\text{secondi [s]}$ | $> 0$ | Durata dell’acquisizione |
| SAMPLING_PERIOD | Float | $\text{Hertz [Hz]}$ | $> 0$ | Frequenza di campionamento |

La struttura del messaggio indicata nella tabella precedente, deve essere estesa a seconda della modalità di acquisizione dei dati scelta.

### **SINGLE MODE**

| Field | **Type** | **Unit** | **Values** | **Description** |
| --- | --- | --- | --- | --- |
| DUTY_CYCLE | Integer | % | 0-100 | Duty Cycle |

### **SWEEP MODE**

| Field | Type | Unit | Values | Description |
| --- | --- | --- | --- | --- |
| INITIAL_DC | Integer | % | 0-100 | Duty Cycle iniziale |
| FINAL_DC | Integer | % | 0-100 | Duty Cycle finale |
| CURVE_TYPE | Enum | - | RAMP | STEPS | Ramp | Steps |
| N_STEPS | Integer | - | > 0 | Numero di steps (solo se CURVE=STEPS) |

I dati inviati dall’Arduino all’interfaccia esterna, in questo caso LabVIEW, sono strutturati nel seguente ordine:

| Field | Type | Unit | Values | Description |
| --- | --- | --- | --- | --- |
| CURRENT | Float | $\text{mA}$ | $≥ 0.0$ | Corrente fornita all’ESC |
| VOLTAGE | Float | $\text{V}$ | $≥ 0.0$ | Voltaggio fornito all’ESC |
| TEMPERATURE | Float | $\text{°C}$ | - | Temperatura misurata con MLX90614 |
| RPM | Float | $\text{RPM}$ | $≥ 0.0$ | Misura degli RMPs misurati con IR LED PHOTORESISTOR COUPLE |
| THRUST | Float | $\text{g}$ | - | Misura della spinta con STRAIN GAUGE |
| NOISE | Float | $\text{dB}$ | $≥ 0.0$ | Misura del rumore  con un MICROFONO |

### Esempi di applicazioni

```powershell
# SINGLE MODE
	# Duration = 10s
	# Sampling Period = 5 Hz
	# Duty Cycle = 80%
START,SINGLE,10,5,80 

# SWEEP MODE
	# Duration = 100s
	# Sampling Period = 10 Hz
	# INITIAL_DC = 20
	# FINAL_DC = 80
	# CURVE_TYPE = RAMP
START,SWEEP,100,10,20,80,RAMP # sweep function

# STOP COMMAND
STOP

# RECEIVED DATA SAMPLE
	# Current = 1A
	# Voltage = 20V
	# Temperature = 25 °C
	# RPM = 12130 rpm
	# Thrust = 200g
	# Noise = 90dB
DATA,1,20,25,12130,200,90
```

## Progetto Arduino

Il software per **Arduino** è stato sviluppato utilizzando **PlatformIO** e organizzato secondo la seguente struttura:

---

```
├── src
│   ├── header
│   │   ├── code_parser.h
│   │   ├── data.h
│   │   ├── engine.h
│   │   ├── microphone.h
│   │   ├── mode.h
│   │   ├── parser.h
│   │   ├── pin.h
│   │   ├── rpm_sensor.h
│   │   ├── strain_gauge.h
│   │   ├── temperature.h
│   │   └── utils.h
│   ├── mega
│   │   ├── adam_project.cpp
│   │   ├── code_parser.cpp
│   │   ├── data.cpp
│   │   ├── engine.cpp
│   │   ├── microphone.cpp
│   │   ├── mode.cpp
│   │   ├── parser.cpp
│   │   ├── rpm_sensor.cpp
│   │   ├── strain_gauge.cpp
│   │   ├── temperature.cpp
│   │   └── utils.cpp
│   └── uno
│       └── strain_gauge.cpp
```

---

Dal file system emerge l'impiego di **due schede Arduino**:

- **Arduino Mega 2560**
    - È la scheda principale del progetto.
    - Gestisce il controllo del motore, l'acquisizione dei dati e la trasmissione delle informazioni sulla porta seriale.
- **Arduino Uno**
    - Agisce come modulo di acquisizione per il sensore *strain gauge*.
    - Comunica con Arduino Mega attraverso il collegamento seriale (RX/TX).

Per la gestione del **sensore strain gauge**, è stata adottata la libreria ufficiale di **HX711**, che semplifica la calibrazione e fornisce letture con **attenuazione del rumore**. Tuttavia, la libreria introduce un **ritardo intrinseco** che ha generato i seguenti problemi nel controllo del motore:

- Accelerazioni troppo brusche;
- Passaggi di velocità incostanti;
- Rischio di rottura dell’alimentatore da banco per i ritorni di corrente a causa dello stop improvviso del motore senza una decellerazione attenuata.

Per risolvere questa criticità, il codice di acquisizione del sensore è stato **spostato su Arduino Uno**, che trasmette i valori aggiornati ad Arduino Mega **non appena disponibili**.

La scelta di **Arduino Mega** è stata motivata da:

- **Ampia disponibilità di porte I/O**, riducendo la necessità di cablaggi aggiuntivi che potrebbero degradare la qualità del segnale, soprattutto con sensori sensibili, introducendo rumore.
- **Presenza di timer a 16 bit**, fondamentali per il controllo del motore e la gestione del sistema di acquisizione.

In particolare, la libreria **Servo** di Arduino utilizza tutti i timer disponibili. Dopo diversi test, si è deciso di **escludere il Timer 5 dalla libreria**, in modo da destinarlo esclusivamente alla gestione degli interrupt per la trasmissione dati sulla seriale.

Di seguito è riportato il **diagramma di flusso** che illustra il funzionamento del sistema:

---

![Adam.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Adam.png)

---

### Mode

Il progetto prevede diverse **modalità di funzionamento**, ognuna implementata tramite una specifica classe con relativi attributi e metodi.

---

![Mode.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/544449f5-f0a4-4db6-9cf1-151969881695.png)

---

Per evitare problemi di inconsistenza dei dati all’interno del progetto, l’header associato al modulo Mode esporta le seguenti variabili globali:

- CurrentMode: enumerativo che stabilisce la modalità con cui si sta attualmente lavorando
- globalSweepMode: istanza globale di SweepMode
- globalSingleSpeedMode: istanza globale di SingleSpeedMode

### PIN

In questo header vengono definite tutte le macro-costanti del progetto, come riportato di seguito

---

```cpp
// pin.h
#define ESC_PIN 11 
#define RMP_PIN 8
#define MICROPHONE_PIN A0
#define MIN_SPEED 1000
#define MAX_SPEED 2000
#define DATA_PIN 3
#define CLOCK_PIN 4
#define AMPERE_PIN A10
#define VOLTAGE_PIN A7
```

---

Il motivo per cui è stato dichiarato questo file di header è quello di utilizzarlo per impostare la configurazione hardware dell’ambiente di lavoro, tramite dichiarazione dei pin che verranno utilizzati da ciascuna componente hardware. 

### DATA

Per prevenire problemi di inconsistenza, è stata definita e istanziata un'unica classe globale dedicata alla gestione delle misurazioni acquisite dai vari sensori. Ogni modulo specifico dei sensori può accedere a questa classe e aggiornare i valori corrispondenti attraverso apposite funzioni setter.

Durante la gestione degli interrupt, la classe si occupa della generazione della stringa da inviare sulla porta seriale, minimizzando il tempo di esecuzione.

### Parsing

Il modulo *Parser* ha il compito di elaborare i dati ricevuti dalla seriale, estraendo i parametri secondo il formato definito nella sezione [Protocollo di comunicazione testuale](https://www.notion.so/Relazione-Thrust-Stand-1acec7177fc980e29f7ec16b75a74b64?pvs=21). Per garantire la consistenza delle informazioni tra i vari moduli, le seguenti variabili globali vengono rese accessibili ai codici che importano il namespace `parser`:

- `sampling_period`
- `duration`

La funzione `parse_command(String &optional)` analizza il comando ricevuto, identificando la modalità operativa (`mode`), la durata dell'esecuzione (`duration`) e il periodo di campionamento (`sampling_period`). A seconda della modalità selezionata (`SINGLE`, `SWEEP`), vengono estratti e impostati i relativi parametri, garantendo il corretto funzionamento del sistema.

Se il parsing non va a buon fine, viene segnalato un errore tramite la seriale.

### Sensori

Per garantire un'architettura modulare e uniforme, ogni sensore è implementato all'interno di un proprio *namespace*, migliorando la leggibilità e la manutenibilità del codice.

Tutti i sensori seguono una struttura simile: se richiedono una configurazione iniziale, includono la funzione `begin()`, mentre l'acquisizione del dato campionato avviene tramite una funzione denominata `process<Sensor>Sample()`, dove `<Sensor>` rappresenta il tipo di sensore specifico.

Ad esempio:

- Il sensore di temperatura (`temperature`) definisce le funzioni `begin()` e `processTemperatureSample()`.
- Il sensore *strain gauge* (`strain_gauge`) segue lo stesso schema, con `begin()` e `processStrainSample()`.
- Il sensore di tensione (`voltage`) non richiede configurazione iniziale e fornisce direttamente la funzione `processVoltageSample()`.

---

![ADAMS_bb.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/ADAMS_bb.png)

## Progetto LabVIEW

Il progetto LabVIEW rappresenta l'interfaccia utente del sistema di acquisizione dati, sviluppata per offrire un controllo intuitivo e una visualizzazione efficace delle misurazioni. Questa componente software è stata progettata per gestire la comunicazione seriale con Arduino, elaborare i dati in tempo reale e presentarli attraverso una interfaccia grafica user-friendly.

L'applicazione è strutturata in diversi moduli funzionali che si occupano della configurazione della comunicazione seriale, dell'invio dei comandi di controllo, dell'acquisizione e visualizzazione dei dati.

### Interfaccia grafica

L’interfaccia grafica è divisa in quattro zone, ognuna delle quali svolge un ruolo specifico nel processo di acquisizione. Sulla sinistra sono presenti i pannelli di **connessione** e di **setup del test**, in cui è possibile impostare i parametri di esecuzione. Sulla destra, in alto si possono osservare gli **indicatori** che riportano il **valore istantaneo** delle misurazioni in corso, in basso si sono presenti i grafici relativi ad alcune **curve di caratterizzazione** utili allo studio delle prestazioni del motore e del propeller.

![Interfaccia grafica per operare e visualizzare i dati acquisiti dal trust stand.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-07_alle_10.48.15.png)

Interfaccia grafica per operare e visualizzare i dati acquisiti dal trust stand.

Per rendere più agevole l’utilizzo dell’interfaccia da parte degli utenti è stato sfruttato l’utilizzo dei *property node,* relativi ad alcuni degli elementi grafici dell’interfaccia. Questa metodologia ha permesso la coesistenza, compatta ed efficace, di modalità multiple, selezionabili attraverso dei menù a tendina, e la disattivazione dinamica di alcuni elementi dell’interfaccia (i.e. bottoni o selettori) non utilizzabili nello stato attuale del sistema. Tutta la logica relativa al controllo dell’aspetto degli elementi grafici è stata raggruppata all’interno di un **loop apposito** e indipendente dal resto della logica applicativa in modo da evitare conflitti nell’esecuzione di t**ask critici** e permettere all’interfaccia di essere sempre reattiva.

![Logica per la gestione della visibilità e della disabilitazione dinamica di alcuni elementi grafici a partire dallo stato del sistema.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.16.35_PM.png)

Logica per la gestione della visibilità e della disabilitazione dinamica di alcuni elementi grafici a partire dallo stato del sistema.

Nelle prossime sezioni verranno approfondite le peculiarità dei singoli elementi dell’interfaccia, con un focus specifico sulle scelte progettuali attuate.

### Connessione seriale

![Pannello per la connessione seriale.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image.png)

Pannello per la connessione seriale.

La prima sezione con cui l’utente è tenuto ad interagire è il **pannello per la gestione della connessione seriale**. Qui è possibile selezionare la risorsa hardware con cui **stabilire la connessione**. In seguito viene riportato lo stato della connessione ed eventuali errori relativi ad essa. In questo modo l’utente viene informato in caso di **malfunzionamenti** e può agire in base all’errore incontrato.

Inoltre l’utente può utilizzare questo pannello per chiudere la connessione in modo esplicito. Di seguito è riportata la logica per la gestione della connessione. Lo schema attende la pressione del pulsante “*Connect*” per aprire la connessione e la tiene aperta finché l’utente non richiede la disconnessione. 

![Logica per la gestione della connessione seriale e per la sua chiusura.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.21.47_PM.png)

Logica per la gestione della connessione seriale e per la sua chiusura.

Com’è possibile notare dalla figura, all’interno di questa sequenza non avviene né l’invio dei comandi né la lettura dei dati, entrambe le operazioni sono gestite da altri due loop indipendenti. Eventuali dati o stati condivisi tra più loop vengono letti e scritti attraverso gli appositi *property node* riferiti al *value* dell’oggetto in questione*.* E’ stato attuato questo metodo di divisione dei loop per permettere l’indipendenza tra le diverse operazioni quali la gestione della connessione e il processo di acquisizione vera e propria dei dati. In questa maniera è possibile effettuare più misurazioni successive senza dover disconnettere e riconnettere il dispositivo.

### Invio dei comandi

![Pannello per il controllo dell’acquisizione.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%201.png)

Pannello per il controllo dell’acquisizione.

Una volta effettuata la connessione l’utente può procedere con l’inserimento dei parametri necessari a definire le modalità relative all’**acquisizione** e al **controllo del motore** durante quest’ultima. 

Nella prima sezione è possibile selezionare la **modalità di controllo del motore**, la **durata complessiva** dell’acquisizione e la **frequenza di campionamento.**

Nella seconda sezione sono presenti i **parametri specifici** relativi alla modalità scelta. La modalità SWEEP presenta un numero maggiore di parametri (come mostrato in figura) e permette la **realizzazione automatica** di diverse **curve di caratterizzazione**.

Proseguendo lungo il pannello è presente il bottone “*Start*”, abilitato unicamente in seguito ad una connessione avvenuta con successo, e due campi di testo a scopo di **debug**, in cui viene mostrato il comando in forma testuale (sulla sinistra) e l’ultimo messaggio ricevuto dalla scheda (sulla destra).

Come anticipato precedentemente, l’invio del comando di START e la lettura dei dati risiedono in un loop indipendente. Anche questo loop è caratterizzato da una sequenza il cui primo frame è dedicato all’attesa della pressione di un tasto, in questo caso il tasto “*Start*”, dopo cui il comando viene avviato ed è possibile avviare la fase di acquisizione vera e propria dei dati.

![Logica dell’invio del comando di START.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%202.png)

Logica dell’invio del comando di START.

Come precedentemente trattato nella sezione [Protocollo di comunicazione testuale](https://www.notion.so/Relazione-Thrust-Stand-1acec7177fc980e29f7ec16b75a74b64?pvs=21), il programma LabView comunica con la scheda di controllo attraverso un protocollo testuale, per questo motivo è stato necessario sviluppare un sotto-programma in grado di trasformare le informazioni contenute all’interno del cluster “*Control*” nella apposita stringa seguendo il protocollo definito. Questo obiettivo è stato raggiunto tramite la concatenazione di stringhe composte attraverso più blocchi “*Build Text*” appositamente configurati e combinate correttamente a seconda delle varie modalità utilizzando la “*Case Structure*”.

![Logica per la composizione della stringa testuale per il comando di START a partire dal cluster relativo al pannello “*Command*”.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.31.06_PM.png)

Logica per la composizione della stringa testuale per il comando di START a partire dal cluster relativo al pannello “*Command*”.

### Gestione dei dati

Il cuore del programma risiede all’interno del seguente loop che contiene la logica per la **gestione di un ciclo di acquisizione**, dall’invio del comando START (già analizzato precedentemente), passando per la l**ettura dei dati**, fino alla **costruzione in tempo reale delle curve** di caratterizzazione e al **post processing** necessario ad una più corretta lettura dei grafici al termine dell’acquisizione.

![Overview del loop contenente la logica per la gestione di un ciclo di acquisizione.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.26.21_PM.png)

Overview del loop contenente la logica per la gestione di un ciclo di acquisizione.

Una volta inviato il comando di START, la sequenza passa il controllo ad un loop di lettura che, oltre che della gestione dei dati, si occupa di intercettare eventuali errori, controllare l’integrità del messaggio, leggere la sequenza di terminazione e gestire il bottone di stop, all’interno del programma. Tutto questo è realizzato in modo da rendere l’esperienza dell’utente sicura ed trasparente. 

Ad ogni ciclo, la seriale viene letta con l’apposito blocco, il messaggio viene decodificato e i dati prendono due diverse strade:

- invio diretto al cluster di indicatori per i valori istantanei
- concatenazione dei dati all’interno di vettori utili alla costruzione dei grafici

Il parsing del messaggio avviene attraverso l’apposito blocco “*Scan From String*” seguendo il formato definito dal protocollo di trasmissione.

![Logica di parsing della stringa di dati ricevuta dalla scheda di acquisizione.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%203.png)

Logica di parsing della stringa di dati ricevuta dalla scheda di acquisizione.

### Costruzione grafici

Come accennato nella sezione precedente, i dati vengono accumulati, ad ogni ciclo di lettura, all’interno di vettori pronti all’utilizzo per la costruzione di grafici e curve di caratterizzazione. L’accumulazione avviene utilizzando un sotto-programma appositamente realizzato per aggiungere elementi tanti vettori contemporaneamente a partire dal cluster di dati letti. Questi vettori vengono poi passati al ciclo successivo attraverso l’utilizzo di *Shift Registers.* Ad ogni ciclo di acquisizione i dati vengono combinati e passati all’apposito grafico per essere mostrati

![Logica di lettura, accumulazione dei dati e costruzione dei grafici.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.28.20_PM.png)

Logica di lettura, accumulazione dei dati e costruzione dei grafici.

![Logica di post processing dei grafici.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.29.05_PM.png)

Logica di post processing dei grafici.

Analizzando i dati delle prime letture, abbiamo riscontrato che la natura delle curve di caratterizzazione del sistema richiede un **riordino dei dati** in base alla variabile scelta come ascissa del grafico, al fine di evitare **errori di rappresentazione**. Inoltre, abbiamo osservato la necessità di applicare un filtro a media mobile per **migliorare la leggibilità** del grafico che correla spinta e rumorosità.

Il riordinamento dei dati è stato implementato attraverso un **sotto-programma** appositamente sviluppato per soddisfare le esigenze specifiche di questa applicazione.

Di seguito è riportato lo schema adottato per il riordinamento, che considera anche i casi in cui più occorrenze di un dato corrispondano alla stessa ascissa. In tali situazioni, viene calcolata la media tra le occorrenze.

![Screenshot 2025-03-04 at 10.33.42 PM.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.33.42_PM.png)

![Logica per il riordinamento di coppie di dati al fine della rappresentazione grafica di una curva.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.33.53_PM.png)

Logica per il riordinamento di coppie di dati al fine della rappresentazione grafica di una curva.

### Arresto dell’app

Infine è stato gestito in modo esplicito anche l’arresto dell’applicazione. Anche in questo caso si attende la pressione di apposito bottone. Una volta premuto, la sequenza si occupa della chiusura della connessione seriale, reimposta tutti i valori di input e indicatori al loro valore di default e ferma il programma.

![Logica di arresto dell’applicazione LabView.](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/Screenshot_2025-03-04_at_10.17.11_PM.png)

Logica di arresto dell’applicazione LabView.

# Componentistica

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/465fe573-83b0-462a-b734-7024f24567ea.png)

Il progetto si compone di un motore brushless (”Avenger 2812 V3 900kv”) corrente massima assorbibile $33.8A$ e tensione massima applicabile di $23.8V$ (potenza massima in input di $804.44W$). Il motore è in grado di raggiungere una temperatura di $97.8°C$ dopo 2 min di funzionamento al 100% del throttle (a temperatura ambiente di $29°C$).

Il motore è in grado di raggiungere una temperatura di $97.8°C$ dopo 2 min di funzionamento al 100% del throttle (a temperatura ambiente di $29°C$).

Per la rilevazione delle misure necessarie al monitoraggio e alla caratterizzazione del motore, durante il suo funzionamento, sono stati presi in considerazione diversi sensori:

- **Corrente**:  PowerModule GM V 1.0, ACS712
- **Tensione**: Power Module GM V1.0
- **Temperatura**: KY-028, MLX90614
- **RPM**:  KY-032, KY-033
- **Spinta**: Strain Gauge con HX711
- **Rumore**: KY-037

## Sensore di corrente

Il sensore di corrente è necessario per rendersi conto del quantitativo di corrente erogato dall’alimentatore e fornito all’intero sistema.

L’idea iniziale è stata quella di utilizzare un singolo sensore per la misura di corrente e tensione, un unico PowerModule inserito tra l’alimentazione e il cablaggio del motore, con la possibilità di alimentare Arduino direttamente dall’uscita 5V del PowerModule.

Effettuando vari tentativi, si è notato che il PowerModule crea non pochi problemi nella misura di piccole correnti. Questo problema parte dal fatto che la corrente viene misurata con uno shunt resistivo, con resistenza molto bassa, per far sì che non si vada a falsare la misura. Questo però comporta la lettura di una tensione molto bassa sullo shunt, che avrà bisogno di essere amplificata.

Durante i vari esperimenti si è visto che il problema può essere causato da un fenomeno di correnti parassite sui cavi dai quali si effettua la misura, oltre che dalla rumorosità del dato letto. Aggiungere un amplificatore, avrebbe solamente amplificato il rumore.

Si è optato per l’acquisto di un sensore di corrente ACS712, basato su effetto Hall, che, isolato galvanicamente, fornisce una misura precisa della corrente circolante nel circuito.

Il sensore è alimentato a 5V e fornisce in uscita un segnale analogico (tensione) proporzionale alla corrente.

> [Link al Datasheet](https://homotix_it.e-mind.it/upld/repository/File/datasheet%20acs712.pdf)
> 

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%204.png)

Sul Datasheet è indicata una sensibilità in uscita tra i 66 e 185 mV/A  e un rumore picco-picco di 22mV. Lavorando con correnti basse, la misura risulta molto imprecisa, creando il bisogno di effettuare una media del segnale nel tempo per stabilire un valore più veritiero.

Per effettuare una misurazione corretta, prima di essere utilizzato all’interno del progetto, il sensore è stato attentamente calibrato con l’alimentatore da banco.

---

```cpp
if (sampleCount < numSamples) {
    // ...verifica che sia trascorso l'intervallo desiderato
    if (currentTime - lastSampleTime >= sampleInterval) {
      lastSampleTime = currentTime;
      int reading = analogRead(AMPERE_PIN);      // Legge il valore dal sensore
      sampleSum += reading;              // Accumula il campione
      sampleCount++;

    }
  } 
  // Una volta acquisiti tutti i campioni, calcola la media e la corrente
  else {
    float avgAcs = sampleSum / numSamples;  // Media dei campioni
    // Oppure, se preferisci utilizzare la formula fornita per il calcolo della corrente:
    float current = -(avgAcs - 517.55) / 13.0;
    
    // Stampa il risultato sul monitor seriale
    globalData.setCurrent(current);
    
    // Resetta le variabili per iniziare una nuova acquisizione
    sampleCount = 0;
    sampleSum = 0.0;
  }
```

## Sensore di Tensione

Il sensore di tensione è stato impiegato per la misura della tensione di alimentazione fornita dall’alimentatore da banco all’intero sistema.

Per la misurazione è stato utilizzato un Power Module Gm v1.0 che ha permesso di leggere in uscita una tensione proporzionale a quella fornita, attraverso un segnale analogico.

Il sensore è alimentato direttamente dalla tensione erogata dall’alimentatore verso il sistema.

> Questo power module non ha un datasheet disponibile online
> 

---

```cpp
if (sampleCount < numSamples) {
    // ...verifica che sia trascorso l'intervallo desiderato
    if (currentTime - lastSampleTime >= sampleInterval) {
      lastSampleTime = currentTime;
      int reading = analogRead(VOLTAGE_PIN);      // Legge il valore dal sensore
      sampleSum += reading;              // Accumula il campione
      sampleCount++;

    }
  } 
  // Una volta acquisiti tutti i campioni, calcola la media e la corrente
  else {
    float avgVcs = sampleSum / numSamples;  // Media dei campioni
    // Oppure, se preferisci utilizzare la formula fornita per il calcolo della corrente:
    float voltage = (7*avgVcs/133) + 0.2632;
    
    // Stampa il risultato sul monitor seriale
    globalData.setVoltage(voltage);
    
    // Resetta le variabili per iniziare una nuova acquisizione
    sampleCount = 0;
    sampleSum = 0.0;
  }
```

## Sensore di Temperatura

Il sensore di temperatura è stato adottato allo scopo di monitorare la temperatura del motore per tracciare la caratteristica di: Tempo/Temperatura.

![KY-028](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%205.png)

KY-028

All’inizio si è optato per l’uso di un **thermistor di tipo NTC** (Negative Temperature Coefficient - resistenza inversamente proporzionale alla temperatura), nello specifico il modulo KY-028, che fornisce in uscita un segnale analogico proporzionale alla resistenza del materiale (effetto termoresistivo).

Il modulo ha una configurazione interna di tipo high-side con resistenza di riferimento $R_1 = 10000.0 Ω$, range di temperature $-55°C / +125°C$, range di tensioni operative $3.3V - 5.5V$ e accuratezza $±0.5°C$.

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%206.png)

Il modulo è stato caratterizzato tramite lo **Steinhart-Hart Model**:

$$
1/T=A + B*ln(R)+C*ln(R)^3
$$

con $R$ la resistenza del NTC e con coefficienti:
$A = 0.001129148, B = 0.000234125, C = 0.0000000876741$ .

Il modulo però adottava un comportamento anomalo quando messo a contatto con le bobine del motore (unica zona statica a cui era possibile avvicinarsi) iniziando a surriscaldarsi e fornendo letture irregolari. 

![MLX90614](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%207.png)

MLX90614

Data l’assenza di zone di contatto diverse dalle bobine, l’uso di sensori di temperatura a contatto è stato scartato e si è scelto un sensore a infrarossi, per la precisione un modulo **MLX90614**. Questo sensore sfrutta un chip con rilevatore a termopila sensibile all'IR, la radiazione infrarossa generata dal corpo analizzato produce una differenza di potenziale agli estremi della termopila che è proporzionale alla temperatura del corpo che ha prodotto l’onda.

Il modulo presenta, integrati, un amplificatore a basso rumore e un ADC a 17 bit, che hanno permesso letture con attenuazione del rumore e tramite un segnale PWM. Le letture sono state effettuate tramite libreria **Adafruit MLX90614**, messa a disposizione per Arduino.

In fine, il sensore è risultato equivalentemente valido, presentando range operativo di temperatura $-40°C/+125˚C$, range di temperature misurabili $-70°C/+380˚C$, accuratezza di $±0.5°C$ da $0°C/+50˚C$, range di tensioni operative $3.3V - 5.5V$.

## Sensore di RPM

Per leggere la velocità alla quale ruota il motore, si ha la possibilità di utilizzare diversi strumenti sul mercato. In questo progetto si è optato per cercare la migliore soluzione a basso costo, sfruttando quelli che sono i sensori base dei classici kit Arduino, scegliendo il modulo più adatto in base al proprio principio di funzionamento.

I due moduli presi in considerazione sono stati:

- Obstacle Avoidance Sensor Module (KY-032)
- Line Tracking Sensor Module (KY-033)

![KY-032](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/57dd6d37-476d-4359-a5f9-9ba254ff0066.png)

KY-032

![KY-033](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/4973ece4-ec6a-4895-8dca-a9b0703590c6.png)

KY-033

Il primo è un sensore fotoelettrico a infrarossi per il rilevamento di ostacoli.
Il sensore, attraverso un apposito chip, genera un’onda quadra a 38kHz, utilizzata per illuminare un LED a infrarossi, dopodiché l’IR riflesso dall’ostacolo viene rilevato da un ricettore IR, che incorporando un filtro IR ottico esterno, e un filtro passa-banda interno a 38kHz rende il modulo ricettivo solo alla luce IR pulsante a quella frequenza.
Questo modulo restituisce in uscita un segnale che descrive il livello TTL (Time To Live), con soglia regolabile mediante potenziometro.

Il secondo, quello scelto per il progetto, ha un principio di funzionamento basato sul fatto che la luce infrarossa emessa dal LED rimbalzando su una superficie va a colpire il fototransistor permettendo il passaggio di corrente. A seconda della quantità di luce riflessa dalla superficie, la resistenza del fototransistor cambia e può essere misurata da un circuito esterno per determinare la posizione, nel nostro caso, di un nastro bianco posizionato sul motore.
Per ottenere la massima precisione, il nastro bianco è stato applicato sulla metà della superficie laterale del motore, in modo tale che coprisse le scritte presenti, e che alla massima velocità potessero ugualmente essere rilevati i fronti di salita e discesa.
Il funzionamento è completamente analogico, il comparatore ha un tempo di risposta di 1.3µs, quindi l’unica limitazione, che non permette di rilevare RPM troppo elevati, è la scheda Arduino in uso.

## Sensore di Spinta

Per monitorare la spinta esercitata dal motore nei vari casi applicativi, quest’ultimo è stato collegato all’estremità di una cella di carico, poi fissata alla struttura atta al sostenimento dell’intera apparecchiatura.

Per l’esattezza si è fatto uso di una **celle di carico a punto singolo**, che funziona grazie a un elemento metallico elastico che si deforma sotto carico, la cui deformazione, viene rilevata da estensimetro (**foil type strain gauges**), tramite la variazione di resistenza, e trasformata in un segnale elettrico. Poiché si tratta di piccole resistenze con piccole variazioni sotto deformazione, la lettura del segnale è molto debole e soggetta rumore a causa degli effetti dovuti alla temperatura e alle tensioni spurie generate lungo i conduttori. 

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%208.png)

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%209.png)

Per effettuare misure più precise si utilizzano più strain gauges (2 o 4) disposti in configurazione opposta (push-pull), in modo che siano soggette alla stessa intensità di deformazione ma di tipo opposto, e posti all’interno di un ponte di Wheatstone, in modo da leggere una tensione che viene amplificata il più possibile a seconda del tipo di predisposizione (tanto maggiore quanto maggiore è la disparità tra i prodotti delle resistenze su due rami paralleli del ponte).  

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%2010.png)

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%2011.png)

Per l’interfacciamento tra il ponte e l’Arduino è stato sfruttato un modulo **HX711,** un convertitore analogico-digitale (ADC) a 24 bit di precisione e frequenza di funzionamento a $10Hz$, usato in bilance e applicazioni industriali per leggere direttamente segnali da sensori a ponte (come le celle di carico), con amplificazione integrata e gestione semplificata grazie a clock e alimentazione interni. Per la lettura della cella di carico si è utilizzato il canale A del modulo con guadagno di 128.

![image.png](Relazione%20Thrust%20Stand%201acec7177fc980e29f7ec16b75a74b64/image%2012.png)

In fine, Arduino per poter processare i segnali provenienti dal modulo HX711 richiede l’utilizzo di due script (forniti direttamente dall’IDE):

- **HX Calibration**: che sottoponendo la cella di carico a sollecitazioni note e fornendo in input il valore della sollecitazione (in grammi) restituisce due valori, **Offset** e **Scale**. Per la tara si sono fatte seguire più misure con carichi noti e si è poi calcolata la media dei vari valori di Offset e Scale in modo da ridurre l’incertezza di misura.
- **HX Plotter**: che forniti i valori di Offset e Scale riporta in output i valori di spinta esercitati sulla cella (in grammi).

## Sensore di Rumore

Per valutare l’intensità del rumore generato dalla rotazione del motore e dell’elica, si è scelto di usare il Microphone Sensor Module (High Sensitivity | KY-037).

Questo sensore presenta due diversi pin per il segnale in uscita, un pin per l’uscita analogica, che restituisce in tempo reale il segnale di tensione di uscita del microfono, e un pin di uscita digitale, in cui quando l'intensità del suono raggiunge una certa soglia, l'uscita del modulo è a livello alto, mentre quando l'intensità del suono è inferiore alla soglia impostata, è basso.

L'intensità del segnale è stata determinata a partire dal calcolo del valore picco-picco della tensione, ovvero la differenza tra il massimo e il minimo valore della tensione misurata durante il periodo di osservazione. Questo valore rappresenta l'ampiezza massima del segnale, che è correlata all'intensità del rumore.

Successivamente, il valore picco-picco è stato trasformato in decibel (dB) utilizzando la formula:

$$
L_{dB}=20⋅log⁡(V_{pp}/V_{ref})
$$

dove $V_{pp}$ è il valore picco-picco della tensione misurata e $V_{ref}$ è una tensione di riferimento.

Infine, per garantire accuratezza nelle misurazioni, il segnale in dB è stato calibrato aggiungendo un offset. Questo offset è stato determinato confrontando i valori di dB misurati con un dispositivo di misura del rumore più preciso e con un livello di rumore di riferimento noto, per correggere eventuali errori o deviazioni dovuti alla sensibilità del sensore. In questo modo, si è ottenuto un grafico preciso e accurato dell'andamento del rumore.

---

```cpp
void processAudioSample() {
    static unsigned long startMillis = 0;
    static unsigned int signalMax = 0;
    static unsigned int signalMin = 1024;
    static bool sampling = false;
    static float ema = 0;          // Valore EMA per la media esponenziale
    const float alpha = 0.1;       // Fattore di smoothing, 0 < alpha <= 1
    static bool firstRun = true;   // Flag per inizializzare la EMA

    unsigned long currentMillis = millis();

    if (!sampling) {
        // Avvio del periodo di campionamento
        startMillis = currentMillis;
        signalMax = 0;
        signalMin = 1024;
        sampling = true;
    }

    // Campionamento in corso
    if (currentMillis - startMillis < sampling_period) {
        int sample = analogRead(MICROPHONE_PIN);
        if (sample < 1024)  // Scarta valori anomali
            if (sample > signalMax) signalMax = sample;
            if (sample < signalMin) signalMin = sample;
    } else {
        // Fine del periodo di campionamento
        float peakToPeak = signalMax - signalMin;
        if (peakToPeak != 0) {
            float y = 55.5 + 1.2 * 10 * log(peakToPeak);
            
            // Applica la media mobile esponenziale (EMA)
            if (firstRun) {
                ema = y;
                firstRun = false;
            } else {
                ema = alpha * y + (1 - alpha) * ema;
            }
            globalData.setNoise(ema);
        }

        // Reset per il prossimo ciclo
        sampling = false;
    }
}

```

## Implementazioni future

Per migliorare l'efficienza complessiva del sistema, si potrebbero apportare diverse ottimizzazioni. Innanzitutto, la sostituzione di Arduino con una piattaforma STM permetterebbe di ottenere prestazioni superiori in termini di velocità di elaborazione e gestione delle periferiche. Inoltre, l'adozione di sensori più sensibili migliorerebbe la precisione delle misurazioni.

Un ulteriore passo avanti sarebbe la progettazione e la stampa di un circuito PCB dedicato, che consentirebbe di ridurre al minimo il rumore elettrico generato dai collegamenti via cavo, migliorando così l'affidabilità del sistema. Infine, la realizzazione di una struttura più resistente e, possibilmente, chiusa garantirebbe una maggiore protezione dei componenti da fattori esterni, aumentando la durabilità e la robustezza complessiva del dispositivo.

Dal punto di vista software, il protocollo è stato progettato per supportare diverse tipologie di curve come parametri in input. Questo apre la possibilità di implementare nuove funzionalità, come ad esempio un profilo di accelerazione esponenziale, che potrebbe aprire l’analisi del motore in maggiori scenari applicativi.