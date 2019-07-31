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
 * Filename:    side_manager.h
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

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */

#ifndef SIDE_MANAGER_H
#define SIDE_MANAGER_H

#include <cstddef>
#include <string>

#include "physical_types.h"

/* Orientations. */
class SideManager {
  public: /* Constructor */
    SideManager(enum e_side side);
    SideManager();
    SideManager(size_t side);
  public: /* Accessors */
    enum e_side get_side() const;
    enum e_side get_opposite() const;
    enum e_side get_rotate_clockwise() const;
    enum e_side get_rotate_counterclockwise() const;
    bool validate() const;
    size_t to_size_t() const;
    const char* c_str() const;
    std::string to_string() const;
  public: /* Mutators */
    void set_side(size_t side);
    void set_side(enum e_side side);
    void set_opposite();
    void rotate_clockwise();
    void rotate_counterclockwise();
  private: /* internal data */
    enum e_side side_;  
};

#endif

/************************************************************
 * End Of File (EOF)
 ***********************************************************/
