/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2017 Daniel Williams
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

#ifndef __DATA_PARSERS_H__
#define __DATA_PARSERS_H__

#include "data_stream_event.h"

class wxJSONValue;

namespace parsers {
   
namespace JSON {
// public:
    bool generateNextEvent( wxInputStream& socket_stream, DataStreamEvent& output_event );
    
    float itof( const uint32_t input );


// private:
    DataStreamEvent::type DeduceType( wxString& document);
    wxString readNextDocument( wxInputStream &source);
    wxString toPrettyJSON( const wxJSONValue & document);
    
}; // namespace JSON

namespace NMEA0183{ 
    //public: 
    // (NYI)
    // std::unique_ptr<DataUpdate> parseNMEA0183Sentence( wxInputStream &doc_source );
    
    // // protected:
    // OwnShipUpdate* ToOwnShip( const wxString& document);
    // TargetUpdate* ToTarget( const wxString& document);
}; // namespace NMEA0183

}; // namespace parsers


#endif
