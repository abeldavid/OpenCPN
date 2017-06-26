/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
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

#ifndef __MMSI_TRACKER_H__
#define __MMSI_TRACKER_H__

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "mmsi_properties.h"

namespace AIS {

typedef std::map< mmsi_t, std::shared_ptr<MMSIProperties> >::iterator mmsi_iter_t;

class MMSITracker {
public:
    MMSITracker();
    ~MMSITracker();

    mmsi_iter_t begin();
    mmsi_iter_t end();


    void clear();

    MMSIProperties& clone( mmsi_t mmsi_old, mmsi_t mmsi_new);

    bool contains( mmsi_t mmsi) const ;

    void emplace( const std::string& serialized_data );

    // takes ownership of pointer
    void insert( MMSIProperties * new_props);

    void erase( const mmsi_t key );

    // register or retrieve
    MMSIProperties& GetOrRegister( mmsi_t mmsi);


    // retrieve only
    MMSIProperties* operator[]( mmsi_t mmsi);

    void LoadShipNames( const std::string& filename );
    void SaveAllShipNames();
    void SaveSingleShipName( mmsi_t mmsi );

    size_t size() const;


protected:
    std::string m_name_file;
    std::map< mmsi_t, std::shared_ptr<MMSIProperties> >  m_mmsi_properties;

}; // class MMSITracker

}; // namespace AIS

#endif // __MMSI_PROPERTIES_H__
