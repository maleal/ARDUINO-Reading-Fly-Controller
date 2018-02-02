#ifndef _PID_H_
#define _PID_H_

enum {
    XAXIS_PID_IDX = 0,
    YAXIS_PID_IDX,
    ZAXIS_PID_IDX,
    LAST_PID_IDX  // keep this definition at the end of this enum
};

//// PID Variables
struct PIDdata {
    float P, I, D;
    float lastError;
    // AKA experiments with PID
    float previousPIDTime;
    float integratedError;
    float windupGuard; // Thinking about having individual wind up guards for each PID
} PID[LAST_PID_IDX];


void initializePIDs() {
    PID[XAXIS_PID_IDX].P = 3.0;
    PID[XAXIS_PID_IDX].I = 0.5;
    PID[XAXIS_PID_IDX].D = 2.5;
    PID[YAXIS_PID_IDX].P = 3.0;
    PID[YAXIS_PID_IDX].I = 0.05;
    PID[YAXIS_PID_IDX].D = 2.5;
    PID[ZAXIS_PID_IDX].P = 3.5;
    PID[ZAXIS_PID_IDX].I = 5.0;
    PID[ZAXIS_PID_IDX].D = 1.0;
}
// This struct above declares the variable PID[] to hold each of the PID values for various functions
// The following constants are declared in AeroQuad.h
// ROLL = 0, PITCH = 1, YAW = 2 (used for Arcobatic Mode, gyros only)
// ROLLLEVEL = 3, PITCHLEVEL = 4, LEVELGYROROLL = 6, LEVELGYROPITCH = 7 (used for Stable Mode, accels + gyros)
// HEADING = 5 (used for heading hold)
// ALTITUDE = 8 (used for altitude hold)
// ZDAMPENING = 9 (used in altitude hold to dampen vertical accelerations)

//
float updatePID(float targetPosition, float currentPosition, struct PIDdata *PIDparameters) {

    // AKA PID experiments
    const float deltaPIDTime = (currentTime - PIDparameters->previousPIDTime) / 1000000.0;
  
    PIDparameters->previousPIDTime = currentTime;  // AKA PID experiments
    float error = targetPosition - currentPosition;
  
    if (inFlight) {
      PIDparameters->integratedError += error * deltaPIDTime;
    }
    else {
      PIDparameters->integratedError = 0.0;
    }
        
    //PIDparameters->integratedError = constrain(PIDparameters->integratedError, -PIDparameters->windupGuard, PIDparameters->windupGuard);
    float dTerm = ( error - PIDparameters->lastError ) / deltaPIDTime;
    
    //float dTerm = PIDparameters->D * (currentPosition - PIDparameters->lastError) / (deltaPIDTime * 100); // dT fix from Honk
    //PIDparameters->lastError = currentPosition;

    PIDparameters->lastError = error;
    return (PIDparameters->P * error) + (PIDparameters->I * PIDparameters->integratedError) + dTerm;
}

void zeroIntegralError() __attribute__ ((noinline));
void zeroIntegralError() {
  for (byte axis = 0; axis <= ATTITUDE_YAXIS_PID_IDX; axis++) {
    PID[axis].integratedError = 0;
    PID[axis].previousPIDTime = currentTime;
  }
}

#endif //_PID_H_


