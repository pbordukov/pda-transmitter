#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "RF24_config.h"
#include "printf.h"

unsigned long then = 0;
unsigned long now = 0;
const uint64_t pipe = 0xF0F0F0F0C3LL;
RF24 radio(A0, 10);
int8_t high[12];
int8_t low[12];
void setup(void)
{
  Serial.begin(9600);
  printf_begin();
  pinMode(10, OUTPUT);
  pinMode(A1, OUTPUT);

  for (int i = 2; i <= 9; i++)
    pinMode(i, INPUT);

  for (int i = 2; i <= 9; i++)
    digitalWrite(i, HIGH);
  delay(50);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(0);
  radio.setRetries(1, 15);
  radio.setChannel(0);
  radio.stopListening();
  radio.openWritingPipe(pipe);
  radio.printDetails();
}
/* msg[12]
[0] = radio_hp_bonus
[1] = radio_hp_armor_bonus
[2] = radio_hp_mod
[3] = radio_psy_bonus
[4] = radio_psy_armor_bonus
[5] = radio_psy_mod
[6] = radio_rad_bonus
[7] = radio_rad_armor_bonus
[8] = radio_rad_mod
[9] = radio_inf_mod
[10] = radio_type_mod
[11] = radio_multi_mod */
void loop()
{
  now = millis();
  if (now - then >= 800)
  {
    if (!digitalRead(2))
    {
      high[10] = 1;
      high[11] = 1;
      low[2] = 1;
      low[5] = 0;
      low[8] = -1;
    }
    if (!digitalRead(3))
    {
      high[10] = 2;
      low[2] = 5;
      low[8] = 1;
    }
    if (!digitalRead(4))
    {
      high[10] = 5;
      low[2] = -2;
    }
    if (!digitalRead(5))
    {
      high[10] = 8;
      low[9] = random(6,10);
    }
    if (!digitalRead(6))
    {
      low[1] = 1;
      low[4] = 1;
      low[7] = 1;
    }
    if (!digitalRead(7))
    {
      low[11] = 10;
    }
    if (!digitalRead(8))
    {
      low[11] = 21;
    }
    if (!digitalRead(9))
    {
      low[11] = 2;
    }

    radio.setPALevel(RF24_PA_MIN);
    delay(50);
    radio.write(&low, sizeof(low));
    delay(50);
    radio.setPALevel(RF24_PA_MAX);
    delay(50);
    radio.write(&high, sizeof(high));
    delay(50);
    Serial_diag();
    then = millis();
    for (int8_t i = 0; i <= 11; i++) high[i] = 0;
    for (int8_t i = 0; i <= 11; i++) low[i] = 0;
  }

}

void Serial_diag()
{
      Serial.println("");
    Serial.print("LOW ");
    for (int8_t i = 0; i<=11; i++)
    {
    Serial.print(low[i]);
    Serial.print(" ");
    }
    Serial.print("HIGH ");
    for (int8_t i = 0; i<=11; i++)
    {
    Serial.print(high[i]);
    Serial.print(" ");
    }
    Serial.println("");
}

