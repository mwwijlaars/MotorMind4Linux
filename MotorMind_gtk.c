#include "action.h"
#include "protocol.h"
#include "commands.h"
#include "xml_parse.h"
#include <fcntl.h>
#include <termios.h>
#include <strings.h>
#include <gtk/gtk.h>

#define BAUDRATE B115200 //115200,8N1
#define DEVICE "/dev/ttyUSB0"

#define BOT 0x11
#define EOT 0x13
void start_cb(GtkWidget *button)
{
  printf("start button\n");
}

int main(int argc,char *argv[])
{
  int fd=0;
  int i,j;
  int size_a=MAX_SIZE_A;
  int size_q=MAX_SIZE_Q;
  int max_size;
  unsigned char *cmd_buf;
  struct termios oldtio,newtio;
  GtkWidget *window;
  GtkWidget *grid[2];
  GtkWidget *text_view;
  GtkTextBuffer *buffer;
  GtkWidget *scrolled_window;
  GtkWidget *button;
  GtkWidget *combo;

  int nr_entries;
  struct set_entry *sp_q;

  struct get_entry_q s_get_entry_q;
  struct get_entry_a s_get_entry_a;
  char *unit;
  char s[100];

  unit=malloc(100*sizeof(char));   
  if(size_q>size_a)max_size=size_q;else max_size=size_a;
  cmd_buf=malloc(max_size);
  sp_q=malloc(0xFF*sizeof(struct set_entry));

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Linux MotorMind");
//  gtk_window_set_default_size (GTK_WINDOW(window),200,600);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 8);

  scrolled_window=gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scrolled_window,500,300);
  gtk_container_add(GTK_CONTAINER(window), scrolled_window);
  gtk_widget_show(scrolled_window);

  /* Create new grid and place grid into container */
  grid[0]=gtk_grid_new();
  gtk_container_add (GTK_CONTAINER (scrolled_window), grid[0]);


  
#if 1
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
#endif
  nr_entries=xml_parse(fd,cmd_buf,1,sp_q);
  for(i=1;i<nr_entries;i++)
  {
    text_view = gtk_text_view_new ();
    gtk_grid_attach(GTK_GRID(grid[0]),text_view,0,i-1,1,1);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    s_get_entry_q.id=(sp_q+i)->id;
    s_get_entry_a.unit=(sp_q+i)->unit;
    number_to_unit(s_get_entry_q,s_get_entry_a, unit);

    for(j=0;j<((sizeof(parameter_flags)/NR_PARAMETERS)-17);j++)
    {
      if((sp_q+i)->id == parameter_flags[j][0]) break;
    }
    sprintf(s,"%s: %i %s",parameter_name[j],(sp_q+i)->value, unit); // moet beter
    gtk_text_buffer_set_text (buffer, s, -1);
  }

//  combo = gtk_combo_box_text_new_with_entry ();
// in table met text entry en Id label.
// also see Tree View shopping list in gtk3-demo
  combo = gtk_combo_box_text_new ();
gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "never", "Not visible");
gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "when-active", "Visible when active");
gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (combo), "always", "Always visible");
  gtk_grid_attach(GTK_GRID(grid[0]),combo,1,0,1,2);

//  gtk_container_add (GTK_CONTAINER (box), combo);


 button= gtk_button_new_with_label("Start Motor");
 g_signal_connect (button,"clicked", G_CALLBACK(start_cb), NULL);
 gtk_grid_attach(GTK_GRID(grid[0]),button,1,2,1,2);
 
 


  gtk_widget_show_all (window);
  gtk_main ();
#if 1
  tcsetattr(fd,TCSANOW,&oldtio);

  close(fd);
  free(sp_q);
  free(unit);
#endif
  return 0;
}
