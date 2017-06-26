/***************************************************************************
 *
 * Project:  OpenCPN
 * Authors:  Daniel Williams
 *
 ***************************************************************************
 *   Copyright (C) 2017                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef __TARGET_H__
#define __TARGET_H__

#include <cstdint>
#include <memory> 
#include <vector>

// #include "track.h"

namespace tracking {
  
class Target {
public: 
    Target();
    ~Target();

    // TrackPoint* getLastLocation() const;
    // TrackPoint* getLocation( const time_t timestamp );

// protected:  
    // uint32_t m_id;
    //std::vector< std::shared_ptr<TrackPoint> > mp_track;
    // Track m_track;
    
};  // class Target

}; // namespace tracks

#endif
