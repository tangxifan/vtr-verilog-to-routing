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
 * Filename:    side_manager.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/07/31  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 * This file defines a class for the sides of a physical block in FPGA architecture
 * Basically, each block has four sides :
 * TOP, RIGHT, BOTTOM, LEFT  
 * This class aims to provide a easy proctol for manipulating a side 
 ***********************************************************************/

#include "side_manager.h"

/* Member functions of class SideManager */

/* Constructors */
SideManager::SideManager(enum e_side side) {
  side_  = side;
}

SideManager::SideManager() {
  side_ = NUM_SIDES;
}

SideManager::SideManager(size_t side) { 
  set_side(side); 
}

/* Public Accessors */
enum e_side SideManager::get_side() const { 
  return side_; 
} 

enum e_side SideManager::get_opposite() const {
  switch (side_) {
  case TOP:
    return BOTTOM; 
  case RIGHT:
    return LEFT;
  case BOTTOM:
    return TOP;
  case LEFT:
    return RIGHT;
  default:
    return NUM_SIDES;
  }
}

enum e_side SideManager::get_rotate_clockwise() const {
  switch (side_) {
  case TOP:
    return RIGHT; 
  case RIGHT:
    return BOTTOM;
  case BOTTOM:
    return LEFT;
  case LEFT:
    return TOP;
  default:
    return NUM_SIDES;
  }
}  

enum e_side SideManager::get_rotate_counterclockwise() const {
  switch (side_) {
  case TOP:
    return LEFT;
  case RIGHT:
    return TOP;
  case BOTTOM:
    return RIGHT; 
  case LEFT:
    return BOTTOM;
  default:
    return NUM_SIDES;
  }
} 

bool SideManager::validate() const {
  if (NUM_SIDES == side_) {
    return false;
  }
  return true;
}

size_t SideManager::to_size_t() const {
  switch (side_) {
  case TOP:
    return 0; 
  case RIGHT:
    return 1;
  case BOTTOM:
    return 2;
  case LEFT:
    return 3;
  default:
    return 4;
  }
}

/* Convert to char array:
 * FIXME: Should use the SIDE_STRING in physical_types.h
 * I did not change here just for a smooth code integration 
 */
const char* SideManager::c_str() const { 
  switch (side_) {
  case TOP:
    return "top"; 
  case RIGHT:
    return "right";
  case BOTTOM:
    return "bottom";
  case LEFT:
    return "left";
  default:
    return "invalid_side";
  }
} 

/* Convert to string 
 * FIXME: Should use the SIDE_STRING in physical_types.h
 * I did not change here just for a smooth code integration 
 */
std::string SideManager::to_string() const { 
  std::string ret;
  switch (side_) {
  case TOP:
    ret.assign("top"); 
    break;
  case RIGHT:
    ret.assign("right");
    break;
  case BOTTOM:
    ret.assign("bottom");
    break;
  case LEFT:
    ret.assign("left");
    break;
  default:
    ret.assign("invalid_side");
    break;
  }

  return ret;
} 

/* Public Mutators */
void SideManager::set_side(size_t side) { 
  switch (side) {
  case 0:
    side_ = TOP;
    return; 
  case 1:
    side_ = RIGHT;
    return; 
  case 2:
    side_ = BOTTOM;
    return; 
  case 3:
    side_ = LEFT;
    return; 
  default:
    side_ = NUM_SIDES;
    return; 
  }
}

void SideManager::set_side(enum e_side side) { 
  side_ = side; 
  return;
}

void SideManager::set_opposite() { 
  side_ = get_opposite(); 
  return;
}
    
void SideManager::rotate_clockwise() {
  side_ = get_rotate_clockwise(); 
  return;
}  

void SideManager::rotate_counterclockwise() {
  side_ = get_rotate_counterclockwise(); 
  return;
}  

/************************************************************
 * End Of File (EOF)
 ***********************************************************/

