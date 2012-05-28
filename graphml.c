#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include "graphml.h"

const char *colors[] = {
  "#C0F000",
  "#C0F0F0",
  "#C000F0",
  "#00FF00",
  "#00FFFF",
  "#0000FF",
  0
};

inline void
printGlobalHeader() {
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  printf("<graphml");
  printf(" xmlns=\"http://graphml.graphdrawing.org/xmlns\"");
  printf(" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");
  printf(" xmlns:y=\"http://www.yworks.com/xml/graphml\"");
  printf(" xmlns:yed=\"http://www.yworks.com/xml/yed/3\"");
  printf(" xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://www.yworks.com/xml/schema/graphml/1.1/ygraphml.xsd\"");
  printf(">\n");
  printf("  <!--Created by yFiles for Java 2.9-->\n");
  printf("  <key for=\"graphml\" id=\"d0\" yfiles.type=\"resources\"/>\n");
  printf("  <key for=\"port\" id=\"d1\" yfiles.type=\"portgraphics\"/>\n");
  printf("  <key for=\"port\" id=\"d2\" yfiles.type=\"portgeometry\"/>\n");
  printf("  <key for=\"port\" id=\"d3\" yfiles.type=\"portuserdata\"/>\n");
  printf("  <key attr.name=\"url\" attr.type=\"string\" for=\"node\" id=\"d4\"/>\n");
  printf("  <key attr.name=\"description\" attr.type=\"string\" for=\"node\" id=\"d5\"/>\n");
  printf("  <key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>\n");
  printf("  <key attr.name=\"Description\" attr.type=\"string\" for=\"graph\" id=\"d7\"/>\n");
  printf("  <key attr.name=\"url\" attr.type=\"string\" for=\"edge\" id=\"d8\"/>\n");
  printf("  <key attr.name=\"description\" attr.type=\"string\" for=\"edge\" id=\"d9\"/>\n");
  printf("  <key for=\"edge\" id=\"d10\" yfiles.type=\"edgegraphics\"/>\n");
}

inline void
printGlobalFooter() {
  printf("</graphml>\n");
}

inline void
printGraphHeader() {
  printf("  <graph edgedefault=\"directed\" id=\"G\">\n");
}

inline void
printGraphFooter() {
  printf("  </graph>\n");
}

void
printNode(const int n, const uint32_t radius, const char* label) {
  printf("    <node id=\"n%i\">\n", n);
  printf("      <data key=\"d5\"/>\n");
  printf("      <data key=\"d6\">\n");
  printf("        <y:SVGNode>\n");
  printf("          <y:Geometry height=\"%i\" width=\"%i\" x=\"%i\" y=\"%i\"/>\n", 2 * radius, 2 * radius, n * radius, 0);
  printf("          <y:Fill color=\"#CCCCFF\" transparent=\"false\"/>\n");
  printf("          <y:BorderStyle color=\"#000000\" type=\"line\" width=\"1.0\"/>\n");
  printf("          <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"12\"");
  printf(" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" hasText=\"false\" height=\"4.0\"");
  printf(" modelName=\"sandwich\" modelPosition=\"s\" textColor=\"#000000\" visible=\"true\" width=\"4.0\" x=\"%i\" y=\"%i\"/>\n", (n + 1) * radius, 0);
  if (label != NULL) {
    printf("          <y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"%i\"", (int)(radius / 3.2));
    printf(" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" height=\"4.0\"");
    printf(" modelName=\"custom\" textColor=\"#000000\" visible=\"true\" width=\"4.0\" x=\"%f\" y=\"%i\">%s<y:LabelModel>\n", (((double)n) + 0.5) * radius, 0, label);
    printf("              <y:SmartNodeLabelModel distance=\"4.0\"/>\n");
    printf("            </y:LabelModel>\n");
    printf("            <y:ModelParameter>\n");
    printf("              <y:SmartNodeLabelModelParameter labelRatioX=\"0.0\" labelRatioY=\"0.0\" nodeRatioX=\"0.0\" nodeRatioY=\"0.0\" offsetX=\"0.0\" offsetY=\"0.0\" upX=\"0.0\" upY=\"-1.0\"/>\n");
    printf("            </y:ModelParameter>\n");
    printf("          </y:NodeLabel>\n");
  }
  printf("          <y:SVGNodeProperties usingVisualBounds=\"true\"/>\n");
  printf("          <y:SVGModel svgBoundsPolicy=\"0\">\n");
  printf("            <y:SVGContent refid=\"%i\"/>\n", n + 1);
  printf("          </y:SVGModel>\n");
  printf("        </y:SVGNode>\n");
  printf("      </data>\n");
  printf("    </node>\n");
}

void
printEdge(const int n, const int i, const int j, const uint32_t size, const char* color) {
  printf("    <edge id=\"e%i\" source=\"n%i\" target=\"n%i\">\n", n, i, j);
  printf("      <data key=\"d9\"/>\n");
  printf("      <data key=\"d10\">\n");
  printf("        <y:SplineEdge>\n");
  printf("          <y:Path sx=\"0.0\" sy=\"0.0\" tx=\"0.0\" ty=\"0.0\"/>\n");
  printf("          <y:LineStyle color=\"%s\" type=\"line\" width=\"%f\"/>\n", color, (double)size);
  printf("          <y:Arrows source=\"none\" target=\"none\"/>\n");
  printf("        </y:SplineEdge>\n");
  printf("      </data>\n");
  printf("    </edge>\n");
}

inline void
printRessourceHeader() {
  printf("  <data key=\"d0\">\n");
  printf("    <y:Resources>\n");
}

inline void
printRessourceFooter() {
  printf("    </y:Resources>\n");
  printf("  </data>\n");
}

void
printPieGraph(const int n, const uint32_t radius, const uint8_t size[]) {
  int i;
  uint16_t total;
  long x, y, lastX, lastY;
  double tmp, seg;

  printf("      <y:Resource id=\"%i\">&lt;!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n", n + 1);
  printf("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"&gt;\n");
  printf("&lt;svg width=\"20cm\" height=\"20cm\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"&gt;\n");

  lastX = radius;
  lastY = 0;
  seg = 0;
  total = 0;
  for (i = 0; i < PIE_SIZE; ++i) {
    total += size[i];
  }

  for (i = 0; i < PIE_SIZE; ++i) {
    if (size[i] != 0) {
      tmp = ((double)size[i])/((double)total) * 2 * M_PI;
      seg += tmp;
      x = (int) (cos(seg) * radius);
      y = (int) (sin(seg) * radius);
      if (tmp > M_PI) {
        printf("&lt;path d=\"M 0,0 l %li,%li a %"PRIu32",%"PRIu32" 0 1,0 %li,%li z\" fill=\"%s\" stroke=\"none\" stroke-width=\"2\" stroke-linejoin=\"round\" /&gt;\n", \
                lastX, -lastY, radius, radius, x - lastX, lastY - y, colors[i]);
      } else {
        printf("&lt;path d=\"M 0,0 l %li,%li a %"PRIu32",%"PRIu32" 0 0,0 %li,%li z\" fill=\"%s\" stroke=\"none\" stroke-width=\"2\" stroke-linejoin=\"round\" /&gt;\n", \
                lastX, -lastY, radius, radius, x - lastX, lastY - y, colors[i]);
      }
      lastX = x;
      lastY = y;
    }
  }
  printf("&lt;/svg&gt;\n");
  printf("</y:Resource>\n");
}
