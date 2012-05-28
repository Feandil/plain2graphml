#ifndef GRAPHML_H
#define GRAPHML_H

#include <inttypes.h>

inline void printGlobalHeader();
inline void printGlobalFooter();
inline void printGraphHeader();
inline void printGraphFooter();
void printNode(const int n, const uint32_t radius, const char* label);
inline void printRessourceHeader();
inline void printRessourceFooter();
void printPieGraph(const int n, const uint32_t radius, const uint8_t size[]);

#endif /* GRAPHML_H */