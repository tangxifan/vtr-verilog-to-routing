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
 * Filename:    rr_graph_gsb.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/07/31  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file constains the member functions for class RRGraphGSB 
 ************************************************************************/

#include <algorithm>

#include "vpr_error.h"
#include "rr_graph_gsb.h"
#include "side_manager.h"

/************************************************************************
 *  Constructors for class RRGraphGSB
 ************************************************************************/
/* Construct an empty GSB */
RRGraphGSB::RRGraphGSB(){
  
  invalidate_fast_node_lookup();

  return;
}

/************************************************************************
 * Accessors 
 ************************************************************************/

/* Get the number of sides of the GSB */
size_t RRGraphGSB::get_num_sides(const DeviceCoordinator& gsb_coordinator) const {
  VTR_ASSERT_SAFE(valid_fast_node_lookup());
  /* Fast look-up recordes the max number of sides! */
  /* validate coordinator */
  VTR_ASSERT_SAFE(valid_gsb_coordinator(gsb_coordinator));
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  return node_lookup_[x][y].size();
}

/* Get the channel width of a side of a GSB */
size_t RRGraphGSB::get_chan_width(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const {
  VTR_ASSERT_SAFE(valid_fast_node_lookup());
  /* validate coordinator */
  VTR_ASSERT_SAFE(valid_gsb_coordinator(gsb_coordinator));
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  t_rr_type chan_type = get_chan_node_type(gsb_side);

  /* Make sure the CHAN type is valid in look-up */
  VTR_ASSERT_SAFE(valid_node_type(gsb_coordinator, gsb_side, chan_type));
  return node_lookup_[x][y][(size_t)gsb_side][chan_type].size();
}

/* Get the channel width of a CB inside a GSB with a given type of the CB (X-direction or Y-direction) */
size_t RRGraphGSB::get_cb_chan_width(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type) const {
  VTR_ASSERT_SAFE(valid_fast_node_lookup());
  /* validate coordinator */
  VTR_ASSERT_SAFE(valid_gsb_coordinator(gsb_coordinator));
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  e_side cb_chan_side = get_cb_chan_side(cb_type);
  SideManager side_manager(cb_chan_side);
  return node_lookup_[x][y][side_manager.to_size_t()][cb_type].size();
}

/* Get the maximum channnel width in a GSB */
size_t RRGraphGSB::get_max_chan_width(const DeviceCoordinator& gsb_coordinator) const {
  size_t max_chan_width = 0;
  for (size_t side = 0; side < get_num_sides(gsb_coordinator); ++side) {
    SideManager side_manager(side);
    max_chan_width = std::max(max_chan_width, get_chan_width(gsb_coordinator, side_manager.get_side()));
  }
  return max_chan_width;
}

/* Get the Id in RR Graph (RRNodeId) of a channel node in the context of a GSB */
RRNodeId RRGraphGSB::get_chan_node(const DeviceCoordinator& gsb_coordinator, 
                                   const e_side& gsb_side, const size_t& track_id) const {
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);
  t_rr_type chan_node_type = get_chan_node_type(gsb_side);
  /* Make sure the track_id is valid */
  VTR_ASSERT_SAFE(valid_track_id(gsb_coordinator, gsb_side, chan_node_type, track_id));
  return node_lookup_[x][y][gsb_side_manager.to_size_t()][chan_node_type][track_id];
}

/* Get the port direction (OUT or IN) of a channel node in the context of a GSB */
enum PORTS RRGraphGSB::get_chan_node_direction(const DeviceCoordinator& gsb_coordinator, 
                                               const e_side& gsb_side, const size_t& track_id) const {
  /* Rules are simple: 
   * TOP    side: INC_DIRECTION => OUTPUT; DEC_DIRECTION => INPUT
   * LEFT   side: INC_DIRECTION => INPUT;  DEC_DIRECTION => OUTPUT
   * RIGHT  side: INC_DIRECTION => OUTPUT; DEC_DIRECTION => INPUT
   * BOTTOM side: INC_DIRECTION => INPUT;  DEC_DIRECTION => OUTPUT
   *
   *                      INC  DEC
   *                      /|\   |
   *                       |   \|/
   *                     +---------+
   *          DEC <------|         |<------- DEC
   *                     |   GSB   |
   *          INC ------>|         |-------> INC
   *                     +---------+
   *                      /|\  |
   *                       |  \|/
   *                      INC DEC
   */
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);
  t_rr_type chan_node_type = get_chan_node_type(gsb_side);

  /* Make sure the track_id is valid */
  VTR_ASSERT_SAFE(valid_track_id(gsb_coordinator, gsb_side, chan_node_type, track_id));
  return port_lookup_[x][y][gsb_side_manager.to_size_t()][chan_node_type][track_id];
}

