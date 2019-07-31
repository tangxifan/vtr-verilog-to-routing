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
 * Filename:    device_coordinator.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/07/31  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */

#include <algorithm>
#include "device_coordinator.h"

/* Member functions for DeviceCoordinator */
/* Public constructors */
DeviceCoordinator::DeviceCoordinator(const DeviceCoordinator& coordinator) {
  set(coordinator.get_x(), coordinator.get_y());
  return;
}

DeviceCoordinator::DeviceCoordinator(size_t x, size_t y) {
  set(x, y);
  return;
}

DeviceCoordinator::DeviceCoordinator() {
  set(0, 0);
  return;
}

/*****************************************************
 * Operator overloaded  
 *****************************************************/
/* Sum two coordinators */
DeviceCoordinator DeviceCoordinator::operator + (const DeviceCoordinator& obj) {
  DeviceCoordinator ret;
  ret.set(obj.get_x() + x_, obj.get_y() + y_);
  return ret;
}

/* Evaluate if two coordinators are same */
bool DeviceCoordinator::operator == (const DeviceCoordinator& obj) {
  if ( (x_ == obj.get_x()) 
    && (y_ == obj.get_y()) ) {
    return true;
  }
  return false;
}

/* Evaluate if two coordinators are same */
bool DeviceCoordinator::operator != (const DeviceCoordinator& obj) {
  if ( (x_ != obj.get_x()) 
    || (y_ != obj.get_y()) ) {
    return true;
  }
  return false;
}

/*****************************************************
 * Public accessors  
 ****************************************************/

size_t DeviceCoordinator::get_x() const {
  return x_;
}

size_t DeviceCoordinator::get_y() const {
  return y_;
}

/*****************************************************
 * Public mutators 
 ****************************************************/
void DeviceCoordinator::set(size_t x, size_t y) {
  set_x(x); 
  set_y(y);
  return;
}

void DeviceCoordinator::set_x(size_t x) {
  x_ = x;
  return;
}

void DeviceCoordinator::set_y(size_t y) {
  y_ = y;
  return;
}

void DeviceCoordinator::rotate() {
  std::swap(x_, y_);
  return;
}

void DeviceCoordinator::clear() {
  set(0, 0);
  return;
}

/************************************************************
 * End Of File (EOF)
 ***********************************************************/

