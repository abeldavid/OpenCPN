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

#ifndef __DATA_STREAM_EVENT_H__
#define __DATA_STREAM_EVENT_H__

#include <wx/event.h>
#include <wx/string.h>

class DataStream;

class DataStreamEvent: public wxEvent
{
public:
    // Used to populate the `Id` field of `wxEvent` with meaningful information
    enum class source {
        UNKNOWN=0,
        GPSD,
        OTHER,
        PLUGIN,
        SERIAL,
        TCP,
        UDP,
    };

    enum class format {
        UNKNOWN=0,
        JSON,
        NMEA0183,
    };

    enum class type {
        UNKNOWN=0,
        IGNORE,
        OWNSHIP,
        TARGET,
        WAYPOINTS,
        ZONE,
    };

public:
    DataStreamEvent( wxEventType commandType = wxEVT_NULL, int id = 0 );
    DataStreamEvent( DataStreamEvent::source source_id=source::UNKNOWN);
    ~DataStreamEvent( );

    // deprecated accessors
    void SetNMEAString(std::string new_string) { m_data = new_string; }
    void SetStream( DataStream *pDS ) { mp_data_stream = pDS; }
    std::string GetNMEAString() { return m_data.ToStdString(); }
    DataStream *GetStream() { return mp_data_stream; }

    // deprecated
    wxString ProcessNMEA4Tags();

    // accessors
    wxString inline GetData() const { return m_data.ToStdString(); }
    DataStreamEvent::source inline GetSource() const {return static_cast<DataStreamEvent::source>(GetId());}
    DataStreamEvent::format inline GetFormat() const { return m_format; }
    DataStreamEvent::type inline GetType() const { return m_type; }

    wxString GetSourceString() const;
    wxString GetFormatString() const;
    wxString GetTypeString() const;

    explicit operator bool() const { return !m_data.IsEmpty();}
    bool inline IsEmpty() const { return m_data.IsEmpty(); }

    void inline SetData(wxString new_string) { m_data = new_string; }
    void inline SetSource( DataStreamEvent::source source_id){ SetId( static_cast<int>(source_id)); };
    void inline SetFormat( DataStreamEvent::format new_format){ m_format = new_format; }
    void inline SetType( DataStreamEvent::type new_type){ m_type = new_type; }

    // required for sending with wxPostEvent()
    wxEvent *Clone() const;

    wxString GetEventSummary() const ;
private:
    DataStreamEvent::format m_format;
    DataStreamEvent::type m_type;

    wxString m_data;
    DataStream* mp_data_stream;

};

wxDECLARE_EVENT( wxEVT_OCPN_DATASTREAM, DataStreamEvent);

#endif
