# ArduinoShell
Interactive Arduino shell for sending commands
🖥️ Arduino Shell

Un'interfaccia a riga di comando per interagire con la tua scheda Arduino tramite il Monitor Seriale dell'IDE Arduino o terminali come minicom e CoolTerm.

🔧 Installazione

Carica il codice sulla tua scheda Arduino: Utilizza l'IDE Arduino per caricare lo sketch sulla tua scheda.

Apri il Monitor Seriale: Imposta la velocità di trasmissione a 115200 baud e la terminazione a "Newline".

Inizia a digitare i comandi: Dopo il prompt arduino:~$, puoi inserire i comandi disponibili.

🛠️ Comandi Disponibili
up pin <numero> <modalità>

Configura un pin come INPUT, OUTPUT o INPUT_PULLUP.

Esempio:

arduino:~$ up pin 13 OUTPUT
PIN 13 set to OUTPUT


up analog <numero> <valore>

Imposta un pin analogico con un valore PWM (0-255).

Esempio:

arduino:~$ up analog 3 255
PIN 3 set to OUTPUT
Analog value: 255


down <numero>

Imposta un pin digitale come LOW.
docs.arduino.cc
+3
Amazon
+3
Arduino Official Store
+3

Esempio:

arduino:~$ down 13
PIN 13
state: DOWN


read <numero>

Legge il valore analogico di un pin (0-1023).

Esempio:

arduino:~$ read 4
Reading A4
value: 512


list

Mostra lo stato attuale di tutti i pin configurati.

Esempio:

arduino:~$ list
[13] Digital(PWM)    OUTPUT
[3] Analog    OUTPUT