/* Get the number of IPIN nodes on a side of a GSB */
size_t RRGraphGSB::get_num_ipin_nodes(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const {
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);

  /* Make sure the IPIN is valid in look-up */
  VTR_ASSERT_SAFE(valid_node_type(gsb_coordinator, gsb_side, IPIN));

  return node_lookup_[x][y][gsb_side_manager.to_size_t()][IPIN].size();
}

/* Get the Id in RR Graph (RRNodeId) of a IPIN node in the context of a GSB */
RRNodeId RRGraphGSB::get_ipin_node(const DeviceCoordinator& gsb_coordinator, 
                                   const e_side& gsb_side, const size_t& node_id) const {
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);

  /* Make sure the track_id is valid */
  VTR_ASSERT_SAFE(valid_track_id(gsb_coordinator, gsb_side, IPIN, node_id));
  return node_lookup_[x][y][gsb_side_manager.to_size_t()][IPIN][node_id];
}

/* Get the number of IPIN nodes on a side of a GSB */
size_t RRGraphGSB::get_num_opin_nodes(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const {
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);

  /* Make sure the OPIN is valid in look-up */
  VTR_ASSERT_SAFE(valid_node_type(gsb_coordinator, gsb_side, OPIN));
  return node_lookup_[x][y][gsb_side_manager.to_size_t()][OPIN].size();
}

/* Get the Id in RR Graph (RRNodeId) of a IPIN node in the context of a GSB */
RRNodeId RRGraphGSB::get_opin_node(const DeviceCoordinator& gsb_coordinator, 
                                   const e_side& gsb_side, const size_t& node_id) const {
  /* Get the node id */
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);

  /* Make sure the track_id is valid */
  VTR_ASSERT_SAFE(valid_track_id(gsb_coordinator, gsb_side, OPIN, node_id));
  return node_lookup_[x][y][gsb_side_manager.to_size_t()][OPIN][node_id];
}

/* With a given RRNodeId, find the index in the fast look-up */
/* Think about using another NodeId to get rid of data overflow problem! */
size_t RRGraphGSB::get_node_index(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                  const t_rr_type& node_type, const RRNodeId& rr_node_id, const enum PORTS& node_direction) const {
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);

  /* Make sure the node_type is valid in look-up */
  VTR_ASSERT_SAFE(valid_node_type(gsb_coordinator, gsb_side, node_type));

  /* Criterias in finding a wanted node: 
   * 1. NodeId must match 
   * 2. Port direction must match 
   */
  for (size_t i = 0; i < node_lookup_[x][y][gsb_side_manager.to_size_t()][node_type].size(); ++i) { 
    if ( (rr_node_id == node_lookup_[x][y][gsb_side_manager.to_size_t()][node_type][i]) 
      && (node_direction == port_lookup_[x][y][gsb_side_manager.to_size_t()][node_type][i]) ) {
      /* Return the index */
      return i;
    }
  }
  
  /* FIXME: We may have a problem when the size of vector is larger than the limit of int */
  return GSB_OPEN_NODE_ID; 
}

/* With a given RRNodeId of a channel node, find the index in the fast look-up */
size_t RRGraphGSB::get_chan_node_index(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                       const RRNodeId& rr_node_id, const enum PORTS& node_direction) const {
  /* Find the channel node type using the gsb_side */
  t_rr_type chan_node_type = get_chan_node_type(gsb_side);
  return get_node_index(gsb_coordinator, gsb_side, chan_node_type, rr_node_id, node_direction);
}

