/*
 * JeeNode Mailbox - receiver node.
 *
 * Receive data from the mailbox and print them on the serial line.
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
#include <JNMailbox.h>

const int NODE_ID = 1;
const int GROUP_ID = 212;

const int TRANSMIT_INTERVAL = 0;

JNMessage jnmessage;


void setup() {
    // initialize the serial port and the RF12 driver
    Serial.begin(57600);
    rf12_initialize(NODE_ID, RF12_868MHZ, GROUP_ID);
    //rf12_easyInit(TRANSMIT_INTERVAL);
}


void loop() {
  //rf12_easyPoll();
  if (!rf12_recvDone()) {
    return;
  }
  if (rf12_crc) {
    Serial.println("warn: CRC error");
    return;
  }
  if (rf12_len != sizeof(JNMessage)) {
    Serial.print("warn: wrong data size: ");
    Serial.print(rf12_len);
    Serial.print(" bytes received; ");
    Serial.print(sizeof(JNMessage));
    Serial.println(" bytes expected.");
    return;
  }

  char door_status = -1;
  memcpy(&jnmessage, (JNMessage*) rf12_data, sizeof(JNMessage));
  Serial.print("message: ");
  if (jnmessage.door_status == DOOR_OPEN) {
    Serial.println("upper_door:open");
  } else if (jnmessage.door_status == DOOR_STILL_OPEN) {
    Serial.println("upper_door:still_open");
  }
  if (RF12_WANTS_ACK) {
    rf12_sendStart(RF12_ACK_REPLY, 0, 0);
  }
}
