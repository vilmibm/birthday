#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
  char path[40];
  sprintf(path, "/home/%s/.birthday", directory->d_name);
  if (DEBUG) fprintf(stderr, "%s\n", path);
  return access(path, R_OK) != -1;
}

int main(int argc, char *argv[]) {
  char month_day[5];
  int num_directories;
  int directory_ix = 0;
  struct dirent **namelist;

  process_args(argc, argv, month_day);
  if (DEBUG) fprintf(stderr, "%s\n", month_day);

  num_directories = scandir("/home", &namelist, homedir_selector, alphasort);
  if (DEBUG) fprintf(stderr, "found %d directories\n", num_directories);

  if (num_directories == 0) return 0;

  char path[40];
  FILE *file;
  char c;
  int monthday_ix;
  while (directory_ix < num_directories) {
    sprintf(path, "/home/%s/.birthday", namelist[directory_ix]->d_name);
    if (DEBUG) fprintf(stderr, "checking %s\n", path);
    file = fopen(path, "r");
    if (file) {
      monthday_ix = 0;
      while ((c = getc(file)) != EOF) {
        if (c == month_day[monthday_ix]) {
          monthday_ix++;
        }
        else {
          break;
        }
      }
      if (monthday_ix == strlen(month_day)) {
        fprintf(stdout, "%s\n", namelist[directory_ix]->d_name);
      }
      fclose(file);
    }
    directory_ix++;
  }

  return 0;
}
