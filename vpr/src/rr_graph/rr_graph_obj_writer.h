/*
 * This function writes the RR_graph generated by VPR into a file in XML format
 * Information included in the file includes rr nodes, rr switches, the grid, block info, node indices
 */

#ifndef RR_GRAPH_OBJ_WRITER_H
#define RR_GRAPH_OBJ_WRITER_H


void write_rr_graph_obj_to_xml(const char *file_name, const RRGraph* rr_graph);

#endif
