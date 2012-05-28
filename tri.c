#include <tcutil.h>
#include <string.h>
#include <inttypes.h>

#define PAST_SIZE 8
#define BUFFER_SIZE 512

int
main(int argc, char* argv[])
{
  TCMAP *correlated = tcmapnew();
  TCMAP *argument = tcmapnew();
  uint32_t past[PAST_SIZE];
  uint32_t header[PAST_SIZE];
  char buffer[BUFFER_SIZE];
  char* b;
  size_t len, current = 0;
  char dec_c;
  uint16_t sum;
  uint32_t dec_n, nb;
  uint64_t corr;
  const uint64_t *const_corr;
  const uint32_t *tmp, *tmp2;
  int null_sp, i;
  int use_header = 1;

  const char encode[] = "bcj";

  memset(past, 0, PAST_SIZE * sizeof(uint32_t));

  b = fgets(buffer, BUFFER_SIZE - 1, stdin);
  while(b != NULL) {
    len = strlen(b);
    if (len > 1) {
      dec_c = *b;
      if (dec_c != '-') {
        switch(dec_c) {
          case 'b':
            nb = 1 << 0;
            break;
          case 'c':
            nb = 1 << 8;
            break;
          case 'j':
            nb = 1 << 16;
            break;
          case 'n':
            nb = 0;
            break;
          default:
            printf("Bad input : '%c'\n", dec_c);
            return -1;
        }
        if (sscanf((b + 1), "%"SCNu32, &dec_n) != 1) {
          printf("bad input\n");
          return -1;
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
    b = fgets(buffer, BUFFER_SIZE - 1, stdin);
  }

  tcmapiterinit(argument);
  while ((tmp = tcmapiternext(argument, &null_sp)) != NULL) {
    tmp2 = tcmapiterval(tmp, &null_sp);
    sum = 0;
    sum += *(((const uint8_t*)tmp2) + 0);
    sum += *(((const uint8_t*)tmp2) + 1);
    sum += *(((const uint8_t*)tmp2) + 2);
    printf("s %"PRIu16" %"PRIu32"\n", sum, *tmp);
    for (i = 0; i < 3; ++i) {
      if (*(((const uint8_t*)tmp2) + i)) {
        printf("s.%c %"PRIu8" b%"PRIu32"\n", encode[i], *(((const uint8_t*)tmp2) + i), *tmp);
      }
    }
  }
  tcmapiterinit(correlated);
  while ((const_corr = tcmapiternext(correlated, &null_sp)) != NULL) {
    tmp2 = tcmapiterval(const_corr, &null_sp);
    printf("c %"PRIu32" %"PRIu32"-%"PRIu32"\n", *tmp2, *((const uint32_t*)const_corr), *((const uint32_t*)const_corr + 1));
  }
  return 0;
}
