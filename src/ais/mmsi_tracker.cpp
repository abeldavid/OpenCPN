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
#include <memory>
#include <string>

// wx extension
#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "ais/mmsi_tracker.h"

using std::string;
using std::shared_ptr;


using AIS::MMSIProperties;
using AIS::MMSITracker;

// global instance
MMSITracker g_MMSIProps;

MMSITracker::MMSITracker(){
    // zero is the error index
    insert( new MMSIProperties() );
    m_mmsi_properties[0]->SetShipName("--ERROR--");
}

MMSITracker::~MMSITracker(){
}
AIS::mmsi_iter_t MMSITracker::begin(){ return m_mmsi_properties.begin();}
AIS::mmsi_iter_t MMSITracker::end(){ return m_mmsi_properties.end();}

void MMSITracker::clear(){
    m_mmsi_properties.clear();
}

MMSIProperties& MMSITracker::clone( mmsi_t mmsi_old, mmsi_t mmsi_new ) {
    MMSIProperties* props_new = new MMSIProperties( mmsi_new );

    if ( contains(mmsi_old)){
        const MMSIProperties* props_old = g_MMSIProps[ mmsi_old];
        props_new->LoadFromOther( *props_old);
    }

    insert( props_new);
    return *props_new;
}

bool MMSITracker::contains( mmsi_t mmsi) const{
    return (0 < m_mmsi_properties.count( mmsi ));
}

MMSIProperties& MMSITracker::GetOrRegister( mmsi_t mmsi) {
    auto& props = m_mmsi_properties[ mmsi ];
    if( ! props){
        insert( new MMSIProperties( mmsi) );
    }
    return *operator[]( mmsi );
}

MMSIProperties* MMSITracker::operator[]( mmsi_t mmsi) {
    if( contains( mmsi )){
        return m_mmsi_properties[ mmsi ].get();
    }
    return nullptr;
}

void MMSITracker::emplace( const std::string& serialized_data ){
    // takes ownership of pointer parameters
    auto* props = new MMSIProperties();
    props->LoadFromConfig( serialized_data );
    insert( props );
}

void MMSITracker::erase( const mmsi_t key ){
    m_mmsi_properties.erase( key );
}

void MMSITracker::insert( MMSIProperties* new_props ){
    m_mmsi_properties[ new_props->GetId()] = std::shared_ptr<MMSIProperties>( new_props );
    new_props = nullptr; // for emphasis.  (not strictly necessary)
}

void MMSITracker::LoadShipNames( const string& filename ){
    m_name_file = filename;

    std::ifstream infile( m_name_file );
    if( infile ) {
        string line;
        while ( getline( infile, line ) ) {
            wxJSONReader parser(wxJSONREADER_STRICT);
            wxJSONValue object;

            const int error_count = parser.Parse( line, &object );
            if( 0 < error_count ){
                wxLogMessage("error reading MMSI properties from name-cache-file//json !\n%s",  line );
            }else if(object.IsObject()){
                const wxJSONValue& mmsi_object = object.Get("mmsi", 0);
                mmsi_t mmsi = 0;
                if( mmsi_object.IsInt() ){
                    mmsi = mmsi_object.AsInt();
                }else if( mmsi_object.IsLong()){
                    mmsi = mmsi_object.AsLong();
                }

                if( 0 < mmsi ){
                    MMSIProperties& props = GetOrRegister( mmsi );
                    m_mmsi_properties[mmsi]->LoadFromJSON( line );
                }
            }
        }
    }
    infile.close();
}

void MMSITracker::SaveAllShipNames(){
    std::ofstream outfile( m_name_file, std::ios::out | std::ios::trunc );
    if( outfile ) {
        for (auto iter = m_mmsi_properties.begin(); iter!=m_mmsi_properties.end(); ++iter){
            MMSIProperties& props = *iter->second;
            if( ! props.GetShipName().empty() ){
                const string& save_line = props.ToCacheLine();
                outfile << save_line << "\r\n";
            }
        }
    }
    outfile.close();
}

void MMSITracker::SaveSingleShipName( mmsi_t mmsi ){
    if( contains( mmsi )){
        if( ! m_mmsi_properties[mmsi]->GetShipName().empty() ){
            std::ofstream outfile( m_name_file, std::ios::out | std::ios::app );
            if( outfile ) {
                const string& save_line = m_mmsi_properties[mmsi]->ToCacheLine();
                outfile << save_line << "\r\n";
            }
            outfile.close();
        }
    }
}

size_t MMSITracker::size() const {
    return m_mmsi_properties.size();
}
