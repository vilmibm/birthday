/* This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <dirent.h>
#include <err.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const int MAX_MONTHDAY_LENGTH = 5;

const bool DEBUG = false;

void dwarnx(const char * fmt, ...) {
  if (!DEBUG) return;
  va_list argp;
  va_start(argp, fmt);
  vwarnx(fmt, argp);
  va_end(argp);
}

int homedir_selector(const struct dirent * directory) {
  /* Given a pointer to a dirent struct, return 1 if it contains a .birthday
     file we can read and 0 otherwise.
   */
  char path[PATH_MAX];
  sprintf(path, "/home/%s/.birthday", directory->d_name);
  dwarnx("checking for %s", path);
  return access(path, R_OK) != -1;
}

int main(int argc, char *argv[]) {
  time_t now;
  struct tm date;
  struct tm *date_to_match;
  if (argc == 1) {
    time(&now);
    date_to_match = gmtime(&now);
  }
  else if (argc == 2) {
    if (strptime(argv[1], "%m/%d", &date) == NULL) {
      fprintf(stderr, "bad date: %s. use format month/day\n", argv[1]);
      exit(1);
    }
    date_to_match = &date;
  }
  else {
    fprintf(stderr, "bad number of args passed: %d\n", argc);
    exit(2);
  }
  dwarnx("using date %d/%d", date_to_match->tm_mon, date_to_match->tm_mday);

  struct dirent **namelist;
  int num_dirs = scandir("/home", &namelist, homedir_selector, alphasort);
  dwarnx("found %d birthday files", num_dirs);

  if (num_dirs == 0) return 0;

  char path[PATH_MAX];
  FILE *file;
  char line[LINE_MAX];
  struct tm file_date;

  while (num_dirs--) {
    sprintf(path, "/home/%s/.birthday", namelist[num_dirs]->d_name);
    dwarnx("checking %s", path);

    file = fopen(path, "r");
    if (!file) return false;

    if (!fgets(line, LINE_MAX, file)) continue;
    fclose(file);

    if (strlen(line) < 3) continue;
    if (strlen(line) > MAX_MONTHDAY_LENGTH + 1) continue; // allow for newline
    if (line[strlen(line)-1] == '\n') {
      line[strlen(line)-1] = '\0';
    }

    if (strptime(line, "%m/%d", &file_date) != NULL) {
      dwarnx("comparing against %d/%d", file_date.tm_mon, file_date.tm_mday);

      if (file_date.tm_mon == date_to_match->tm_mon
          && file_date.tm_mday == date_to_match->tm_mday) {
        fprintf(stdout, "%s\n", namelist[num_dirs]->d_name);
      }
    }
    else {
      dwarnx("unable to parse %s", path);
    }

    free(namelist[num_dirs]);
  }

  free(namelist);
  return 0;
}
