#define _USE_MATH_DEFINES
#include <math.h>
#include <tcutil.h>
#include <string.h>
#include <inttypes.h>
#include "graphml.h"

#define PAST_SIZE 8
#define BUFFER_SIZE 512

static void
extract(TCMAP *correlated, TCMAP *argument, FILE* in) {

  uint32_t past[PAST_SIZE];
  uint32_t header[PAST_SIZE];
  char buffer[BUFFER_SIZE];
  char* b;
  size_t len, current = 0;
  char dec_c;
  uint32_t dec_n, nb;
  uint64_t corr;
  const uint32_t *tmp;
  int null_sp, i;
  int use_header = 1;

  memset(past, 0, PAST_SIZE * sizeof(uint32_t));

  b = fgets(buffer, BUFFER_SIZE - 1, in);
  while(b != NULL) {
    len = strlen(b);
    if (len > 1) {
      dec_c = *b;
      if (dec_c != '-') {
        switch(dec_c) {
          case 'j':
            nb = 1 << 0;
            break;
          case 'b':
            nb = 1 << 8;
            break;
          case 'c':
            nb = 1 << 16;
            break;
          case 'n':
            nb = 0;
            break;
          default:
            printf("Bad input : '%c'\n", dec_c);
            exit(-1);
        }
        if (sscanf((b + 1), "%"SCNu32, &dec_n) != 1) {
          printf("bad input\n");
          exit(-1);
        }
        tmp = tcmapget(argument, &dec_n, sizeof(uint32_t), &null_sp);
        if (tmp != NULL) {
          nb += *tmp;
        }
        tcmapput(argument, &dec_n, sizeof(uint32_t), &nb, sizeof(uint32_t));
        for (i = 0; i < PAST_SIZE; ++i) {
          if ((past[i] != 0) && (past[i] != dec_n)) {
            if (past[i] > dec_n) {
              corr = dec_n + (((uint64_t)past[i]) << 32);
            } else {
              corr = (((uint64_t)dec_n) << 32) + past[i];
            }
            tmp = tcmapget(correlated, &corr, sizeof(uint64_t), &null_sp);
            if (tmp == NULL) {
              nb = 1;
            } else {
              nb = (*tmp) + 1;
            }
            tcmapput(correlated, &corr, sizeof(uint64_t), &nb, sizeof(uint32_t));
          }
        }
        if (use_header) {
          for (i = 0; i < PAST_SIZE; ++i) {
            if ((header[i] != 0) && (header[i] != dec_n)) {
              if (header[i] > dec_n) {
                corr = dec_n + (((uint64_t)header[i]) << 32);
              } else {
                corr = (((uint64_t)dec_n) << 32) + header[i];
              }
              tmp = tcmapget(correlated, &corr, sizeof(uint64_t), &null_sp);
              if (tmp == NULL) {
                nb = 1;
              } else {
                nb = (*tmp) + 1;
              }
              tcmapput(correlated, &corr, sizeof(uint64_t), &nb, sizeof(uint32_t));
            }
          }
        }
        if ((dec_c == 'b') || (dec_c == 'j')) {
          header[current] = dec_n;
        } else {
          for (i = 0; i < PAST_SIZE; ++i) {
            if (past[i] == dec_n) {
              past[i] = 0;
            }
          }
          past[current] = dec_n;
        }
        ++current;
        current %= PAST_SIZE;
      } else {
        memset(past, 0, PAST_SIZE * sizeof(uint32_t));
        current = 0;
        if (*(b+1) == '-') {
          memset(header, 0, PAST_SIZE * sizeof(uint32_t));
          use_header = 1;
        } else if (*(b+1) == 'r'){
          use_header = 1;
        } else {
          use_header = 0;
        }
      }
    }
    b = fgets(buffer, BUFFER_SIZE - 1, in);
  }
}

const char *arguments[] = {
  0,
  "Pas nouveau",
  "Expliciter",
  "Erreur ciblage",
  "Insultes",
  "Sournois",
  "S'échaper",
  "? S'échaper ?",
  "Contrôle",
  "Dangereux",
  "!Alternative",
  "Disparition lien",
  "Egocentricisme",
  "Solutions",
  "Mieux qu'avant",
  "DELETED 1",
  "Cutiver diversité",
  "Pas assez",
  "Évasion facile",
  "CNIL",
  "Problème publicité",
  "Promotion",
  "J'ai remarqué",
  "Libre arbitre",
  "Déviance interne",
  "DELETED 2",
  "DELETED 3",
  "J'ai testé",
  "Transparence",
  "Publicité",
  "Potientiel",
  0
};

#define ARG_MAX 31

int
main(int argc, char* argv[])
{
  FILE *f, *s;
  int i, null_sp;
  const uint32_t *arg_f, *arg_s, *tmp;
  const uint64_t *const_corr;
  uint8_t args[PIE_SIZE];
  uint64_t size[ARG_MAX];

  if (argc != 3) {
    printf("Need 2 arguments\n");
  }
  f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("openning file 1 (fopen)");
    exit(-5);
  }
  s = fopen(argv[2], "r");
  if (s == NULL) {
    perror("openning file 1 (fopen)");
    fclose(s);
    exit(-5);
  }

  TCMAP *correlated_f = tcmapnew();
  TCMAP *argument_f = tcmapnew();
  TCMAP *correlated_s = tcmapnew();
  TCMAP *argument_s = tcmapnew();
  extract(correlated_f, argument_f, f);
  extract(correlated_s, argument_s, s);

  fclose(f);
  fclose(s);

  printGlobalHeader();
  printRessourceHeader();
  memset(size, 0, ARG_MAX * sizeof(uint32_t));
  for(i = 1; i < ARG_MAX; ++i) {
    arg_f = tcmapget(argument_f, &i, sizeof(uint32_t), &null_sp);
    arg_s = tcmapget(argument_s, &i, sizeof(uint32_t), &null_sp);
    if (arg_f == NULL) {
      memset(args, 0, 3);
    } else {
      args[0] = 4 * (*arg_f) & 0xff;
      args[1] = 2 *((*arg_f) & 0xff00) >> 8;
      args[2] = ((*arg_f) & 0xff0000) >> 16;
    }
    if (arg_s == NULL) {
      memset(args + 3, 0, 3);
    } else {
      args[3] = 4 * (*arg_s) & 0xff;
      args[4] = 2 * ((*arg_s) & 0xff00) >> 8;
      args[5] = ((*arg_s) & 0xff0000) >> 16;
    }
    size[i] = printPieGraph(i, args);
  }
  printRessourceFooter();

  printGraphHeader();
  for(i = 1; i < ARG_MAX; ++i) {
    printNode(i, size[i], arguments[i]);
  }

  i = 0;
  tcmapiterinit(correlated_f);
  while ((const_corr = tcmapiternext(correlated_f, &null_sp)) != NULL) {
    tmp = tcmapiterval(const_corr, &null_sp);
    if (*tmp > 1)
      printEdge(++i, (int) *((const uint32_t*)const_corr), (int) *((const uint32_t*)const_corr + 1), *tmp, "#999999");
  }
  tcmapiterinit(correlated_s);
  while ((const_corr = tcmapiternext(correlated_s, &null_sp)) != NULL) {
    tmp = tcmapiterval(const_corr, &null_sp);
    if (*tmp > 1)
      printEdge(++i, (int) *((const uint32_t*)const_corr), (int) *((const uint32_t*)const_corr + 1),  *tmp, "#800000");
  }
  printGraphFooter();
  printGlobalFooter();

  return 0;
}
