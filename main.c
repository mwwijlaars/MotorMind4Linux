#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <strings.h>
#include "crc8.h"
#include "action.h"
#include "protocol.h"
#include "commands.h"
#include "xml_parse.h"

#define BAUDRATE B115200 //115200,8N1
#define DEVICE "/dev/ttyUSB0"
#define NSEC_PER_SEC    1000000000L

#define BOT 0x11
#define EOT 0x13

int inputAvailable(void)  
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds));
}
/* timespec stuff to use ns time resolution */
void set_normalized_timespec(struct timespec *ts, time_t sec,long long nsec)
{
  while (nsec >= NSEC_PER_SEC)
  {
    /*
    * The following asm() prevents the compiler from
    * optimising this loop into a modulo operation. See
    * also __iter_div_u64_rem() in include/linux/time.h
    */

    asm("" : "+rm"(nsec));
    nsec -= NSEC_PER_SEC;
    ++sec;
  }

  while (nsec < 0)
  {
    asm("" : "+rm"(nsec));
    nsec += NSEC_PER_SEC;
    --sec;
  }
  ts->tv_sec = sec;
  ts->tv_nsec = nsec;
}

/* add normalised time and return a struct timespec */
struct timespec timespec_sub(struct timespec lhs,struct timespec rhs)
{
  struct timespec ts_delta;
  set_normalized_timespec(&ts_delta, lhs.tv_sec - rhs.tv_sec,lhs.tv_nsec - rhs.tv_nsec);
  return ts_delta;
}

