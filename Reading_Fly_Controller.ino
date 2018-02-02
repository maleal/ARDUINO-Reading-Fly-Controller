#include "Reading_Fly_Controller.h"


/* Arduino Mega 2560 External Interrupts:
   Vista: Pin ASC a Interrupt
   2, 3, 18, 19, 20, 21 
   Inp. Pin 2 (interrupt 0), Inp. Pin 3 (interrupt 1), Inp. Pin 18 (interrupt 5), Inp. Pin 19 (interrupt 4), Inp. Pin 20 (interrupt 3), Inp. Pin and 21 (interrupt 2). 
   These pins can be configured to trigger an interrupt on a low level, a rising or falling edge, or a change in level. See the attachInterrupt() function for details.
*/

/* Arduino Mega 2560 External Interrupts:
   Vista: Interrupcion ASC a Pin
   2, 3, 21, 20, 19, 18.
   Inp. Pin 2 (interrupt 0), Inp. Pin 3 (interrupt 1), Inp. Pin and 21 (interrupt 2), Inp. Pin 20 (interrupt 3), Inp. Pin 19 (interrupt 4), Inp. Pin 18 (interrupt 5).
   These pins can be configured to trigger an interrupt on a low level, a rising or falling edge, or a change in level. See the attachInterrupt() function for details.
*/



//volatile uint16_t rc_start[MAX_NUM_CHANNEL];
uint16_t rc_values[MAX_NUM_CHANNEL];
//volatile uint16_t rc_shared[MAX_NUM_CHANNEL];


void setup() {
  
  pRC_Obj = create_RC_input();
  enableInterrupt(RC_CH1_ROLL_INPUT_PIN,          isr_rc_read_roll_cmd,     CHANGE);
  enableInterrupt(RC_CH2_PITCH_INPUT_PIN,         isr_rc_read_pitch_cmd,    CHANGE);
  enableInterrupt(RC_CH3_THROTTLE_INPUT_PIN,      isr_rc_read_throttle_cmd, CHANGE);
  enableInterrupt(RC_CH4_YAW_INPUT_PIN,           isr_rc_read_yaw_cmd,      CHANGE);
  enableInterrupt(RC_CH5_AUX1_INPUT_PIN,          isr_rc_read_aux1_cmd,     CHANGE);
  enableInterrupt(RC_CH6_AUX2_INPUT_PIN,          isr_rc_read_aux2_cmd,     CHANGE);
  
}

void loop() {
 // rc_read_values();
  unsigned long currentTime = 0;
  currentTime = micros();
  
  if( bReceiverCommand() ) {
      Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
      Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
      Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
      Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]); Serial.print("\t");
      Serial.print("CH5:"); Serial.println(rc_values[RC_CH5]); Serial.print("\t");
      Serial.print("CH6:"); Serial.println(rc_values[RC_CH6]); Serial.print("\t");
  }
  
    
  
  

  delay(200);
}

