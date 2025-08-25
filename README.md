# 🖥️ Arduino Mini-Shell

A command-line interface to interact with your Arduino board via the Serial Monitor.

---

## 🔧 Usage

### Open the Serial Monitor
Call `shellInit()` in your `setup()` function.
You can also call the `setPrompt()` function to change the prompt as you like.

### Start the Arduino Shell
Call `shellTask()` in your `loop()` function to allow the shell to capture your inputs.

### Upload the code to your Arduino board
Use the Arduino IDE to upload the sketch to your board.

### Start typing commands
After the prompt `arduino:~$`, you can enter the available commands.

---

## 🛠️ Available Commands

### `up pin <number> <mode>`
Configure a pin as `INPUT`, `OUTPUT`, or `INPUT_PULLUP`.

**Example:**
arduino:~$ up pin 13 OUTPUT
PIN 13 set to OUTPUT

### `up analog <number> <value>`
Set an analog pin with a PWM value (0-255).

**Example:**
arduino:~$ up analog 3 255
PIN 3 set to OUTPUT
Analog value: 255

### `down <number>`
Set a digital pin to `LOW`.

**Example:**
arduino:~$ down 13
PIN 13
state: DOWN

### `read <number>`
Read the analog value of an analogic pin (0-1023).

**Example:**
arduino:~$ read 4
Reading A4
value: 512

### `list`
Display the current status of all configured pins.

**Example:**
arduino:~$ list
[13] Digital(PWM) OUTPUT
[3] Analog OUTPUT
