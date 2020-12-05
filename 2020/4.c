#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aoc.h"

static const char *fields[8] =
    { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid" };

static int isvalid(entry *k, entry *v)
{
    int d, i;
    long ld;
    unsigned int u;
    char s[SNBUF], *key, *val;

    s[0] = '\0';
    key = k->f.s;
    val = v->f.s;
    if (strncmp(key, "byr", 3) == 0) {
        if (sscanf(val, "%d%n", &d, &i) == EOF || i != 4 || d < 1920
            || d > 2002)
            return 0;
    } else if (strncmp(key, "iyr", 3) == 0) {
        if (sscanf(val, "%d%n", &d, &i) == EOF || i != 4 || d < 2010
            || d > 2020)
            return 0;
    } else if (strncmp(key, "eyr", 3) == 0) {
        if (sscanf(val, "%d%n", &d, &i) == EOF || i != 4 || d < 2020
            || d > 2030)
            return 0;
    } else if (strncmp(key, "hgt", 3) == 0) {
        if (sscanf(val, "%d%s", &d, s) == EOF
            || !((strcmp(s, "cm") == 0 && d >= 150 && d <= 193)
                 || (strcmp(s, "in") == 0 && d >= 59 && d <= 76)))
            return 0;
    } else if (strncmp(key, "hcl", 3) == 0) {
        if (sscanf(val, "#%x%n", &u, &i) == EOF || i != 7 || u > 0xFFFFFF)
            return 0;
    } else if (strncmp(key, "ecl", 3) == 0) {
        if (sscanf(val, "%s%n", s, &i) == EOF || i > 3
            || strncmp(s, "amb", 3)
            * strncmp(s, "blu", 3)
            * strncmp(s, "brn", 3)
            * strncmp(s, "gry", 3)
            * strncmp(s, "grn", 3)
            * strncmp(s, "hzl", 3)
            * strncmp(s, "oth", 3) != 0)
            return 0;
    } else if (strncmp(key, "pid", 3) == 0) {
        if (sscanf(val, "%ld%n", &ld, &i) == EOF || i != 9)
            return 0;
    }
    return 1;
}

static void parse(char *str, int *pr, int *va)
{
    char key[4], val[SNBUF], s[SNBUF + 4];
    int i;
    entry *k, *v;

    hinit();
    key[3] = '\0';
    i = 0;
    do {
        /* key-value pairs are delimited by spaces or newlines */
        if (*str == ' ' || *str == '\n') {
            s[i] = '\0';
            if (sscanf(s, "%3c:%s", key, val) != EOF) {
                k = newstr(key);
                v = newstr(val);
                hinsert(k, v);
            }
            i = 0;
        } else {
            s[i] = *str;
            i++;
        }
        str++;
    } while (i < SNBUF + 4 && *str != '\0');

    /* field[7] is optional (cid) */
    *pr = *va = 1;
    for (i = 0; i < 7; i++) {
        k = newstr(fields[i]);
        v = hlookup(k);
        *pr &= (v != NULL);
        *va &= v ? isvalid(k, v) : 0;
        free(k);
    }
    hfree(free);
}

int main(void)
{
    int i, c, p, na, nb, x, y;
    char passport[1024];

    na = nb = p = i = 0;
    do {
        c = getchar();
        /* passport entries are delimited by blank lines */
        if ((c == '\n' && p == '\n') || c == EOF) {
            passport[i] = '\0';
            parse(passport, &x, &y);
            na += x;
            nb += y;
            i = 0;
        } else {
            passport[i] = c;
            i++;
        }
        p = c;
    } while (i < 1023 && c != EOF);

    printf("A) %d passports with all fields present\n", na);
    printf("B) %d passports with all fields present and valid\n", nb);

    return 0;
}