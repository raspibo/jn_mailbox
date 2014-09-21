/*
 * JeeNode Mailbox - sender node.
 *
 * When powered on, send a message informing that the mailbox was open.
 * After STILL_OPEN_DELAY ms, send a message informing that the mailbox
 * was left open and then shutdown itself.
 *
 * Copyright (c) 2014 Davide Alberani <da@erlug.linux.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <JeeLib.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include <JNMailbox.h>


const int NODE_ID = 2;
const int GROUP_ID = 212;
const int TRANSMIT_INTERVAL = 0;
const unsigned long int STILL_OPEN_DELAY = 30000;
const unsigned long int EXTRA_TIME_BEFORE_SHUTDOWN = 10000;

unsigned long int boot_time;
boolean boot_message_sent = false;
boolean still_open_sent = false;

Port port(1);
JNMessage jnmessage;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }


void setup() {
  // Prepare the rf12 radio (only for JeeNode Micro).
  bitSet(DDRB, 0);
  bitClear(PORTB, 0);
  rf12_initialize(NODE_ID, RF12_868MHZ, GROUP_ID);
  randomSeed(port.anaRead());
  // Set up easy transmissions.
  rf12_easyInit(TRANSMIT_INTERVAL);
  boot_time = millis();
}


void loop() {
  if (!boot_message_sent) {
    jnmessage.door_status = DOOR_OPEN;
    jnmessage.rnd = random(32000);
    rf12_easySend(&jnmessage, sizeof(jnmessage));
    boot_message_sent = true;
  }

  // Keep the easy transmission mode going.
  rf12_easyPoll();
  
  unsigned long int current_millis = millis();

  if (current_millis - boot_time > STILL_OPEN_DELAY + EXTRA_TIME_BEFORE_SHUTDOWN) {
    // Put everything to sleep.
    Sleepy::watchdogInterrupts(0);
    rf12_sleep(0);
    Sleepy::powerDown();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    power_adc_disable();
    power_timer0_disable();
    power_timer1_disable();
    sleep_mode();
    return;
  }

  if (!still_open_sent && current_millis - boot_time > STILL_OPEN_DELAY) {
    jnmessage.door_status = DOOR_STILL_OPEN;
    jnmessage.rnd = random(32000);
    rf12_easySend(&jnmessage, sizeof(jnmessage));
    still_open_sent = true;
  }
}

