#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "action.h"
#include "protocol.h"
#include "commands.h"
#include "xml_parse.h"
#include "units_enums.h"

#define DEBUG 0
//#define USE_GTK

#ifdef USE_GTK
int use_gtk=1;
#else
int use_gtk=0;
#endif

/*
 *To compile this file using gcc you can type
 *gcc -Wall `xml2-config --libs` `xml2-config --cflags` -o xml_parse xml_parse.c 
 */

struct set_entry q;

int number_to_unit(struct get_entry_q q, struct get_entry_a a, char *unit)
{
  switch(q.id)
  {
  case 0x11:
    switch(a.value)
    {
    case MOTOR_CW_CCW: strcpy(unit,"it_direction_both"); break;
    case MOTOR_CW_ONLY: strcpy(unit,"it_direction_clockwise"); break;
    case MOTOR_CCW_ONLY: strcpy(unit,"it_direction_counter_clockwise"); break;
    }
    break;
  case 0x12:
    switch(a.unit)
    {
    case RAD_BY_S2: strcpy(unit,"rad/s²"); break;
    case HZ_BY_S: strcpy(unit,"Hz/s"); break;
    case RPM_BY_S: strcpy(unit,"RPM/s"); break;
    case RPM_BY_MIN: strcpy(unit,"RPM/min"); break;
    }
    break;
  case 0x13:
    switch(a.unit)
    {
    case MNM_BY_A: strcpy(unit,"mNm/A"); break;
    case NM_BY_A: strcpy(unit,"Nm/A"); break;
      //case MNM_BY_MA: strcpy(unit,"mNm/mA"); break;
    }
    break;
  case 0x14:
    switch(a.unit)
    {
    case OHM: strcpy(unit,"Ω");break;
    case MOHM: strcpy(unit,"mΩ"); break;
    case UOHM: strcpy(unit,"µΩ"); break;
    }
    break;
  case 0x15:
    switch(a.unit)
    {
    case MH: strcpy(unit,"mH");break;
    case UH: strcpy(unit,"µH"); break;
    case NH: strcpy(unit,"nHΩ"); break;
    }
    break;
  case 0x16:
  case 0x17:
  case 0x50:
    switch(a.unit)
    {
    case HZ: strcpy(unit,"Hz"); break;
    case RPM: strcpy(unit,"RPM"); break;
    }
    break;
  case 0x19:
  case 0x1A:
  case 0x1B:
  case 0x1C:
  case 0x5B:
    switch(a.unit)
    {
    case AMPERE: strcpy(unit,"A"); break;
    case MAMPERE: strcpy(unit,"mA"); break;
    }
    break;
  case 0x59:
  case 0x5A:
    switch(a.unit)
    {
    case 0: strcpy(unit,"V"); break;
    case -3: strcpy(unit,"mV"); break;
    }
    break;
  case 0x53:
  case 0x5C:
    switch(a.unit)
    {
    case 0: strcpy(unit,"%"); break;
    }
    break;
  case 0x56:
    switch(a.unit)
    {
    case 0: strcpy(unit,"℃"); break;
    }
    break;


  case 0x55:
    switch(a.unit)
    {
    case 0: strcpy(unit,"mV/s"); break; //zou -3 kunnen zij
    }
    break;
  case 0x47:
    switch(a.unit)
    {
    case 0: strcpy(unit,"mA/Hz"); break;  // zou -3 kunnen zijn
    }
    break;
  case 0x46:
    switch(a.unit)
    {
    case 0: strcpy(unit,"mA/Hz*s"); break; //zou -3 kunnen zijn
    }
    break;
  case 0x45:
    switch(a.unit)
    {
    case 0: strcpy(unit,"Hz/V"); break;
    }
    break;
  case 0x44:
    switch(a.unit)
    {
    case 0: strcpy(unit,"Hz/Vs"); break;
    }
    break;
  case 0x40:
  case 0x42:
    switch(a.unit)
    {
    case 0: strcpy(unit,"V/As"); break;
    }
    break;
  case 0x18:
  case 0x52:
  case 0x70:
  case 0x71:
    switch(a.unit)
    {
    case -9: strcpy(unit,"ns"); break;
    case -6: strcpy(unit,"µs"); break;
    case -3: strcpy(unit,"ms"); break;
    case 0: strcpy(unit,"s"); break;
    }
    break;
  case 0x41:
  case 0x43:
  case 0x72:
    switch(a.unit)
    {
    case 0: strcpy(unit,"V/A"); break;
    case MV_BY_A: strcpy(unit,"mV/A"); break;
    case UV_BY_A: strcpy(unit,"µV/mA"); break;
    }
    break;
  case 0x73:
    switch(a.unit)
    {
    case MV_BY_V: strcpy(unit,"mV/V"); break;
    }
    break;
  case 0x30:
    switch(a.value)
    {
    case ENCODER_AMS_AS5145H: strcpy(unit,"it_encoder_ams_as5145a"); break;
    case ENCODER_HALL_UV: strcpy(unit,"it_encoder_hall_uv"); break;
    case ENCODER_HALL_UVW: strcpy(unit,"it_encoder_hall_uvw"); break;
    case ENCODER_INCENC_AB: strcpy(unit,"it_encoder_incremental_ab"); break;
    case ENCODER_INCENC_ABZ: strcpy(unit,"it_encoder_incremental_abz"); break;
    case ENCODER_NO_ENCODER: strcpy(unit,"it_encoder_nong"); break;
    case ENCODER_SINGLE_PULSE: strcpy(unit,"it_encoder_single_pulse"); break;
    }
    break;
  case 0x31:
    switch(a.value)
    {
    case ENCODER_USAGE_EVENT: strcpy(unit,"it_encoder_usage_event"); break;
    case ENCODER_USAGE_NONE: strcpy(unit,"it_encoder_usage_none"); break;
    case ENCODER_USAGE_SPEED: strcpy(unit,"it_encoder_usage_speed"); break;
    }
    break;
  case 0x58:
    switch(a.value)
    {
    case EXTERNAL_SPEED_NONE: strcpy(unit,"it_external_speed_control_non"); break;
    case EXTERNAL_SPEED_PWM: strcpy(unit,"it_external_speed_control_pwm_duty"); break;
    case EXTERNAL_SPEED_SERVO_0_TO_MAX: strcpy(unit,"it_external_speed_control_servo_pulse_0_to_MAX"); break;
    case EXTERNAL_SPEED_SERVO_NEGMAX_TO_MAX: strcpy(unit,"it_external_speed_control_servo_pulse_NEGMAX_to_MAX"); break;
    case EXTERNAL_SPEED_ADC: strcpy(unit,"it_external_speed_control_voltage"); break;
    }
    break;
  case 0x75:
    switch(a.value)
    {
    case CURRENT_SENSOR_2: strcpy(unit,"it_measurement_type_2_sensor"); break;
    case CURRENT_SHUNT_3: strcpy(unit,"it_measurement_type_3_shunt"); break;
    case CURRENT_SHUNT_1: strcpy(unit,"it_measurement_type_single_shunt"); break;
    }
    break;
  case 0x76:
    switch(a.value)
    {
    case CURRENT_SENSOR_INVERTED: strcpy(unit,"it_sensor_direction_inverted"); break;
    case CURRENT_SENSOR_NORMAL: strcpy(unit,"it_sensor_direction_normal"); break;
    }
    break;
  case 0x77:
  case 0x78:
    switch(a.value)
    {
    case POLARISATION_HIGH: strcpy(unit,"it_polarity_active_high"); break;
    case POLARISATION_LOW: strcpy(unit,"it_polarity_active_low"); break;
    }
    break;
  case 0x54:
    switch(a.value)
    {
    case RESTART_MOTOR: strcpy(unit,"it_restart_mode_stall"); break;
    case SWITCH_OFF_MOTOR: strcpy(unit,"it_restart_mode_switch_off_stall"); break;
    }
    break;
  case 0x51:
    switch(a.value)
    {
    case SHUTDOWN_GENTLE: strcpy(unit,"it_shutdown_mode_gentle"); break;
    case SHUTDOWN_DISABLE_OUTPUT: strcpy(unit,"it_shutdown_mode_no_signals"); break;
    case SHUTDOWN_SHORT_BRAKE: strcpy(unit,"it_shutdown_mode_short_brake"); break;
    }
    break;
  default:
    strcpy(unit,"");
    break;
  }
  return 0;
}



  unsigned char unit_to_number(char *unit)
  {
    if(!strcmp(unit,"mNm/A")) return MNM_BY_A;
    if(!strcmp(unit,"Nm/A")) return NM_BY_A;
    if(!strcmp(unit,"mNm/mA")) return MNM_BY_MA;

    if(!strcmp(unit,"rad/s²")) return  RAD_BY_S2;
    if(!strcmp(unit,"Hz/s")) return HZ_BY_S;
    if(!strcmp(unit,"RPM/s")) return  RPM_BY_S;
    if(!strcmp(unit,"RPM/min")) return RPM_BY_MIN;

    if(!strcmp(unit,"Ω")) return OHM;
    if(!strcmp(unit,"mΩ")) return MOHM;
    if(!strcmp(unit,"µΩ")) return UOHM;

    if(!strcmp(unit,"mH")) return MH;
    if(!strcmp(unit,"µH")) return UH;
    if(!strcmp(unit,"nH")) return NH;

    if(!strcmp(unit,"Hz")) return HZ;
    if(!strcmp(unit,"RPM")) return RPM;

    if(!strcmp(unit,"A")) return AMPERE;
    if(!strcmp(unit,"mA")) return MAMPERE;

    if(!strcmp(unit,"V/As")) return 0;

    if(!strcmp(unit,"V/A")) return 0;

    if(!strcmp(unit,"Hz/Vs")) return 0;

    if(!strcmp(unit,"Hz/V")) return 0;

    if(!strcmp(unit,"mA/Hz*s")) return 0;

    if(!strcmp(unit,"mA/Hz")) return 0;
    if(!strcmp(unit,"mV/s")) return 0;

    if(!strcmp(unit,"ns")) return -9;
    if(!strcmp(unit,"µs")) return -6;
    if(!strcmp(unit,"ms")) return -3;
    if(!strcmp(unit,"s")) return 0;

    if(!strcmp(unit,"mV/A")) return MV_BY_A;
    if(!strcmp(unit,"µV/mA")) return UV_BY_A;

    if(!strcmp(unit,"mV/V")) return MV_BY_V;

    if(!strcmp(unit,"it_direction_both")) return MOTOR_CW_CCW;
    if(!strcmp(unit,"it_direction_clockwise")) return MOTOR_CW_ONLY;
    if(!strcmp(unit,"it_direction_counter_clockwise")) return  MOTOR_CCW_ONLY;

    if(!strcmp(unit,"it_encoder_ams_as5145a")) return ENCODER_AMS_AS5145H;
    if(!strcmp(unit,"it_encoder_hall_uv")) return ENCODER_HALL_UV;
    if(!strcmp(unit,"it_encoder_hall_uvw")) return ENCODER_HALL_UVW;
    if(!strcmp(unit,"it_encoder_incremental_ab")) return  ENCODER_INCENC_AB;
    if(!strcmp(unit,"it_encoder_incremental_abz")) return  ENCODER_INCENC_ABZ;
    if(!strcmp(unit,"it_encoder_none")) return ENCODER_NO_ENCODER;
    if(!strcmp(unit,"it_encoder_single_pulse")) return ENCODER_SINGLE_PULSE;

    if(!strcmp(unit,"it_encoder_usage_event")) return ENCODER_USAGE_EVENT;
    if(!strcmp(unit,"it_encoder_usage_none")) return ENCODER_USAGE_NONE;
    if(!strcmp(unit,"it_encoder_usage_speed")) return ENCODER_USAGE_SPEED;

    if(!strcmp(unit,"it_external_speed_control_none")) return EXTERNAL_SPEED_NONE;
    if(!strcmp(unit,"it_external_speed_control_pwm_duty")) return EXTERNAL_SPEED_PWM;  
    if(!strcmp(unit,"it_external_speed_control_servo_pulse_0_to_MAX")) return  EXTERNAL_SPEED_SERVO_0_TO_MAX;
    if(!strcmp(unit,"it_external_speed_control_servo_pulse_NEGMAX_to_MAX")) return EXTERNAL_SPEED_SERVO_NEGMAX_TO_MAX;
    if(!strcmp(unit,"it_external_speed_control_voltage")) return EXTERNAL_SPEED_ADC;

    if(!strcmp(unit,"it_measurement_type_2_sensor")) return  CURRENT_SENSOR_2;
    if(!strcmp(unit,"it_measurement_type_3_shunt")) return  CURRENT_SHUNT_3;
    if(!strcmp(unit,"it_measurement_type_single_shunt")) return CURRENT_SHUNT_1;

    if(!strcmp(unit,"it_polarity_active_high")) return POLARISATION_HIGH;
    if(!strcmp(unit,"it_polarity_active_low")) return POLARISATION_LOW;

    if(!strcmp(unit,"it_restart_mode_stall")) return RESTART_MOTOR;
    if(!strcmp(unit,"it_restart_mode_switch_off_stall")) return SWITCH_OFF_MOTOR;

    if(!strcmp(unit,"it_sensor_direction_inverted")) return CURRENT_SENSOR_INVERTED;
    if(!strcmp(unit,"it_sensor_direction_normal")) return CURRENT_SENSOR_NORMAL;

    if(!strcmp(unit,"it_shutdown_mode_gentle")) return SHUTDOWN_GENTLE;
    if(!strcmp(unit,"it_shutdown_mode_gentle_tooltip")) return 3;
    if(!strcmp(unit,"it_shutdown_mode_no_signals")) return SHUTDOWN_DISABLE_OUTPUT;
    if(!strcmp(unit,"it_shutdown_mode_short_brake")) return SHUTDOWN_SHORT_BRAKE;
    return 0;
  }

  void get_all_motor_parameters(int fd,unsigned char *cmd)
  {
    int i;
    int res;
    struct get_entry_q s_get_entry_q;
    struct get_entry_a s_get_entry_a;
    char *unit;

    unit=malloc(100*sizeof(char));    
//    for(i=0;i<sizeof(parameter_flags)/NR_PARAMETERS;i++)
    for(i=0;i<(sizeof(parameter_flags)/NR_PARAMETERS)-17;i++)
    {
      s_get_entry_q.id=parameter_flags[i][0];s_get_entry_q.offset=0;
      res=get_one_motor_parameter (fd ,cmd,parameter_flags[i][2],s_get_entry_q);
      if(DEBUG) print_result(cmd,res);

//      usleep(50000);

      if(cmd[1]==0xEE)
      {
	if(!use_gtk)printf("Parameter: 0x%.2X error\n",i);
      }
      else
      {
	s_get_entry_a.value=(cmd[2]<<0)|(cmd[3]<<8)|(cmd[4]<<16)|(cmd[5]<<24);
	s_get_entry_a.unit=cmd[6];
//      printf("Parameter Id: 0x%.2X, Value: 0x%.8X, Unit: 0x%.2X\n",s_get_entry_q.id,s_get_entry_a.value, s_get_entry_a.unit);


	number_to_unit(s_get_entry_q,s_get_entry_a, unit);
	if(strlen(unit))
	{
	  if(!strncmp(unit,"it_",3))
	  {
	    if(!use_gtk)printf("Parameter Id: 0x%.2X, Value: %s\n",s_get_entry_q.id,unit);
	  }
	  else
	  {
	    if(!use_gtk)printf("Parameter Id: 0x%.2X, Value: %i, Unit: %s\n",s_get_entry_q.id,s_get_entry_a.value, unit);
	  }
	}
	else
	{
	  if(!use_gtk)printf("Parameter Id: 0x%.2X, Value: %i\n",s_get_entry_q.id,s_get_entry_a.value);
	}
      }
    }
    free(unit);
  }