/* substract normalised time and return a struct timespec */
struct timespec timespec_add(struct timespec lhs, struct timespec rhs)
{
  struct timespec ts_delta;
  set_normalized_timespec(&ts_delta, lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
  return ts_delta;
}

int main (int argc, char *argv[])
{
  int fd=0;
  int res;
  int i=0;
  int us_sleep=100000;
  unsigned char  motor=1;
  int count=1,start_count=0,old_count=1;
  struct get_entry_q s_get_entry_q;
  struct get_entry_a s_set_entry_a;
  struct set_entry   s_set_entry_q;

  int size_a=MAX_SIZE_A;
  int size_q=MAX_SIZE_Q;
  int max_size;
  unsigned char *cmd_buf;
  struct termios oldtio,newtio;
  struct set_entry *sp_q;
  struct timespec ts[3];
  long long int dt[2];
  sp_q=malloc(0xFF*sizeof(struct set_entry));

  if(argc<2)
  {
    printf("Usage: motormind <speed> [turns]\n");
    return -1;
  }
  if(size_q>size_a)max_size=size_q;else max_size=size_a;
  cmd_buf=malloc(max_size);

  if((fd=open(DEVICE,O_RDWR|O_NOCTTY))==-1)
  {
    printf("Device: %s error\n",DEVICE); 
    return -1;
  }

  tcgetattr(fd,&oldtio); /* save current port settings */
        
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

  newtio.c_iflag &= ~IGNBRK; 
  newtio.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
  newtio.c_iflag |= IGNPAR|ICRNL;

  newtio.c_oflag = 0;
   
  newtio.c_lflag = 0;
  newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
  newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
  newtio.c_cc[VERASE]   = 0;     /* del */
  newtio.c_cc[VKILL]    = 0;     /* @ */
  newtio.c_cc[VEOF]     = 0;  /* Ctrl-d */
  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
  newtio.c_cc[VSWTC]    = 0;     /* '\0' */
  newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
  newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
  newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
  newtio.c_cc[VEOL]     = 0;     /* '\0' */
  newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
  newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
  newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
  newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
  newtio.c_cc[VEOL2]    = 0;     /* '\0' */         



  tcflush(fd, TCIOFLUSH);         
  tcsetattr(fd,TCSANOW,&newtio); 

  res=get_firmware_version(fd,cmd_buf);
  firmware_version(cmd_buf);

  s_get_entry_q.id=MPC_FirmwareFeatures;s_get_entry_q.offset=0;
  res=get_one_motor_parameter (fd ,cmd_buf,-1,s_get_entry_q);
  capabilities(cmd_buf);


  xml_parse(fd,cmd_buf,1,sp_q); //moet filename toevoegen

  printf("\n\n");
  get_all_motor_parameters(fd,cmd_buf);

  s_get_entry_q.id=MPC_Channels;s_get_entry_q.offset=0;
  res=get_one_motor_parameter (fd ,cmd_buf,-1,s_get_entry_q);
  print_result(cmd_buf,res);


  s_set_entry_q.id=MMS_Speed; s_set_entry_q.unit=0; s_set_entry_q.value=atoi(argv[1]);
  res=set_one_motor_parameter (fd ,cmd_buf,motor,s_set_entry_q);
  print_result(cmd_buf,res);
  if(argc>2)
  {
    s_get_entry_q.id=MMS_TurnNumber;s_get_entry_q.offset=0;
    res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
    start_count=encoder_counts(cmd_buf);

    res=do_turn (fd ,cmd_buf,motor,atoi(argv[2]),s_set_entry_q.value);
    printf("press any key.\n");
    us_sleep=(999999/(atoi(argv[1])/60));

//CLOCK_PROCESS_CPUTIME_ID

    clock_gettime(CLOCK_MONOTONIC,&ts[0]);
    while(!inputAvailable())
    {
      usleep(us_sleep);
      s_get_entry_q.id=MMS_TurnNumber;s_get_entry_q.offset=0;
//            printf("count: %i\n",count);
      res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
//            printf("count: %i\n",count);
      old_count=count;


      count=encoder_counts(cmd_buf);

//      printf("count: %i\n",count);
      clock_gettime(CLOCK_MONOTONIC,&ts[1]);
      dt[0]=dt[1];
      dt[1]=ts[1].tv_sec*1000000000+ts[1].tv_nsec;
      dt[1]-=ts[0].tv_sec*1000000000+ts[0].tv_nsec;

//      ts[1]=timespec_sub(ts[1],ts[0]);
      printf("dt: %li\n",dt[1]-dt[0]);
      printf("speed: %f rpm, counts: %i\n",(float)60.0*(count-old_count)/((dt[1]-dt[0])/1000000000.0)/2000,count-start_count);
//      printf("abs count-start_count: %i\n",abs(count-start_count));

      if(abs(count-start_count)>=(1950*atoi(argv[2])))break;

    } 
  }
  else
  {
    res=start_motor (fd ,cmd_buf,motor); 
    print_result(cmd_buf,res);
    usleep(100000);
    printf("press any key.\n");
    while(!inputAvailable()) 
    {

      res=get_motor_state (fd,cmd_buf,motor);
      motor_state(cmd_buf);
      usleep(us_sleep);
      
      
//    s_get_entry_q.id=MMS_MotorStage;s_get_entry_q.offset=0;
//    res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
//    print_result(cmd_buf,res);
      
      s_get_entry_q.id=MMS_TurnNumber;s_get_entry_q.offset=0;
      res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
      old_count=count;
      count=encoder_counts(cmd_buf);
      printf("speed: %f rpm, ",(float)60.0*((count-old_count)/2000.0)/(us_sleep/1000000.0));
      
//    s_get_entry_q.id=MMS_DcLinkVoltage;s_get_entry_q.offset=0;
//    res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
//    DC_voltage(cmd_buf);
      
      
/*
  s_get_entry_q.id=MMS_ErrorState;s_get_entry_q.offset=0;
  res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
  print_result(cmd_buf,res);
*/
      if(i<10) i++; else break;
    }
    res=stop_motor (fd ,cmd_buf,motor);
    print_result(cmd_buf,res);
  }


//s_set_entry_q.id=MPC_EncoderUsage; s_set_entry_q.unit=0; s_set_entry_q.value=1;
//res=set_one_motor_parameter (fd ,cmd_buf,motor,s_set_entry_q);
//print_result(cmd_buf,res);

//  s_get_entry_q.id=MPC_EncoderUsage;s_get_entry_q.offset=0;
//  res=get_one_motor_parameter (fd ,cmd_buf,motor,s_get_entry_q);
//  print_result(cmd_buf,res);


//  res=get_motor_state (fd ,cmd_buf,motor);
//  print_result(cmd_buf,res);

//  res=do_turn (fd ,cmd_buf,motor,500,1000);
//  print_result(cmd_buf,res);


//  res=clear_parameters (fd ,cmd_buf);
//  print_result(cmd_buf,res);



//  res=start_motor (fd ,cmd_buf,motor);
//  print_result(cmd_buf,res);

//  res=stop_motor (fd ,cmd_buf,motor);
//  print_result(cmd_buf,res);


//  usleep (100000);
//  res=store_parameters (fd ,cmd_buf,motor);
//  print_result(cmd_buf,res);




  tcsetattr(fd,TCSANOW,&oldtio);

  close(fd);
  return 0;
}
