#ifndef GRAPHML_H
#define GRAPHML_H

#include <inttypes.h>

#define PIE_SIZE 6

inline void printGlobalHeader();
inline void printGlobalFooter();
inline void printGraphHeader();
inline void printGraphFooter();
void printNode(const int n, const uint64_t size, const char* label);
void printEdge(const int n, const int i, const int j, const uint32_t size, const char* color);
inline void printRessourceHeader();
inline void printRessourceFooter();
uint64_t printPieGraph(const int n, const uint8_t size[]);

#endif /* GRAPHML_H */
