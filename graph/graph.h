#ifndef _miiskira_graph_h_
#define _miiskira_graph_h_

#include <refer.h>

typedef void (*miiskira_graph_present_f)(refer_t pri, const uint32_t *restrict pixel_bgra, uint32_t width, uint32_t height);

extern uint32_t miiskira$type$graph_present_create;
extern uint32_t miiskira$type$graph_present_destroy;
extern uint32_t miiskira$type$graph_present_resize;
extern uint32_t miiskira$type$graph_present_do;

void miiskira_graph_present_create_bgra(uint64_t address, refer_string_t name, miiskira_graph_present_f present_func, refer_t pri);
void miiskira_graph_present_destroy(uint64_t address, refer_string_t name);
void miiskira_graph_present_resize(uint64_t address, refer_string_t name, uint32_t width, uint32_t height);
void miiskira_graph_present_do(uint64_t address, refer_string_t name, uint64_t report_address);

#endif
