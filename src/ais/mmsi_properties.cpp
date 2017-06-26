/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Track MMSI Properties
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
#include <fstream>
#include <iostream>

#include "wx/string.h"
#include "wx/tokenzr.h"

// wx extension
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
 
#include "ais/ais.h"
#include "ais/mmsi_properties.h"

using std::string;

using AIS::MMSIProperties;

// =================== MMSIProperties:: ===================
MMSIProperties::MMSIProperties( mmsi_t init_mmsi)
    : m_mmsi( init_mmsi )
    , m_track_type( TRACK_TYPE::DEFAULT )
    , m_should_track( false)
    , m_should_persist( false)
    , m_sar(false)
    , m_ownship(false)
    , m_follower(false)
{
    ValidateFlags();
}


void MMSIProperties::LoadFromOther( const MMSIProperties& other){
    m_track_type= other.m_track_type;
    m_should_track= other.m_should_track;
    m_should_persist= other.m_should_persist;
    m_sar= other.m_sar;
    m_ownship= other.m_ownship;
    m_follower= other.m_follower;
}

void MMSIProperties::LoadFromConfig( const string& ini_serialized_data ){
    wxStringTokenizer tkz( wxString(ini_serialized_data), _T(";") );
    wxString s;
    
    s = tkz.GetNextToken();
    long mmsil;
    s.ToLong(&mmsil);
    m_mmsi = (int)mmsil;
    ValidateFlags();
    
    s = tkz.GetNextToken();
    if(s.Len()){
        if(s.Upper() == _T("ALWAYS"))
            m_track_type = TRACK_TYPE::ALWAYS;
        else if(s.Upper() == _T("NEVER"))
            m_track_type = TRACK_TYPE::NEVER;
    }else{
        m_track_type = TRACK_TYPE::DEFAULT;
    }
    
    s = tkz.GetNextToken();
    if(s.Len()){
        if(s.Upper() == _T("IGNORE"))
            m_should_track = false;
    }

    s = tkz.GetNextToken();
    if(s.Len()){
        if(s.Upper() == _T("MOB"))
            m_sar = true;
    }
    
    s = tkz.GetNextToken();
    if(s.Len()){
        if(s.Upper() == _T("VDM"))
            m_ownship = true;
    }

    s = tkz.GetNextToken();
    if (0 < s.Len()){
        if (s.Upper() == _T("FOLLOWER")){
            m_follower = true;
        }
    }

    s = tkz.GetNextToken();
    if(s.Len()){
        if(s.Upper() == _T("PERSIST"))
            m_should_persist = true;
    }

    s = tkz.GetNextToken();
    SetShipName( s.Upper().ToStdString());
}

void MMSIProperties::LoadFromJSON( const string& document ){
    wxJSONReader parser(wxJSONREADER_STRICT);
    wxJSONValue object;
    
    const int error_count = parser.Parse( document, &object );
    if( error_count ){
        wxLogMessage("error reading MMSI properties from json!\n%s",  document );
    }else if(object.IsObject()){
        const string new_name = object.Get("shipname", "").AsString().ToStdString();
        if( ! new_name.empty()){
            m_ship_name = new_name;
        }
    }
}

string MMSIProperties::ToCacheLine() const {
    wxJSONValue object;
    object["mmsi"] = m_mmsi;
    object["shipname"] = m_ship_name;
    
    wxString output;
    wxJSONWriter writer( wxJSONWRITER_NONE );
    writer.Write( object, output );
    
    return output.ToStdString();
}

std::string MMSIProperties::Serialize( void )
{
    wxString serialized;
    wxString s;
    
    serialized.Printf(_T("%d"), m_mmsi);
    serialized << _T(";");
    
    if( ShouldForceDisplay() ){
        serialized << _T("always");
    }else if( ! ShouldDisplay() ){
        serialized << _T("never");
    }
    serialized << _T(";");
    
    if(ShouldIgnore()){
        serialized << _T("ignore");
    }
    serialized << _T(";");

    if(IsSAR()){
        serialized << _T("MOB");
    }
    serialized << _T(";");
    
    if(IsOwnship()){
        serialized << _T("VDM");
    }    
    serialized << _T(";");

    if (IsFollower()){
        serialized << _T("Follower");
    }
    serialized << _T(";");
    
    if(ShouldPersist()){
        serialized << _T("PERSIST");
    }
    serialized << _T(";");

    serialized << m_ship_name;
    serialized << _T(";");
    return serialized.ToStdString();
}
  
AIS::mmsi_t MMSIProperties::GetId() const {
    return m_mmsi;
}

string MMSIProperties::GetTrackTypeText(){
    switch( m_track_type ){
        case TRACK_TYPE::DEFAULT:
            return string("default");
        case TRACK_TYPE::ALWAYS:
            return string("always");
        case TRACK_TYPE::NEVER:
            return string("never");
    }
    return string("");
}

MMSIProperties::operator bool() const {
    return (ERROR_MMSI != m_mmsi);
}
string MMSIProperties::GetShipName() const {
    return m_ship_name;
}

void MMSIProperties::SetShipName( const string new_name ){
    if( ! new_name.empty()){
        m_ship_name = new_name;
    }
}

bool MMSIProperties::ShouldDisplay() const {
    return (TRACK_TYPE::NEVER != m_track_type);
}

bool MMSIProperties::ShouldForceDisplay() const{
    return (TRACK_TYPE::ALWAYS == m_track_type );
}

void MMSIProperties::SetDisplayDefault(){
    m_track_type = TRACK_TYPE::DEFAULT;
}

void MMSIProperties::SetDisplayForce(){
    m_track_type = TRACK_TYPE::ALWAYS;
}

void MMSIProperties::SetDisplayNever(){
    m_track_type = TRACK_TYPE::NEVER;
}

bool MMSIProperties::IsFollower() const{
    return m_follower;
}

void MMSIProperties::SetAsFollower( bool follow){
    m_follower = follow; 
}

bool MMSIProperties::ShouldIgnore() const {
    return (!m_should_track);
}

void MMSIProperties::ShouldIgnore( bool ignore ){ 
    m_should_track = ! ignore;
}

bool MMSIProperties::ShouldPersist() const {
    return m_should_persist;
}
  
void MMSIProperties::ShouldPersist( bool persist ){
    m_should_persist = persist;
}

bool MMSIProperties::ShouldTrack() const { 
    return m_should_track;
}

void MMSIProperties::ShouldTrack( bool track){
    m_should_track = track;
}


bool MMSIProperties::IsOwnship() const {
    return m_ownship;
}

void MMSIProperties::SetAsOwnship( bool ownship){
    m_ownship = ownship;
}

bool MMSIProperties::IsSAR() const {
    return m_sar;
}

void MMSIProperties::SetAsSAR( bool sar ){
    m_sar = sar;
}

void MMSIProperties::ValidateFlags(){
    if( MMSIProperties::ERROR_MMSI != m_mmsi ){
        m_should_track=true;
        //m_should_persist=true;
    }
}

