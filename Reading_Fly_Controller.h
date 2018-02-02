#ifndef _READIN_FLY_CONTROLLLER_H_
#define _READIN_FLY_CONTROLLLER_H_

#define EI_ARDUINO_INTERRUPTED_PIN // to enable pin states functionality 
#include <EnableInterrupt.h>

#define RC_CH1  0   //Rolll input
#define RC_CH2  1   //Pitch
#define RC_CH3  2   //Throttle
#define RC_CH4  3   //Yaw
#define RC_CH5  4   //Aux 1
#define RC_CH6  5   //Aux 2
#define MAX_NUM_CHANNEL   6


#define RC_CH1_ROLL_INPUT_PIN       2//  A0
#define RC_CH2_PITCH_INPUT_PIN      3//  A1
#define RC_CH3_THROTTLE_INPUT_PIN   21// A2
#define RC_CH4_YAW_INPUT_PIN        20// 
#define RC_CH5_AUX1_INPUT_PIN       19// 
#define RC_CH6_AUX2_INPUT_PIN       18//

#define MIN_SIGNAL_RC_INP     -45   // controls the maximum lean angle which by default is 450 (i.e. 45 degrees)
#define MAX_SIGNAL_RC_INP     45    //
#define MIN_THROTTLE_RC_INP   1000
#define MAX_THROTTLE_RC_INP   2000
#define NEUTRAL_SIGNAL_RC_INP 1300
#define RC_PWM_MIN            1000
#define RC_PWM_MAX            2000

extern uint16_t rc_values[MAX_NUM_CHANNEL];

class RC_Input {
  private:
    volatile uint16_t rc_start[MAX_NUM_CHANNEL];
    volatile uint16_t rc_shared[MAX_NUM_CHANNEL];
  public:
    RC_Input() {
      for(int i=0; i<MAX_NUM_CHANNEL; i++)
          rc_shared[i]=1500;
      
    }
    void read_roll_cmd () {
        this->read_input(RC_CH1, RC_CH1_ROLL_INPUT_PIN);
    }

    void read_yaw_cmd () {
        this->read_input(RC_CH4, RC_CH4_YAW_INPUT_PIN);
    }

    void read_pitch_cmd() {
        this->read_input(RC_CH2, RC_CH2_PITCH_INPUT_PIN);
    }

    void read_throttle_cmd() {
        this->read_input(RC_CH3, RC_CH3_THROTTLE_INPUT_PIN);
    }

    void read_aux1_cmd()      { this->read_input(RC_CH5, RC_CH5_AUX1_INPUT_PIN); }
    void read_aux2_cmd()      { this->read_input(RC_CH6, RC_CH6_AUX2_INPUT_PIN); }
    
    const uint16_t * get_rc_commands_inputs(){
        noInterrupts();
        for(int i=0; i<MAX_NUM_CHANNEL; i++){
          switch( i ) {
            case RC_CH1: case RC_CH2: case RC_CH4:
              rc_shared[i] = constrain(rc_shared[i], RC_PWM_MIN, RC_PWM_MAX);
              rc_shared[i] = map(rc_shared[i], RC_PWM_MIN, RC_PWM_MAX, MIN_SIGNAL_RC_INP, MAX_SIGNAL_RC_INP);  //micro segundos
              break;
            case RC_CH3:
              rc_shared[i] = constrain(rc_shared[i], RC_PWM_MIN, RC_PWM_MAX);
              rc_shared[i] = map(rc_shared[i], RC_PWM_MIN, RC_PWM_MAX, MIN_THROTTLE_RC_INP, MAX_THROTTLE_RC_INP);  //micro segundos
              break;
            default:
              break;
          }
        }
        interrupts();
        return rc_shared;   
    }
    
 private:
    void read_input(uint8_t channel, uint8_t input_pin) {
      if (digitalRead(input_pin) == HIGH) {
        rc_start[channel] = micros();
      } else {
        rc_shared[channel] = (uint16_t)( micros() - rc_start[channel] );
      }
    }
  
};

#ifdef __cplusplus
extern "C" {
  void *pRC_Obj=NULL;
  
  void *create_RC_input() {
    return new RC_Input();
  }
  
  void rc_read_roll_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_roll_cmd();
  }
  void rc_read_yaw_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_yaw_cmd();
  }
  void rc_read_pitch_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_pitch_cmd();
  }
  void rc_read_throttle_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_throttle_cmd();
  }
  void rc_read_aux1_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_aux1_cmd();
  }
  void rc_read_aux2_cmd(void *anObject) {
    static_cast<RC_Input*>(anObject)->read_aux2_cmd();
  }
  
  
  void isr_rc_read_roll_cmd() {
      rc_read_roll_cmd(pRC_Obj);
  }
  void isr_rc_read_yaw_cmd() {
      rc_read_yaw_cmd(pRC_Obj);
  }
  void isr_rc_read_pitch_cmd() {
      rc_read_pitch_cmd(pRC_Obj);
  }
  void isr_rc_read_throttle_cmd() {
      rc_read_throttle_cmd(pRC_Obj);
  }
  void isr_rc_read_aux1_cmd() {
      rc_read_aux1_cmd(pRC_Obj);
  }
  void isr_rc_read_aux2_cmd() {
      rc_read_aux2_cmd(pRC_Obj);
  }

  bool bReceiverCommand() {
      uint16_t * tmp;
      bool ret;
      if( pRC_Obj != NULL){
        tmp = static_cast<RC_Input*>(pRC_Obj)->get_rc_commands_inputs();
        memcpy(rc_values, (const void *)tmp, sizeof(uint16_t)*MAX_NUM_CHANNEL);
        ret=true;
      }else{
        ret=false;
      }
      return ret;
  }

}
#endif
#endif //_READIN_FLY_CONTROLLLER_H_
