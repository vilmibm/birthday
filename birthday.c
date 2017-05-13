#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// check each home dir for .birthday file
    // collect matches
// print matches

const char DEBUG = 0;


char valid_date(char *date) {
  int converted, month, day;
  char extra[1];
  if (strlen(date) > 5) return 0;
  converted = sscanf(date, "%2d/%2d%1c", &month, &day, extra);
  if (converted != 2) return 0;
  if (month < 1 || month > 12) return 0;
  if (day < 1 || day > 31) return 0;
  return 1;
}

void process_args(const int argc, char *argv[], char *month_day) {
  // TODO consider returning error info and dispatching error messages by code.
  time_t now;
  struct tm *utc;
  if (argc == 1) {
    time(&now);
    utc = gmtime(&now);
    sprintf(month_day, "%d/%d", utc->tm_mon + 1, utc->tm_mday);
  }
  else if (argc == 2) {
    if (!valid_date(argv[1])) {
      fprintf(stderr, "bad date: %s. use format month/day\n", argv[1]);
      exit(1);
    }
    strncpy(month_day, argv[1], 5);
  }
  else {
    fprintf(stderr, "bad number of args passed: %d\n", argc);
    exit(2);
  }
}

int homedir_selector(const struct dirent * directory) {
  return 1;
}

int main(int argc, char *argv[]) {
  char month_day[5];
  int num_directories;
  struct dirent **namelist;

  process_args(argc, argv, month_day);
  if (DEBUG) {
    puts(month_day);
  }

  num_directories = scandir("/home", &namelist, homedir_selector, alphasort);
  fprintf(stderr, "%d\n", num_directories);

  return 0;
}
