#include <libxml/parser.h>
#include <libxml/tree.h>
#define NR_PARAMETERS 3

static const char * const parameter_name[]=
{
  "MPC_PolePairs",
  "MPC_Direction",
  "MPC_MaxAngAcc",
  "MPC_TorqueFactor",
  "MPC_Resistance",
  "MPC_Inductance",
  "MPC_SpeedLimit",
  "MPC_SpeedChange",
  "MPC_PositionDelay",
  "MPC_IqStart",
  "MPC_IdStart",
  "MPC_IqLimit",
  "MPC_IdLimit",
  "MPC_MotorId",
  
  "MPC_Encoder",
  "MPC_EncoderUsage",
  "MPC_EncoderResolution",
  
  "MPC_PiIdKi",
  "MPC_PiIdKp",
  "MPC_PiIqKi",
  "MPC_PiIqKp",
  "MPC_PositionKi",
  "MPC_PositionKp",
  "MPC_SpeedKi",
  "MPC_SpeedKp",

  "MPC_PwmFrequency",
  "MPC_ShutdownMode",
  "MPC_Braketime",
  "MPC_Brakepercentage",
  "MPC_RestartMode",
  "MPC_StalldetectValue",
  "MPC_Overtemperature",
  "MPC_CanId",
  "MPC_ExternalSpeedControl",
  "MPC_SwOvervoltage",
  "MPC_SwUndervoltage",
  "MPC_SwOvercurrent",
  "MPC_SpeedReduction",

  "MPC_Deadtime",
  "MPC_BootstrapDelay",
  "MPC_SensitivityCurrentMeasure",
  "MPC_SensitivityVoltageMeasure",
  "MPC_GainCurrentMeasure",
  "MPC_MeasurementType",
  "MPC_SensorDirection",
  "MPC_PolL",
  "MPC_PolH",

  "MPC_FirmwareFeatures",
  "MPC_BoardRevision",
  "MPC_Channels",
  "MPC_DsoSize",
  "MPC_ParameterTransferCount",
  "MPC_BoardName",
  "MPC_BoardNamePWR0",
  "MPC_BoardNamePWR1",
 
/* system settings */
  "MSS_FirmwareVersion",
  "MSS_ResetStatus",
  "MSS_SystemLoad",
 
/* motor settings */
  "MMS_Temperature",
  "MMS_ErrorState",
  "MMS_DcLinkVoltage",
  "MMS_MotorStage",
  "MMS_TurnNumber",
  "MMS_Speed"
};

static char parameter_flags[][NR_PARAMETERS]=
{
/* Id,                             RW,MOTOR_ID */
  {MPC_PolePairs                   ,1,1},
  {MPC_Direction                   ,1,1},
  {MPC_MaxAngAcc                   ,1,1},
  {MPC_TorqueFactor                ,1,1},
  {MPC_Resistance                  ,1,1},
  {MPC_Inductance                  ,1,1},
  {MPC_SpeedLimit                  ,1,1},
  {MPC_SpeedChange                 ,1,1},
  {MPC_PositionDelay               ,1,1},
  {MPC_IqStart                     ,1,1},
  {MPC_IdStart                     ,1,1},
  {MPC_IqLimit                     ,1,1},
  {MPC_IdLimit                     ,1,1},
  {MPC_MotorId                     ,1,1},
  
  {MPC_Encoder                     ,1,1},
  {MPC_EncoderUsage                ,1,1},
  {MPC_EncoderResolution           ,1,1},
  
  {MPC_PiIdKi                      ,1,1},
  {MPC_PiIdKp                      ,1,1},
  {MPC_PiIqKi                      ,1,1},
  {MPC_PiIqKp                      ,1,1},
  {MPC_PositionKi                  ,1,1},
  {MPC_PositionKp                  ,1,1},
  {MPC_SpeedKi                     ,1,1},
  {MPC_SpeedKp                     ,1,1},

  {MPC_PwmFrequency                ,1,1},
  {MPC_ShutdownMode                ,1,1},
  {MPC_Braketime                   ,1,1},
  {MPC_Brakepercentage             ,1,1},
  {MPC_RestartMode                 ,1,1},
  {MPC_StalldetectValue            ,1,1},
  {MPC_Overtemperature             ,1,1},
  {MPC_CanId                       ,1,1},
  {MPC_ExternalSpeedControl        ,1,1},
  {MPC_SwOvervoltage               ,1,1},
  {MPC_SwUndervoltage              ,1,1},
  {MPC_SwOvercurrent               ,1,1},
  {MPC_SpeedReduction              ,1,1},

  {MPC_Deadtime                    ,1,1},
  {MPC_BootstrapDelay              ,1,1},
  {MPC_SensitivityCurrentMeasure   ,1,1},
  {MPC_SensitivityVoltageMeasure   ,1,1},
  {MPC_GainCurrentMeasure          ,1,1},
  {MPC_MeasurementType             ,1,1},
  {MPC_SensorDirection             ,1,1},
  {MPC_PolL                        ,1,1},
  {MPC_PolH                        ,1,1},

  {MPC_FirmwareFeatures            ,0,-1},
  {MPC_BoardRevision               ,0,-1},
  {MPC_Channels                    ,0,-1},
  {MPC_DsoSize                     ,0,-1},
  {MPC_ParameterTransferCount      ,0,-1},
  {MPC_BoardName                   ,0,-1},
  {MPC_BoardNamePWR0               ,0,-1},
  {MPC_BoardNamePWR1               ,0,-1},
 
/* system settings */
  {MSS_FirmwareVersion             ,0,-1},
  {MSS_ResetStatus                 ,0,-1},
  {MSS_SystemLoad                  ,0,-1},
 
/* motor settings */
  {MMS_Temperature                 ,0,-1},
  {MMS_ErrorState                  ,0,1},
  {MMS_DcLinkVoltage               ,0,1},
  {MMS_MotorStage                  ,0,1},
  {MMS_TurnNumber                  ,0,1},
  {MMS_Speed                      ,-1,1}

};

int number_to_unit(struct get_entry_q, struct get_entry_a, char *);
int get_elements(int ,unsigned char*,int,xmlNode *,struct set_entry *);
int xml_parse(int,unsigned char*,int,struct set_entry *);
void get_all_motor_parameters(int,unsigned char*);

