
#include "pid.h"
#include "utask.h"

// low pass filter:
// f_cut = 1/(2*PI*cutoff_freq)//PI = 3.1415926,¹«Ê½£¬f = 1/2¦ÐRC,RCÓÃ20´øÈëµÃ7.9577e-3
// f_cut = 10 Hz -> _filter = 15.9155e-3
// f_cut = 15 Hz -> _filter = 10.6103e-3
// f_cut = 20 Hz -> _filter =  7.9577e-3
// f_cut = 25 Hz -> _filter =  6.3662e-3
// f_cut = 30 Hz -> _filter =  5.3052e-3
// f_cut = 5 Hz -> _filter = 31.830989e-3
// f_cut = 2 Hz -> _filter = 79.577473e-3
const float  lowpass_filter = 7.9577e-3;



void pidInit(struct PID* pid, float kp,
             float ki, float kd)
{
  pid->merror = 0;//e(t)
  pid->last_error = 0;//e(t-1)
  pid->Integrator = 0;//»ý·Ö
  pid->deriv = 0;//Î¢·Ö
  pid->target = 0;//Ä¿±êÁ¿
  pid->last_deriv = 0;//ÉÏ´ÎÎ¢·Ö
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;

  pid->Lowpass_EN = PID_FALSE;   //µÍÍ¨ÂË²¨	½ûÖ¹
  pid->iLimit = DEFAULT_PID_INTEGRATION_LIMIT;	//»ý·ÖÏÞÖÆ
}

float pidUpdate(struct PID* pid, float measured,float dt)
{
  float output;
  pid->current = measured;//²âÁ¿Öµ

  pid->merror = pid->target - measured;//Ä¿±êÁ¿-²âÁ¿Öµ = Æ«²îe(t)

  //I  »ý·Ö
  if(pid->Ki != 0){
  pid->Integrator += (pid->Ki * pid->merror) * dt;//I = Ki * e(t)
  if (pid->Integrator > pid->iLimit)//»ý·Ö±¥ºÍ´¦Àí
  {
    pid->Integrator = pid->iLimit;
  }
  else if (pid->Integrator < -pid->iLimit)
  {
    pid->Integrator = -pid->iLimit;
  }
  }
  //D Î¢·Ö
  pid->deriv = (pid->merror - pid->last_error) / dt;//¼ÆËãÎ¢·Ö£¬¼´ {e(k)-e(k-1)}/t
  if(pid->Lowpass_EN != PID_FALSE){	//µÍÍ¨ÂË²¨¡£½ØÖ¹ÆµÂÊ20hz
   pid->deriv = pid->last_deriv +
   			(dt / (lowpass_filter + dt)) * (pid->deriv - pid->last_deriv);	//D = D(k-1) + t/(f+t)*(D(k)-D(k-1))
  }                                                                     //

  pid->outP = pid->Kp * pid->merror;
  pid->outI = pid->Integrator;
  pid->outD = pid->Kd * pid->deriv;

  pid->PID_out = output = 	pid->outP +
           					pid->outI +
           					pid->outD;

  pid->last_error = pid->merror;

  return output;
}

float pidUpdate_err(struct PID* pid,float err, float dt)
{
  float output;
  pid->merror = err;
  //I
  pid->Integrator += (pid->Ki * pid->merror) * dt;
  if (pid->Integrator > pid->iLimit)
  {
    pid->Integrator = pid->iLimit;
  }
  else if (pid->Integrator < -pid->iLimit)
  {
    pid->Integrator = -pid->iLimit;
  }
  //D
  pid->deriv = (pid->merror - pid->last_error) / dt;
  if(pid->Lowpass_EN != PID_FALSE){	//µÍÍ¨ÂË²¨¡£½ØÖ¹ÆµÂÊ20hz
   pid->deriv = pid->last_deriv +
   			(dt / (lowpass_filter + dt)) * (pid->deriv - pid->last_deriv);
  }
  //P
  pid->outP = pid->Kp * pid->merror;
  pid->outI = pid->Integrator;
  pid->outD = pid->Kd * pid->deriv;

  pid->PID_out = output = 	pid->outP +
           					pid->outI +
           					pid->outD;

  pid->last_error = pid->merror;

  return output;
}

void pidSetError(struct PID* pid, float err)
{
  pid->merror = err;
}

void pidSetIntegralLimit(struct PID* pid, float limit)
{
  pid->iLimit = limit;
}

void pidReset(struct PID* pid)
{
  pid->merror = 0;
  pid->last_error = 0;
  pid->Integrator = 0;
  pid->deriv = 0;
}


void pidSetTarget(struct PID* pid, float target)
{
  pid->target = target;
}

void pidSetTarget_Measure(struct PID* pid, float target, float measured)
{
  pid->target = target;
  pid->current = measured;
  pid->merror = pid->target - measured;
}

void pidSetKp(struct PID* pid, float kp)
{
  pid->Kp = kp;
}

void pidSetKi(struct PID* pid, float ki)
{
  pid->Ki = ki;
}

void pidSetKd(struct PID* pid, float kd)
{
  pid->Kd = kd;
}

void pidSetMeasured(struct PID* pid, float measured)
{
  pid->current = measured;
}

void pidSetLowPassEnable(struct PID* pid)
{
	pid->Lowpass_EN = PID_TRUE;
}

void pidSet(struct PID* pid, float kp, float ki, float kd)
{
	pid->Kp = kp;
	pid->Ki = ki;
	pid->Kd = kd;
}

//------------------End of File----------------------------


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
