#ifndef PID_H_
#define PID_H_

void pidSetupConstants(double Kp_, double Ki_, double Kd_);
void pidUpdateReference(float reference_);
double pidControl(double saida_medida);

#endif /* PID_H_ */