/* With a given RRNodeId of a channel node of a CB, find the index in the fast look-up */
size_t RRGraphGSB::get_cb_chan_node_index(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type, 
                                          const RRNodeId& rr_node_id, const enum PORTS& node_direction) const {
  /* Find the channel side using the cb_type */
  e_side chan_side = get_cb_chan_side(cb_type);
  return get_node_index(gsb_coordinator, chan_side, cb_type, rr_node_id, node_direction);
}

/* With a given RRNodeId, find the side and index that it locates in a GSB */
void RRGraphGSB::get_node_side_and_index(const DeviceCoordinator& gsb_coordinator, const t_rr_type& node_type,
                                         const RRNodeId& rr_node_id, const enum PORTS& node_direction, 
                                         /* Pointers to return values */
                                         e_side* node_side, size_t* node_index) const {
  size_t side;
  SideManager side_manager;
  
  /* Count the number of existence of RRNodeId in a GSB
   * It could happen that same RRNodeId appears on different sides of a SB
   * For example, a routing track go vertically across the SB.
   * Then its corresponding rr_node appears on both TOP and BOTTOM sides of this SB. 
   * We need to ensure that the found RRNode has the same direction as user want.
   * By specifying the direction of RRNode, There should be only one RRNode can satisfy!
   */
  for (side = 0; side < get_num_sides(gsb_coordinator); ++side) {
    side_manager.set_side(side);
    (*node_index) = get_node_index(gsb_coordinator, side_manager.get_side(), node_type, rr_node_id, node_direction);
    if (GSB_OPEN_NODE_ID != (*node_index)) {
      break;
    }
  }

  if (side == get_num_sides(gsb_coordinator)) {
    /* we find nothing, return NUM_SIDES, and a OPEN node (-1) */
    (*node_side) = NUM_SIDES;
    VTR_ASSERT_SAFE(GSB_OPEN_NODE_ID == (*node_index));
    return;
  }

  (*node_side) = side_manager.get_side();
  VTR_ASSERT_SAFE(GSB_OPEN_NODE_ID != (*node_index));

  return;
}

/* Try to find a given RRNodeId on the opposite side of a GSB
 * Restriction: 
 * we assume the RRNodeId on the gsb_side is an OUTPUT port!!!
 * Should not use this function when RRNodeId is an INPUT port!!!
 */
bool RRGraphGSB::is_sb_node_exist_opposite_side(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                                const t_rr_type& node_type, const RRNodeId& rr_node_id) const {
  SideManager side_manager(gsb_side);

  VTR_ASSERT((CHANX == node_type) || (CHANY == node_type));

  /* See if we can find the same src_rr_node in the opposite chan_side 
   * if there is one, it means a shorted wire across the SB 
   */
  size_t index = get_node_index(gsb_coordinator, side_manager.get_opposite(), node_type, rr_node_id, IN_PORT);

  if (GSB_OPEN_NODE_ID != index) {
    return true;
  }

  return false;
}

/************************************************************************
 * Check if the node indicates a passing wire across the Switch Block part of the GSB 
 * Therefore, we actually do the following check 
 * Check if a track starts from this GSB or not 
 * For INC_DIRECTION
 * (xlow, ylow) should be same as the GSB side coordinator 
 * For DEC_DIRECTION
 * (xhigh, yhigh) should be same as the GSB side coordinator 
 ***********************************************************************/
