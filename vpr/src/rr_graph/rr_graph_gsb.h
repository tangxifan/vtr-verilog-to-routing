/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    rr_graph_gsb.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/07/31  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file constains a class to categorize a RRGraph to
 *  General Switch Blocks (GSBs), which is a unified block of Connection Blocks 
 *  and Switch Blocks.
 *  Each GSB contains
 *  1. A switch block
 *  2. A X-direction Connection block locates at the left side of the switch block
 *  2. A Y-direction Connection block locates at the top side of the switch block   
 *
 *  This data structure includes a fast look-up for all the nodes in a RRGraph based on GSB locations and the node location on GSB perimeters.
 *  To be memory efficient, the only few data stored will be the fast look-up, 
 *  while fruitful data accessors are provided for users to get the useful information 
 *  (mainly get nodes) they want from each GSB 
 *  
 *                                +---------------------------------+
 *                     IPIN_NODES |          Y-direction CB         | IPIN_NODES
 *                                |              [x][y + 1]         |
 *                                +---------------------------------+
 *                   
 *      IPIN_NODES                              TOP SIDE
 *   +-------------+              +---------------------------------+
 *   |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODES           OPIN_NODES |
 *   | X-direction |              |                                 |
 *   |      CB     |  LEFT SIDE   |        Switch Block             |  RIGHT SIDE
 *   |   [x][y]    |              |              [x][y]             |
 *   |             |              |                                 |
 *   |             |              | CHAN_NODES           CHAN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODES           OPIN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *   +-------------+              +---------------------------------+
 *      IPIN_NODES                            BOTTOM SIDE
 *
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef RR_GRAPH_GSB_H
#define RR_GRAPH_GSB_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>

/* External library header files */
#include "vtr_vector.h"
#include "vtr_range.h"

#include "vpr_types.h"
#include "device_coordinator.h"
#include "rr_graph_obj.h"

/* Define invalide node ids to ease code writing */
#define GSB_OPEN_NODE_ID (size_t)(-1)

/***********************************************************************
 *  This data structure focuses on modeling the internal pin-to-pin connections.
 *  It is basically a collection of nodes and edges.
 *  To make the data structure general, the nodes and edges are not linked to any another data 
 *  structures.
 *
 *  node_lookup: a 5D fast look-up [x][y][node_side][node_type][node_index] to place RRNodeIds in GSBs
 *  [x][y]: each GSB has a unique coordinator 
 *  [node_side]: each GSB has 4 sides, but for GSBs at the perimeters, there may be only 3 or 2 sides have nodes
 *  [node_type]: each GSB have four types of RR nodes: CHANX, CHANY, OPIN and IPIN   
 *               the types of nodes are spread over sides.
 *  [node_index]: index of node in a RR Graph ( RRNodeID )
 *
 *  node_lookup: a 5D fast look-up [x][y][node_side][node_type][node_index] to place PORT direction in GSBs
 *  [x][y]: each GSB has a unique coordinator 
 *  [node_side]: each GSB has 4 sides, but for GSBs at the perimeters, there may be only 3 or 2 sides have nodes
 *  [node_type]: each GSB have four types of RR nodes: CHANX, CHANY, OPIN and IPIN   
 *               the types of nodes are spread over sides.
 *  [node_index]: port direction node in a RR Graph ( enum PORTS )

 *  TODO:
 *  Import the vectors storing unique SBs and CB modules
 *
 ***********************************************************************/

