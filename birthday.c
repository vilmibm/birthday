#include <stdlib.h>
#include <stdio.h>

// check for argument, picking default if needed
// check each home dir for .birthday file
    // collect matches
// print matches

// TODO how to get today's month and day
// TODO char point literal

char valid_date(char *date) {
  int converted, month, day;
  // TODO probably fail with single digits
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
  // TODO why no work
  fprintf(stdout, "%d %d\n", month, day);
  return 1;
}


void main(int argc, char *argv[]) {
  char *month_day;
  if (argc == 1) {
    puts("using today");
  }
  else if (argc == 2) {
    if (!valid_date(argv[1])) {
      fprintf(stderr, "bad date: %s. use format month/day\n", argv[1]);
      exit(2);
    }
  }
  else {
    fprintf(stderr, "bad number of args passed: %d\n", argc);
    exit(1);
  }
}
