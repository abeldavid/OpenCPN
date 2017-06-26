/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Track AIS Targets
 * Author:   Daniel Williams
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

#include "tracking/tracker.h"

using tracking::Tracker;
using tracking::Target;
// using tracking::Track;

Tracker g_tracker;

Tracker::Tracker(){
}

// Tracker::~Tracker(){}

Target* Tracker::GetTarget( const uint32_t target_id ){
    // return m_targets.get( target_id, nullptr );
    return nullptr;    
}




