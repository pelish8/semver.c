//
// semver.c
//
// Copyright (c) 2015 Tomas Aparicio
// MIT licensed
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semver.h"
#include "version.h"

#define NUMBERS "0123456789"
#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"
#define DELIMITER "."
#define SLICE_SIZE 50

semver_t *
semver_new (int major, int minor, int patch) {
  semver_t * ver = { major, minor, patch };
  return ver;
}

int
semver_parse (const char *str, semver_t *ver) {
  char major_s[SLICE_SIZE] = "0";
  char minor_s[SLICE_SIZE] = "0";
  char patch_s[SLICE_SIZE] = "0";
  char tail_s[SLICE_SIZE]  = "0";

  char * slice = strtok(str, DELIMITER);

  int count = 0;
  while (slice != NULL) {
    count++;

    size_t len = strlen(slice);
    if (len > SLICE_SIZE) return -1;

    switch (count) {
      case 1: strcpy(major_s, slice); break;
      case 2: strcpy(minor_s, slice); break;
      case 3: strcpy(patch_s, slice); break;
      case 4: strcpy(tail_s, slice); break;
    }

    if (count == 3) break;
    slice = strtok(NULL, DELIMITER);
  }

  //printf("Major: %s\n", major_s);
  //printf("Minor: %s\n", minor_s);
  //printf("Patch: %s\n", patch_s);

  int major = semver_parse_int(major_s);
  if (major == -1) return major;
  ver->major = major;

  int minor = semver_parse_int(minor_s);
  if (minor == -1) return minor;
  ver->minor = minor;

  int patch = semver_parse_int(patch_s);
  if (patch == -1) return patch;
  ver->patch = patch;

  return 0;
}

/**
 * Compare two semantic versions (x, y).
 *
 * Returns:
 * - `1` if x is higher than y
 * - `0` if x is equal to y
 * - `-1` if x is lower than y
 */

int
semver_compare (const semver_t x, const semver_t y) {
  if (x.major != y.major) {
    if (x.major > y.major) {
      return 1;
    }
    return -1;
  }

  if (x.minor != y.minor) {
    if (x.minor > y.minor) {
      return 1;
    }
    return -1;
  }

  if (x.patch != y.patch) {
    if (x.patch > y.patch) {
      return 1;
    }
    return -1;
  }

  return 0;
}

/**
 * Helpers
 */

int
semver_parse_int (const char *s) {
  int invalid = semver_valid_chars(s, NUMBERS);
  if (invalid) return -1;
  return strtol(s, NULL, 10);
}

int
semver_valid_chars (const char *s, const char *c) {
  size_t clen = strlen(c);
  size_t slen = strlen(s);

  for (int i = 0; i < slen; i++) {
    char v = s[i];
    int match = -1;

    for (int x = 0; x < clen; x++) {
      char y = c[x];
      if (y == v) {
        match = 0;
        break;
      }
    }

    if (match) return -1;
  }

  return 0;
}