bool RRGraphGSB::is_sb_node_passing_wire(const RRGraph& rr_graph, /* Need RRGraph to provide coordinator information */
                                         const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                         const size_t& track_id) const {
  /* Get the rr_node */
  //t_rr_node* track_node = get_chan_node(node_side, track_id);
  /* Get the coordinators */
  //DeviceCoordinator side_coordinator = get_side_block_coordinator(node_side); 

  /* Get the coordinator of where the track starts */
  //DeviceCoordinator track_start = get_track_rr_node_start_coordinator(track_node);

  /* INC_DIRECTION start_track: (xlow, ylow) should be same as the GSB side coordinator */
  /* DEC_DIRECTION start_track: (xhigh, yhigh) should be same as the GSB side coordinator */
  //if (  (track_start.get_x() == side_coordinator.get_x())
  //   && (track_start.get_y() == side_coordinator.get_y()) 
  //   && (OUT_PORT == get_chan_node_direction(node_side, track_id)) ) {
    /* Double check: start track should be an OUTPUT PORT of the GSB */
  //  return false; /* This is a starting point */
  //}

  /* Get the coordinator of where the track ends */
  //DeviceCoordinator track_end = get_track_rr_node_end_coordinator(track_node);

  /* INC_DIRECTION end_track: (xhigh, yhigh) should be same as the GSB side coordinator */ 
  /* DEC_DIRECTION end_track: (xlow, ylow) should be same as the GSB side coordinator */ 
  //if (  (track_end.get_x() == side_coordinator.get_x())
  //   && (track_end.get_y() == side_coordinator.get_y()) 
  //   && (IN_PORT == get_chan_node_direction(node_side, track_id)) ) {
    /* Double check: end track should be an INPUT PORT of the GSB */
  //  return false; /* This is an ending point */
  //}

  /* Reach here it means that this will be a passing wire, 
   * we should be able to find the node on the opposite side of the GSB!
   */
  //assert (true == is_sb_node_exist_opposite_side(gsb_coordinator, gsb_side, track_node, node_side));

  return true;
}

/************************************************************************
 * Coordinator and side conversion
 ************************************************************************/
