#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// check for argument, picking default if needed
// check each home dir for .birthday file
    // collect matches
// print matches


char valid_date(char *date) {
  int converted, month, day;
  converted = sscanf(date, "%2d/%2d", &month, &day);
  if (converted != 2) {
    return 0;
  }
  if (month < 1 || month > 12) {
    return 0;
  }
  if (day < 1 || day > 31) {
    return 0;
  }
  return 1;
}


int main(int argc, char *argv[]) {
  char *month_day;
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

  puts(month_day);

  return 0;
}
