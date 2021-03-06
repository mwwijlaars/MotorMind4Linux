#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "protocol.h"
#include "action.h"
#include <stdint.h>
#include "crc8.h"

#define BOT 0x11
#define EOT 0x13

#define DEBUG 0
#define usdelay() usleep(6000)  //dit zou anders moeten kunnen!

void print_result(unsigned char *cmd, int res)
{
  int i;
  for(i=0;i<res;i++) 
  {
    printf("0x%.2X ",cmd[i]);
    if(cmd[i]==0x13)break;
  }
  printf("\n");
}

int start_motor (int fd,unsigned char *cmd,int motor)
{
  cmd[0]=BOT;
  cmd[1]=RPC_StartMotor;
  cmd[2]=motor;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct StartMotor_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct StartMotor_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct StartMotor_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct StartMotor_a)+4);    
}

int stop_motor (int fd,unsigned char *cmd,int motor)
{
  cmd[0]=BOT;
  cmd[1]=RPC_StopMotor;
  cmd[2]=motor;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct StopMotor_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct StopMotor_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct StopMotor_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct StopMotor_a)+4);    
}


int store_parameters (int fd,unsigned char *cmd,int motor)
{
  cmd[0]=BOT;
  cmd[1]=RPC_StoreParameters;
  cmd[2]=motor;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct StoreParameters_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct StoreParameters_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct StoreParameters_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct StoreParameters_a)+4);    
}

int clear_parameters (int fd,unsigned char *cmd)
{
  cmd[0]=BOT;
  cmd[1]=RPC_ClearParameters;
  cmd[2]=RPC_Dummy;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct ClearParameters_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct ClearParameters_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct ClearParameters_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct ClearParameters_a)+4);    
}

int do_turn (int fd,unsigned char *cmd,int motor,int turns, unsigned int max_speed)
{
  cmd[0]=BOT;
  cmd[1]=RPC_DoTurn;
  cmd[2]=motor;
  cmd[3]=(turns>>0)&0xFF;
  cmd[4]=(turns>>8)&0xFF;
  cmd[5]=(max_speed>>0)&0xFF;
  cmd[6]=(max_speed>>8)&0xFF;
  cmd[7]=crc_update(cmd[1],&cmd[2],sizeof(struct DoTurn_q));
  cmd[8]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct DoTurn_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct DoTurn_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct DoTurn_a)+4);    
}



int get_motor_state (int fd,unsigned char *cmd,int motor)
{
  cmd[0]=BOT;
  cmd[1]=RPC_GetMotorState;
  cmd[2]=motor;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct GetMotorState_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct GetMotorState_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct GetMotorState_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct GetMotorState_a)+4);    
}



int get_firmware_version (int fd,unsigned char *cmd)
{
  cmd[0]=BOT;
  cmd[1]=RPC_GetFWVersion;
  cmd[2]=RPC_Dummy;
  cmd[3]=crc_update(cmd[1],&cmd[2],sizeof(struct GetFWVersion_q));
  cmd[4]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct GetFWVersion_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct GetFWVersion_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct GetFWVersion_a)+4);    
}


int get_one_motor_parameter (int fd ,unsigned char *cmd, int motor,struct get_entry_q q)
{
  cmd[0]=BOT;
  cmd[1]=RPC_GetOneMotorParameter;
  cmd[2]=motor;
  cmd[3]=q.id;
  cmd[4]=q.offset;
  cmd[5]=crc_update(cmd[1],&cmd[2],sizeof(struct GetOneMotorParameter_q));
  cmd[6]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct GetOneMotorParameter_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct GetOneMotorParameter_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct GetOneMotorParameter_a)+4);    
}

int set_one_motor_parameter (int fd ,unsigned char *cmd, int motor,struct set_entry q)
{
  cmd[0]=BOT;
  cmd[1]=RPC_SetOneMotorParameter;
  cmd[2]=motor;
  cmd[3]=q.id;
  cmd[4]=q.unit;
  cmd[5]=(q.value>>0)&0xFF;
  cmd[6]=(q.value>>8)&0xFF;
  cmd[7]=(q.value>>16)&0xFF;
  cmd[8]=(q.value>>24)&0xFF;
  cmd[9]=crc_update(cmd[1],&cmd[2],sizeof(struct SetOneMotorParameter_q));
  cmd[10]=EOT;

  if(DEBUG) print_result(cmd,sizeof(struct SetOneMotorParameter_q)+4 );
  usdelay();
  write(fd,cmd,sizeof(struct SetOneMotorParameter_q)+4);
  usdelay();
  return read(fd,cmd,sizeof(struct SetOneMotorParameter_a)+4);    
}