DeviceCoordinator RRGraphGSB::get_side_block_coordinator(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const {
  SideManager side_manager(gsb_side); 
  VTR_ASSERT_SAFE(side_manager.validate());
  DeviceCoordinator ret(get_sb_coordinator(gsb_coordinator));

  switch (side_manager.get_side()) {
  case TOP:
    /* (0 == side) */
    /* 1. Channel Y [x][y+1] inputs */
    ret.set_y(ret.get_y() + 1);
    break;
  case RIGHT:
    /* 1 == side */
    /* 2. Channel X [x+1][y] inputs */
    ret.set_x(ret.get_x() + 1);
    break;
  case BOTTOM:
    /* 2 == side */
    /* 3. Channel Y [x][y] inputs */
    break;
  case LEFT:
    /* 3 == side */
    /* 4. Channel X [x][y] inputs */
    break;
  default:
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid side!\n");
  }

  return ret;
}

/* Get the coordinator of SB inside the GSB */
DeviceCoordinator RRGraphGSB::get_sb_coordinator(const DeviceCoordinator& gsb_coordinator) const {
  /* validate coordinator */
  VTR_ASSERT_SAFE(valid_gsb_coordinator(gsb_coordinator));
  return gsb_coordinator;
} 

/* Get the coordinator of a CB inside the GSB */
DeviceCoordinator RRGraphGSB::get_cb_coordinator(const DeviceCoordinator& gsb_coordinator, const t_rr_type& cb_type) const {
  VTR_ASSERT_SAFE(valid_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return get_side_block_coordinator(gsb_coordinator, LEFT);
  case CHANY:
    return get_side_block_coordinator(gsb_coordinator, TOP);
  default: 
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid type of connection block!\n");
  }
}

/* Get the coordinator of Grid */
DeviceCoordinator RRGraphGSB::get_grid_coordinator(const DeviceCoordinator& gsb_coordinator) const {
  DeviceCoordinator ret(get_sb_coordinator(gsb_coordinator));
  ret.set_y(ret.get_y() + 1);

  return ret;
}

/* Get the coordinator of the GSB */
e_side RRGraphGSB::get_cb_chan_side(const t_rr_type& cb_type) const {
  VTR_ASSERT_SAFE(valid_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return LEFT;
  case CHANY:
    return TOP;
  default: 
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid type of connection block!\n");
  }
}

e_side RRGraphGSB::get_cb_chan_side(const e_side& ipin_side) const {
  switch(ipin_side) {
  case TOP:
    return LEFT;
  case RIGHT:
    return TOP;
  case BOTTOM:
    return LEFT;
  case LEFT:
    return TOP;
  default: 
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid side!\n");
  }
}

/* Get the side of IPIN nodes locating at a GSB with a given type of the CB (X-direction or Y-direction) */
std::vector<e_side> RRGraphGSB::get_cb_ipin_sides(const t_rr_type& cb_type) const {
  VTR_ASSERT_SAFE(valid_cb_type(cb_type));
  
  std::vector<e_side> ipin_sides;

  /* Make sure a clean start */
  ipin_sides.clear();

  switch(cb_type) {
  case CHANX:
    ipin_sides.push_back(TOP);
    ipin_sides.push_back(BOTTOM);
    break;
  case CHANY:
    ipin_sides.push_back(RIGHT);
    ipin_sides.push_back(LEFT);
    break;
  default: 
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid type of connection block!\n");
  }

  return ipin_sides;
}

/* Find the type of channel nodes at a given gsb side 
 * The rules are simple:
 * LEFT and RIGHT are wired to CHANX
 * TOP and BOTTOM are wired to CHANY
 */
t_rr_type RRGraphGSB::get_chan_node_type(const e_side& gsb_side) const {
  switch (gsb_side) {
  case LEFT:
  case RIGHT:
    return CHANX;
  case TOP:
  case BOTTOM:
    return CHANY;
  default:
    /* Error out */
    vpr_throw(VPR_ERROR_ROUTE, __FILE__, __LINE__,
              "Invalid side for GSB!\n");
  }
}

/************************************************************************
 *  Build or Invalidate the fast look-up for the GSB
 ************************************************************************/
void RRGraphGSB::invalidate_fast_node_lookup() const {
  node_lookup_.clear();
}

bool RRGraphGSB::valid_fast_node_lookup() const {
  return !node_lookup_.empty();
}

/* Check if the coordinator is a valid one in GSB array */
bool RRGraphGSB::valid_gsb_coordinator(const DeviceCoordinator& gsb_coordinator) const {
  return ( (gsb_coordinator.get_x() < node_lookup_.size()) 
        && (gsb_coordinator.get_y() < node_lookup_[gsb_coordinator.get_x()].size()));
}

bool RRGraphGSB::valid_cb_type(const t_rr_type& cb_type) const {
  if ( (CHANX == cb_type) || (CHANY == cb_type) ) {
    return true;
  }
  return false;
}

/* Check if the gsb_side is a valid value in the GSB look-up */
bool RRGraphGSB::valid_gsb_side(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side) const {
  VTR_ASSERT_SAFE(valid_gsb_coordinator(gsb_coordinator));
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  SideManager gsb_side_manager(gsb_side);
  if (gsb_side_manager.to_size_t() < node_lookup_[x][y].size() ) { 
    return true;
  }
  return false;
}

/* Check if the node_type is a valid value in the GSB look-up */
bool RRGraphGSB::valid_node_type(const DeviceCoordinator& gsb_coordinator, 
                                 const e_side& gsb_side, const t_rr_type& node_type) const {
  VTR_ASSERT_SAFE(valid_gsb_side(gsb_coordinator, gsb_side));

  SideManager gsb_side_manager(gsb_side);
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  if ((size_t)node_type < node_lookup_[x][y][gsb_side_manager.to_size_t()].size()) {
    return true;
  }
  return false;
}

/* Check if the track_id is a valid value in the GSB look-up */
bool RRGraphGSB::valid_track_id(const DeviceCoordinator& gsb_coordinator, const e_side& gsb_side, 
                                const t_rr_type& node_type, const size_t& track_id) const {
  VTR_ASSERT_SAFE(valid_node_type(gsb_coordinator, gsb_side, node_type));

  SideManager gsb_side_manager(gsb_side);
  size_t x = gsb_coordinator.get_x();
  size_t y = gsb_coordinator.get_y();
  if (track_id < node_lookup_[x][y][gsb_side_manager.to_size_t()][node_type].size()) {
    return true;
  }
  return false;
}

/************************************************************
 * End Of File (EOF)
 ***********************************************************/
