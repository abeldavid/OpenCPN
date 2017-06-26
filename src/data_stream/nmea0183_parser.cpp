/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Parse data stream objects
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Daniel Williams, David S. Register
 * 
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
 **************************************************************************/

#include <memory> 
using std::unique_ptr;
 
#include <wx/log.h>
#include <wx/stream.h>
#include <wx/string.h>



#include "data_stream/data_parsers.h"

namespace parsers{
   
namespace NMEA0183{


// ======== file-private declarations ========


// ========    function definitions   ========
void* parseNMEA0183Sentence( wxString &source ){
    // NYI
    wxLogMessage("!! parse-NMEA-0183-sentences is NYI !!! ");
    
    // NYI
    return nullptr;
}


}; // namespace JSON
}; // namespace parsers
