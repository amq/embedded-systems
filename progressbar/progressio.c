#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef min
  #define min(x,y) (((x) < (y)) ? (x) : (y))
#endif

#define MAX_PROG_STEP_SIZE 10
#define MAX_COLS 80

static int init = 0;

#ifdef _WIN32

#include <windows.h>

void init_io(void) {
  system("cls");
  srand(time(NULL));
  init = 1;
}

CHAR GetCh(VOID) {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD irInputRecord;
  DWORD dwEventsRead;
  CHAR cChar;

  while(ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead)) /* Read key press */
    if (irInputRecord.EventType == KEY_EVENT) {
      cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
      ReadConsoleInputA(hStdin, &irInputRecord , 1, &dwEventsRead); /* Read key release */
      return cChar;
    }
  return EOF;
}

void shutdown_io(void) {
  // NOP
}

#else

#include <unistd.h>
#include <termios.h>
#define GetCh getchar

static struct termios old_termios_settings;

void init_io(void) {
  struct termios new_termios_settings;

  tcgetattr(0, &old_termios_settings);
  new_termios_settings = old_termios_settings;

  /* Disable canonical mode and echoing, and set buffer size to 1 byte */
  new_termios_settings.c_lflag &= (~ICANON & ~ECHO);
  new_termios_settings.c_cc[VTIME] = 0;
  new_termios_settings.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, &new_termios_settings);

  setvbuf(stdin, NULL, _IONBF, 0);
  system("clear");
  srand(time(NULL));
  init = 1;
}

void shutdown_io(void) {
  tcsetattr(0, TCSANOW, &old_termios_settings);
}

#endif

unsigned int get_progress(void) {
  static unsigned int progs = 0;
  unsigned int rand_val;
  unsigned int i;
  if (progs == 0xFFFFFFFF)
    return progs;

  rand_val = rand() % MAX_PROG_STEP_SIZE;
  i = rand() % 4;
  while (((progs >> (i * 8)) & 0xFF) == 0xFF) {
    i = (i + 1) % 4;
  }
  if (((progs >> (i * 8)) & 0xFF) + rand_val < 0xFF)
    progs += rand_val << (i * 8);
  else
    progs |= 0xFF << (i * 8);
  return progs;
}

void progressbar(char done_symbol, char active_symbol, char todo_symbol, unsigned int percent) {
  unsigned int w = MAX_COLS - 2;
  unsigned int limit = percent*w/100;
  unsigned int i;

  if (init == 0)
    exit(42);

  printf("\r[");
  for (i = 0; i < w; i++) {
    putchar(i < limit ? done_symbol : (i == limit ? active_symbol : todo_symbol));
  }
  printf("]\r");
  fflush(stdout);
}

void wait_key(void) {
  if (init == 0)
    exit(42);
  (void)GetCh();
}