int get_elements(int fd,unsigned char * cmd,int motor,xmlNode *a_node,struct set_entry *sp_q)
  {
    static int node_counter=0;
    xmlNode *cur_node = NULL;
    xmlNode *next_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
    {

      if (cur_node->type == XML_ELEMENT_NODE) 
      {
	if(!strcmp((char*)cur_node->name,"Id"))
	{
	  if(node_counter>0) 
	  {
	    if(!use_gtk)printf("\n");
	    set_one_motor_parameter (fd ,cmd,motor,q);
//	    usleep(50000);
	  }
	  (sp_q+node_counter)->id=q.id;
	  (sp_q+node_counter)->unit=q.unit;
	  (sp_q+node_counter)->value=q.value;
	  node_counter++;

	  if(!use_gtk)printf("Parameter %s: ", cur_node->name);
	
	  if(cur_node->children) 
	  {
	    next_node=cur_node->children;
	    if(next_node->type==XML_TEXT_NODE)
	      if(next_node->content)
	      {

		if(!use_gtk)printf("0x%.2X, ",atoi((char*)next_node->content));
		q.id=atoi((char*)next_node->content);
	      }
	  }
	}
	if(!strcmp((char*)cur_node->name,"Value"))
	{
	  if(!use_gtk)printf("%s :", cur_node->name);
	  if(cur_node->children) 
	  {
	    next_node=cur_node->children;
	    if(next_node->type==XML_TEXT_NODE)
	      if(next_node->content)
	      {
		if(!use_gtk)printf("%s, ",next_node->content);
		if(strncmp((char*)next_node->content,"it_",3))
		  q.value=atoi((char*)next_node->content);
		else
		{
		  q.value=unit_to_number((char*)next_node->content);
		  q.unit=0;
		}
	      }
	  
	  }
	}
	if(!strcmp((char*)cur_node->name,"Unit"))
	{
	  if(!use_gtk)printf("%s :", cur_node->name);
	  if(cur_node->children) 
	  {
	    next_node=cur_node->children;
	    if(next_node->type==XML_TEXT_NODE)
	      if(next_node->content)
	      {
		if(!use_gtk)printf("%s, ",next_node->content); //de text string moet omgezet worden naar het unit nummer
		q.unit=unit_to_number((char*)next_node->content);
	      }
	  }
	}
      }
      get_elements(fd,cmd,motor,cur_node->children,sp_q);
    }
    return node_counter;
  }


int xml_parse(int fd,unsigned char *cmd, int motor,struct set_entry *sp_q)
  {
    int nr_elements;
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile("parameters.xml", NULL, 0);

    if (doc == NULL) printf("error: could not parse file\n");


    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    nr_elements=get_elements(fd,cmd,motor,root_element,sp_q);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return nr_elements;
  }