class RRGraphGSB {
  public: /* Constructors */
    RRGraphGSB();
  public: /* Accessors */
  public: /* Coordinator generation */
  public: /* Accessors */
    size_t get_num_sides(const DeviceCoordinator& gsb_coordinator) const;
    size_t get_chan_width(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const;
    size_t get_cb_chan_width(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type) const;
    size_t get_max_chan_width(const DeviceCoordinator& gsb_coordinator) const;
    RRNodeId get_chan_node(const DeviceCoordinator& gsb_coordinator, 
                           const e_side& gsb_side, const size_t& track_id) const;
    enum PORTS get_chan_node_direction(const DeviceCoordinator& gsb_coordinator, 
                                       const e_side& gsb_side, const size_t& track_id) const;
    size_t get_num_ipin_nodes(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const;
    RRNodeId get_ipin_node(const DeviceCoordinator& gsb_coordinator, 
                           const e_side& gsb_side, const size_t& node_id) const;
    size_t get_num_opin_nodes(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const;
    RRNodeId get_opin_node(const DeviceCoordinator& gsb_coordinator, 
                           const e_side& gsb_side, const size_t& node_id) const;
    size_t get_node_index(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                          const t_rr_type& node_type, const RRNodeId& rr_node_id, const enum PORTS& node_direction) const;
    size_t get_chan_node_index(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                               const RRNodeId& rr_node_id, const enum PORTS& node_direction) const;
    size_t get_cb_chan_node_index(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type, 
                                  const RRNodeId& rr_node_id, const enum PORTS& node_direction) const;
    void get_node_side_and_index(const DeviceCoordinator& gsb_coordinator, const t_rr_type& node_type,
                                 const RRNodeId& rr_node_id, const enum PORTS& node_direction, 
                                 /* Pointers to return values */
                                 e_side* node_side, size_t* node_index) const;
    bool is_sb_node_exist_opposite_side(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                        const t_rr_type& node_type, const RRNodeId& rr_node_id) const;
    bool is_sb_node_passing_wire(const RRGraph& rr_graph, /* Need RRGraph to provide coordinator information */
                                 const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                 const size_t& track_id) const;
  public: /* Mutators */
    void add_node(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side,
                  const t_rr_type& node_type, const size_t& index,
                  const RRNodeId& rr_node_id, const enum PORTS& node_direction); 
  public: /* Coordinator and side conversion */
    DeviceCoordinator get_side_block_coordinator(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const;
    DeviceCoordinator get_sb_coordinator(const DeviceCoordinator& gsb_coordinator) const;
    DeviceCoordinator get_cb_coordinator(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type) const;
    DeviceCoordinator get_grid_coordinator(const DeviceCoordinator& gsb_coordinator) const;
    e_side get_cb_chan_side(const t_rr_type& cb_type) const;
    e_side get_cb_chan_side(const e_side& ipin_side) const;
    std::vector<e_side> get_cb_ipin_sides(const t_rr_type& cb_type) const;
    t_rr_type get_chan_node_type(const e_side& gsb_side) const;
  public: /* Build/Free fast node look-up */
    void reserve_lookup(const DeviceCoordinator& gsb_coordinator, const size_t& num_sides);
    void resize_lookup_upon_need(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side,
                                 const t_rr_type& node_type, const size_t& index); 
    void invalidate_fast_lookup() const;
  private: /* Build/Free fast node look-up */
    void reserve_node_lookup(const DeviceCoordinator& gsb_coordinator, const size_t& num_sides);
    void reserve_port_lookup(const DeviceCoordinator& gsb_coordinator, const size_t& num_sides);
    void resize_node_lookup_upon_need(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side,
                                      const t_rr_type& node_type, const size_t& index); 
    void resize_port_lookup_upon_need(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side,
                                      const t_rr_type& node_type, const size_t& index); 
    void invalidate_fast_node_lookup() const;
    void invalidate_fast_port_lookup() const;
  public: /* Validate/Invalidate fast node look-up */
    bool valid_fast_node_lookup() const;
    bool valid_gsb_coordinator(const DeviceCoordinator& gsb_coordinator) const;
    bool valid_gsb_side(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const;
    bool valid_node_type(const DeviceCoordinator& gsb_coordinator, 
                         const e_side& gsb_side, const t_rr_type& node_type) const;
    bool valid_track_id(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                        const t_rr_type& node_type, const size_t& track_id) const;
    bool valid_cb_type(const t_rr_type& cb_type) const;
  private: /* Internal Data */
    /* fast look-up [x][y][node_side][node_type][num_nodes] */
    typedef std::vector< std::vector< std::vector< std::vector< std::vector<RRNodeId> > > > > NodeLookup;
    mutable NodeLookup node_lookup_;
    typedef std::vector< std::vector< std::vector< std::vector< std::vector<enum PORTS> > > > > PortLookup;
    mutable PortLookup port_lookup_;
};

#endif

/************************************************************
 * End Of File (EOF)
 ***********************************************************/
