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

#ifndef __MMSI_PROPERTIES_H__
#define __MMSI_PROPERTIES_H__

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "ais.h"

namespace AIS {

    

// according to the AIS spec, MMSI are 30 bits long
typedef int32_t mmsi_t;

class MMSIProperties
{
public:
    
    static const mmsi_t ERROR_MMSI = 0;
    MMSIProperties( mmsi_t mmsi = ERROR_MMSI );
    
    // virtual ~MMSIProperties();    
    void LoadFromOther( const MMSIProperties& other);
    void LoadFromConfig( const std::string& ini_serialized_data );
    void LoadFromJSON( const std::string& serialized_document );
    
    std::string GetTrackTypeText();
        
    std::string Serialize();

    mmsi_t GetId() const;
    operator bool() const;

    std::string GetShipName() const;
    
    void SetShipName( const std::string new_name );
    
    bool ShouldDisplay() const;
    bool ShouldForceDisplay() const;
    bool ShouldIgnore() const;
    bool ShouldPersist() const;
    bool ShouldTrack() const;
    bool IsSAR() const;
    bool IsOwnship() const;
    bool IsFollower() const;
    
    void SetDisplayDefault();
    void SetDisplayForce();
    void SetDisplayNever();

    void ShouldIgnore( bool ignore );
    void ShouldTrack( bool track);
    void ShouldPersist( bool persist);
    
    void SetAsSAR( bool sar );
    void SetAsOwnship( bool ownship);
    void SetAsFollower( bool follow);
public: 
    std::string ToCacheLine() const;
    
private:
    enum class TRACK_TYPE {
        DEFAULT=0,      // i.e. inherit global settings
        ALWAYS,         // override:  always display
        NEVER,          // override: never display
    };
    
    // not const.  Will set some internal flags.
    void ValidateFlags();

    /* const*/ mmsi_t        m_mmsi;
    std::string   m_ship_name;
    
    TRACK_TYPE    m_track_type;
    bool          m_should_track;
    bool          m_should_persist;
    
    // treat this MMSI as a SART/PLB(AIS) MOB
    bool          m_sar;
    
    // convert AIVDEM TO AIVDO for this MMSI
    // == is ownship? 
    bool          m_ownship;
    
    // this MMSI is my follower : suppress CPA alerts from this id
    bool          m_follower;
    

}; // class MMSIProperties

}; // namespace AIS 

#endif // __MMSI_PROPERTIES_H__
