#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const int MAX_PATH_LENGTH = 40;
const int MAX_MONTHDAY_LENGTH = 5;

const char DEBUG = 0;

struct monthday {
  int month;
  int day;
};


char parse_date(char *date, struct monthday * md) {
  /* Given a date string and a pointer to a monthday struct, attempts to parse
     month and day values out of the string and store them in the struct. If
     able to parse the string, 1 is returned; 0 otherwise. */
  int converted, month, day;
  char extra[1];
  if (strlen(date) > MAX_MONTHDAY_LENGTH) return 0;
  converted = sscanf(date, "%2d/%2d%1c", &month, &day, extra);
  if (converted != 2) return 0;
  if (month < 1 || month > 12) return 0;
  if (day < 1 || day > 31) return 0;
  md->month = month;
  md->day = day;
  return 1;
}

void process_args(const int argc, char *argv[],
                  struct monthday * md) {
  /* Processes argc and argv. Currently exits with the appropriate error code if
     arguments are invalid. Populates the referenced monthday struct with
     today's month/day values (in UTC) or parses the given month/day and
     populates the struct with that.
   */
  // TODO consider returning error info and dispatching error messages by code.
  time_t now;
  struct tm *utc;
  if (argc == 1) {
    time(&now);
    utc = gmtime(&now);
    md->month = utc->tm_mon + 1; // tm_mon is 0 indexed :(
    md->day = utc->tm_mday;
  }
  else if (argc == 2) {
    if (!parse_date(argv[1], md)) {
      fprintf(stderr, "bad date: %s. use format month/day\n", argv[1]);
      exit(1);
    }
  }
  else {
    fprintf(stderr, "bad number of args passed: %d\n", argc);
    exit(2);
  }
}

int homedir_selector(const struct dirent * directory) {
  /* Given a pointer to a dirent struct, return 1 if it contains a .birthday
     file we can read and 0 otherwise.
   */
  char path[MAX_PATH_LENGTH];
  sprintf(path, "/home/%s/.birthday", directory->d_name);
  if (DEBUG) fprintf(stderr, "%s\n", path);
  return access(path, R_OK) != -1;
}

int main(int argc, char *argv[]) {
  struct monthday md;

  process_args(argc, argv, &md);
  if (DEBUG) fprintf(stderr, "%d/%d\n", md.month, md.day);

  int num_directories;
  struct dirent **namelist;
  num_directories = scandir("/home", &namelist, homedir_selector, alphasort);
  if (DEBUG) fprintf(stderr, "found %d directories\n", num_directories);

  if (num_directories == 0) return 0;

  int directory_ix = 0;
  char file_content[MAX_MONTHDAY_LENGTH + 1];
  char path[MAX_PATH_LENGTH];
  FILE *file;
  char c;
  int content_ix;
  struct monthday parsed_monthday;

  while (directory_ix < num_directories) {
    sprintf(path, "/home/%s/.birthday", namelist[directory_ix]->d_name);
    if (DEBUG) fprintf(stderr, "checking %s\n", path);
    file = fopen(path, "r");
    if (file) {
      content_ix = 0;
      c = getc(file);
      while (c != EOF
             && c != '\n'
             && content_ix < MAX_MONTHDAY_LENGTH) {
        file_content[content_ix] = c;
        content_ix++;
        c = getc(file);
      }

      file_content[content_ix] = '\0';

      if (parse_date(file_content, &parsed_monthday)) {
        if (DEBUG) fprintf(stderr, "comparing against %d/%d\n", parsed_monthday.month, parsed_monthday.day);

        if (parsed_monthday.month == md.month && parsed_monthday.day == md.day) {
          fprintf(stdout, "%s\n", namelist[directory_ix]->d_name);
        }
      }
      else if (DEBUG) {
        fprintf(stderr, "unable to parse file\n");
      }
      fclose(file);
    }
    directory_ix++;
  }

  return 0;
}
