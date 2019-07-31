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
 * Filename:    device_coordinator.h
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

#ifndef DEVICE_COORDINATOR_H
#define DEVICE_COORDINATOR_H

/* Coordinator System for FPGA Device
 * It is based on a 3-D (x,y,z) coordinator system
 * (x,y) is used for all the routing resources,
 * z is used for only grid, which have multiple logic blocks 
 */
/* FIXME: overlapped with the template class Point! Should merge!!! */
class DeviceCoordinator {
  public: /* Contructors */
    DeviceCoordinator(size_t x, size_t y);
    DeviceCoordinator(const DeviceCoordinator&); /* copy constructor*/
    DeviceCoordinator();
  public: /* Operator overloaded  */
    DeviceCoordinator operator + (const DeviceCoordinator& obj);
    bool operator == (const DeviceCoordinator& obj);
    bool operator != (const DeviceCoordinator& obj);
  public: /* Accessors */
    size_t get_x() const;
    size_t get_y() const;
  public: /* Mutators */
    void set(size_t x, size_t y);
    void set_x(size_t x);
    void set_y(size_t y);
    void rotate();
    void clear();
  private: /* Internal Mutators */
  private: /* internal functions */
  private: /* Internal Data */
    size_t x_;
    size_t y_;
    size_t z_;
};

#endif

/************************************************************
 * End Of File (EOF)
 ***********************************************************/
