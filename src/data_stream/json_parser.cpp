/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Parse data stream objects
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Daniel Williams
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

#include <cmath>
#include <vector>

// wxWidgets core
#include <wx/wx.h>
#include <wx/log.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/time.h>

// wx extension
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "data_stream/data_parsers.h"
#include "data_stream/data_stream_event.h"

namespace parsers {
namespace JSON {

// ========    function definitions   ========
DataStreamEvent::type DeduceType( wxString& message_type){
    typedef DataStreamEvent::type type;

    if( _T("ATT") == message_type ||
              _T("GGA") == message_type ||
              _T("HDT") == message_type ||
              _T("TPV") == message_type ||
              _T("VTG") == message_type ){
        return type::OWNSHIP;
    }else if( _T("AIS") == message_type ||
            _T("AIVDM") == message_type){
        return type::TARGET;
    }else if( message_type.IsEmpty() ||
              _T("DEVICE") == message_type ||
              _T("DEVICES") == message_type ||
              _T("VERSION") == message_type ||
              _T("WATCH") == message_type){
        return type::IGNORE;
    }

    wxLogMessage("    >> unrecognized json document type: %s ", message_type);
    return type::UNKNOWN;
}

bool generateNextEvent( wxInputStream& source, DataStreamEvent& output ){
    wxString document = readNextDocument( source);
    if( document.IsEmpty()){
        return false;
    }

    wxJSONReader validator(wxJSONREADER_STRICT);
    wxJSONValue object;

    const int error_count = validator.Parse( document, &object );

    if( 0 == error_count && object.IsObject() ){
        output.SetData( document);
        output.SetFormat( DataStreamEvent::format::JSON);

        wxString message_type = object.Get("class", wxEmptyString).AsString();
        output.SetType( DeduceType( message_type) );

        return true;
    }

    wxLogError( "json-stream-parser produced a bad message! (%d errors found!)", error_count);
    wxLogError( validator.GetErrors()[0] );
    wxLogError( document );
    return false;
}

wxString readNextDocument( wxInputStream &source){
    const size_t READ_CHUNK_SIZE = 1024;
    char buffer[READ_CHUNK_SIZE];

    std::string document;
    document.reserve(READ_CHUNK_SIZE+1);

    if( source.CanRead()){
        source.Read( buffer, READ_CHUNK_SIZE);
        const size_t bytes_read = source.LastRead();
        assert( bytes_read <= READ_CHUNK_SIZE );

        if( wxSTREAM_NO_ERROR == source.GetLastError() && ( 0 < bytes_read )){
            int bracket_depth = 0;

            for( size_t end_index = 0; end_index < bytes_read; ++end_index){
                char cur_byte = buffer[end_index];

                document.push_back( cur_byte);

                if('{'== cur_byte){
                    ++bracket_depth;
                }else if( '}' == cur_byte){
                    --bracket_depth;

                    if(0 == bracket_depth){
                        // unread unused bytes
                        const size_t bytes_excess = bytes_read - end_index - 1;

                        // un-read excess bytes back to the stream
                        source.Ungetch( static_cast<void*>(buffer+end_index+1), bytes_excess);

                        return wxString( document);
                    }else if(0 > bracket_depth){
                        bracket_depth = 0;
                        document.clear();
                    }
                }
            }
        }
    }

    return wxEmptyString;
}



wxString toPrettyJSON( const wxJSONValue & document){
    wxString log_buffer;
    wxJSONWriter writer( wxJSONWRITER_STYLED, 0, 4);
    writer.Write( document, log_buffer );
    return log_buffer;
}

}; // namespace JSON
}; // namespace parsers
