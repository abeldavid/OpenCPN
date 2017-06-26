/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#include "data_stream/data_stream_event.h"

wxDEFINE_EVENT( wxEVT_OCPN_DATASTREAM, DataStreamEvent);

DataStreamEvent::DataStreamEvent( wxEventType commandType, int id )
      : wxEvent( id, commandType)
      , m_format( DataStreamEvent::format::UNKNOWN)
      , m_type( DataStreamEvent::type::UNKNOWN)
      , m_data( wxEmptyString)
      , mp_data_stream( nullptr)
{
}

DataStreamEvent::DataStreamEvent( DataStreamEvent::source source_id)
      : wxEvent( static_cast<int>(source_id), wxEVT_OCPN_DATASTREAM)
      , m_format( DataStreamEvent::format::UNKNOWN)
      , m_type( DataStreamEvent::type::UNKNOWN)
      , m_data( wxEmptyString)
      , mp_data_stream( nullptr)
{
}

DataStreamEvent::~DataStreamEvent(){}

wxString DataStreamEvent::GetSourceString() const {
    auto source = static_cast<DataStreamEvent::source>(GetId());
    if( DataStreamEvent::source::GPSD == source){
        return _T("GPSD");
    }else if(DataStreamEvent::source::PLUGIN == source){
        return _T("PLUGIN");
    }else if(DataStreamEvent::source::SERIAL == source){
        return _T("SERIAL");
    }else if(DataStreamEvent::source::TCP == source ){
        return _T("TCP");
    }else if(DataStreamEvent::source::UDP == source ){
        return _T("UDP");
    }

    return _T("Unknown");
}

wxString DataStreamEvent::GetFormatString() const {
    if( DataStreamEvent::format::JSON == m_format ){
        return _T("JSON");
    }else if( DataStreamEvent::format::NMEA0183 == m_format ){
        return _T("NMEA_0183");
    }

    return _T("UNKNOWN");
}

wxString DataStreamEvent::GetTypeString() const {
    if( DataStreamEvent::type::IGNORE == m_type ){
        return _T("IGNORE");
    }else if( DataStreamEvent::type::OWNSHIP == m_type ){
        return _T("OWNSHIP");
    }else if( DataStreamEvent::type::TARGET == m_type ){
        return _T("TARGET");
    }else if( DataStreamEvent::type::WAYPOINTS == m_type ){
        return _T("WAYPOINTS");
    }else if( DataStreamEvent::type::ZONE == m_type ){
        return _T("ZONE");
    }

    return _T("UNKNOWN");
}

wxString DataStreamEvent::GetEventSummary() const {
    return GetSourceString() + "//"+ GetFormatString() + "//" + GetTypeString();
}

// ====== ====== ======  Strip NMEA V4 tags from message  ====== ====== ======
wxString DataStreamEvent::ProcessNMEA4Tags()
{
    wxString msg = wxString(GetNMEAString().c_str(), wxConvUTF8);
   
    int idxFirst =  msg.Find('\\');
    
    if(wxNOT_FOUND == idxFirst)
        return msg;
    
    if(idxFirst < (int)msg.Length()-1){
        int idxSecond = msg.Mid(idxFirst + 1).Find('\\') + 1;
        if(wxNOT_FOUND != idxSecond){
            if(idxSecond < (int)msg.Length()-1){
                
               // wxString tag = msg.Mid(idxFirst+1, (idxSecond - idxFirst) -1);
                return msg.Mid(idxSecond + 1);
            }
        }
    }
    
    return msg;
}

// ====== ====== ======  Required by wxWidgets Event Handling ====== ====== ======
wxEvent* DataStreamEvent::Clone() const
{
    DataStreamEvent *new_event=new DataStreamEvent( GetSource() );
    new_event->m_format = m_format;
    new_event->m_type = m_type;
    new_event->m_data = m_data;
    new_event->mp_data_stream = this->mp_data_stream;
    
    return new_event;
}

