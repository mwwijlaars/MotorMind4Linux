unsigned char get_firmware_version(int,unsigned char *);
unsigned char get_one_motor_parameter(int,unsigned char *, int,struct get_entry_q);

void print_result(unsigned char *, int);
int start_motor (int,unsigned char *,int);
int stop_motor (int,unsigned char *,int);
int store_parameters (int,unsigned char *,int);
int clear_parameters (int,unsigned char *);
int do_turn (int,unsigned char *,int,int, unsigned int);

int get_motor_state (int,unsigned char *,int);
int set_one_motor_parameter (int,unsigned char *, int,struct set_entry);

void firmware_version(unsigned char *);
void capabilities(unsigned char*);
void motor_state(unsigned char *);
void DC_voltage(unsigned char *);
int encoder_counts(unsigned char *);
