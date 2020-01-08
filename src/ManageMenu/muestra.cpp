
/* The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <LiquidCrystal.h>
#include <IRremote.h>

#define KEYS 13
#define COMMANDS 1
#define IR_PIN 19
#define LINE_MAX 16

typedef void (*command)(const struct Key * c, struct State * s);

IRrecv irrecv(IR_PIN);

struct Key {
  long ID;
  int value;
  command cmd;
};

struct State {
  LiquidCrystal * lcd;
  char line[LINE_MAX];
  char position;
} state;

Key record[KEYS];

void setup() {
  memset(record, 0, sizeof(record));

  record[0].ID = 0xFF6897;
  record[0].value = 0;
  record[0].cmd = cmd_write;
  record[1].ID = 0xFF30CF;
  record[1].value = 1;
  record[1].cmd = cmd_write;
  record[2].ID = 0xFF18E7;
  record[2].value = 2;
  record[2].cmd = cmd_write;
  record[3].ID = 0xFF7A85;
  record[3].value = 3;
  record[3].cmd = cmd_write;
  record[4].ID = 0xFF10EF;
  record[4].value = 4;
  record[4].cmd = cmd_write;
  record[5].ID = 0xFF38C7;
  record[5].value = 5;
  record[5].cmd = cmd_write;
  record[6].ID = 0xFF5AA5;
  record[6].value = 6;
  record[6].cmd = cmd_write;
  record[7].ID = 0xFF42BD;
  record[7].value = 7;
  record[7].cmd = cmd_write;
  record[8].ID = 0xFF4AB5;
  record[8].value = 8;
  record[8].cmd = cmd_write;
  record[9].ID = 0xFF52AD;
  record[9].value = 9;
  record[9].cmd = cmd_write;
  record[10].ID = 0xFF9867;
  record[10].value = 0;
  record[10].cmd = cmd_clear;
  record[11].ID = 0xFF02FD;
  record[11].value = -1;
  record[11].cmd = cmd_move;
  record[12].ID = 0xFFC23D;
  record[12].value = 1;
  record[12].cmd = cmd_move;

  // Serial
  Serial.begin(19200);
  // Setup IR
  irrecv.enableIRIn(); // Start the receiver
  // Set up the LCD
  // initialize the library with the numbers of the interface pins
  state.lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
  state.lcd->begin(16, 2);
  state.lcd->print("blog.drk.com.ar");
  state.lcd->cursor();
  state.lcd->setCursor(state.position, 1);
}

void loop() {
  decode_results results;

  if (irrecv.decode(&results)
      && (results.value & 0xFF0000 == 0x00FF0000)) {
    Serial.print("IR: ");
    Serial.println(results.value, HEX);

    for(int i=0; i < KEYS; ++i) {
      if (record[i].ID == results.value) {
        Serial.println(record[i].value);

        if (record[i].cmd != NULL)
          (*record[i].cmd)(&record[i], &state);
      }
    }

    irrecv.resume();
  }
}

void cmd_write(const struct Key * r, struct State * s)
{
  // write the value in the string
  s->line[s->position] = r->value + 48;
  s->lcd->print(s->line[s->position]);
  s->position++;
  // Limit position to LINE_MAX-1
  if (s->position == LINE_MAX)
    s->position = LINE_MAX - 1;
  s->lcd->setCursor(s->position, 1);
}
void cmd_clear(const struct Key * r, struct State * s)
{
  for(int i=0; i<LINE_MAX; ++i) {
    s->line[i] = ' ';
    s->lcd->setCursor(i, 1);
    s->lcd->print(s->line[i]);
    s->position = 0;
    s->lcd->setCursor(s->position, 1);
  }
}
void cmd_move(const struct Key * r, struct State * s)
{
  s->position += r->value;
  if (s->position == LINE_MAX)
    s->position = LINE_MAX - 1;
  if (s->position < 0)
    s->position = 0;
  s->lcd->setCursor(s->position, 1);
}