void firmware_version(unsigned char *cmd)
{
  int major=cmd[2];
  int minor=cmd[3];
  printf("MotorControlFW version %i.%i\n",major,minor);
}

void capabilities(unsigned char *cmd)
{
  struct get_entry_a a;
  a.value=cmd[2]<<0|cmd[3]<<8|cmd[4]<<16|cmd[5]<<24;
  a.unit=cmd[6];
//  printf("Value: 0x%.8X, unit: 0x%.2X\n",a.value,a.unit);

  if(a.value&FEATURE_HAS_DSO) printf("Has DSO.\n");
  if(a.value&FEATURE_HAS_LOAD_STATISTIC) printf("Has load statistic.\n");
  if(a.value&FEATURE_HAS_TURN_CONTROL) printf("Has turn control.\n");
  if(a.value&FEATURE_HAS_TEMPERATURE_CONTROL) printf("Has temperature control.\n");
  if(a.value&FEATURE_HAS_HSDSO) printf("Has high speed DSO.\n");
  if(a.value&FEATURE_HAS_RW_BOARD_SETTINGS) printf("Has read/write board settings.\n");
  if(a.value&FEATURE_HAS_EXTERNAL_SPEED_CONTROL) printf("Has external speed control.\n");
  if(a.value&FEATURE_HAS_SW_OVER_UNDER_VOLTAGE_DETECTION) printf("Has Software over/under voltage detection .\n");
  if(a.value&FEATURE_HAS_MOTOR_DISCONNECT_DETECTION) printf("Has motor disconnect detection .\n");
  if(a.value&FEATURE_HAS_SW_OVERCURRENT_DETECTION) printf("Has software overcurrect detection .\n");
  if(a.value&FEATURE_HAS_LOAD_DEPENDANT_SPEED_REDUCTION) printf("Has load dependant speed reduction.\n");
  if(a.value&FEATURE_HAS_CAN) printf("Has Can bus.\n");
  if(a.value&FEATURE_HAS_STALL_DETECT) printf("Has stall detect.\n");
  if(a.value&FEATURE_HAS_GET_DC_LINK_VOLTAGE) printf("Has get dc link voltage.\n");
  if(a.value&FEATURE_HAS_GET_MOTOR_STAGE) printf("Has get motor stage.\n");
}

void motor_state(unsigned char *cmd)
{
  int32_t ActualSpeed;
  int32_t TargetSpeed;
  int32_t Current;
  int32_t Torque;
  int32_t Timestamp;

  ActualSpeed=cmd[2]<<0|cmd[3]<<8|cmd[4]<<16|cmd[5]<<24;
  TargetSpeed=cmd[6]<<0|cmd[7]<<8|cmd[8]<<16|cmd[9]<<24;
  Current=cmd[10]<<0|cmd[11]<<8|cmd[12]<<16|cmd[13]<<24;
  Torque=cmd[14]<<0|cmd[15]<<8|cmd[16]<<16|cmd[17]<<24;
  Timestamp=cmd[18]<<0|cmd[19]<<8|cmd[20]<<16|cmd[21]<<24;

  printf("Target speed: %i, Actual speed: %i\n",TargetSpeed,ActualSpeed);
  printf("current: 0x%.8X, torque: 0x%.8X\n",Current,Torque);
  printf("Timestamp: %.3f\n",Timestamp/1000.0);
}

int encoder_counts(unsigned char *cmd)
{
  int32_t ticks;

  ticks=cmd[2]<<0|cmd[3]<<8|cmd[4]<<16|cmd[5]<<24;

//  printf("timer ticks between encoder counts: %i\n",ticks);
  return ticks;
}

void DC_voltage(unsigned char *cmd)
{
int adc=cmd[2]<<0|cmd[3]<<8|cmd[4]<<16|cmd[5]<<24;
  printf("DC voltage: %i, \n",adc);
}
