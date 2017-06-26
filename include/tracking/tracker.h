/***************************************************************************
 *
 * Project:  OpenCPN
 * Author:   Daniel Williams 
 ***************************************************************************
 *   Copyright (C) 2017
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

#ifndef __TRACKER_H__
#define __TRACKER_H__

#include <cstdint>
#include <map>
#include <memory>

#include "target.h"

class TrackList;
extern TrackList *pTrackList; // defined at Track.h:147

namespace tracking {

class Tracker {
public:
    Tracker();
    // virtual ~Tracker();
    
    // Search the current AISTargetList for a match ...
    // and return NULL if not found.
    Target* GetTarget( const uint32_t target_id );
    
protected:
    typedef uint32_t key_type;
    
    // eventual target
    //std::map< uint32_t, std::shared_ptr<Target> >  m_targets;
    
    
};  // class Tracker
extern Tracker g_tracker;

};  // namespace AIS

#endif