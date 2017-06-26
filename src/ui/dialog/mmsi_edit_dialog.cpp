/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Options Dialog
 * Author:   David Register
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
 **************************************************************************/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "options.h"

#include "ui/dialog/mmsi_edit_dialog.h"

#include "ais/ais_decoder.h"
#include "ais/ais_target.h"
#include "ais/mmsi_properties.h"
#include "ais/mmsi_tracker.h"

using AIS::mmsi_t;
using AIS::MMSIProperties;
using AIS::MMSITracker;

extern AIS_Decoder* g_pAIS;

///////////////////////////////////////////////////////////////////////////////
/// Class MMSIEditDialog
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MMSIEditDialog, wxDialog)
EVT_BUTTON(ID_MMSIEDIT_CANCEL, MMSIEditDialog::OnMMSIEditCancelClick)
EVT_BUTTON(ID_MMSIEDIT_OK, MMSIEditDialog::OnMMSIEditOKClick)
END_EVENT_TABLE()

MMSIEditDialog::MMSIEditDialog(AIS::MMSIProperties* props, wxWindow* parent,
                               wxWindowID id, const wxString& caption,
                               const wxPoint& pos, const wxSize& size,
                               long style)
    : wxDialog(parent, id, caption, pos, size,
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      m_props(props) {
  CreateControls();
  GetSizer()->SetSizeHints(this);
  Centre();
}

MMSIEditDialog::~MMSIEditDialog(void) { delete m_MMSICtl; }

void MMSIEditDialog::CreateControls(void) {
  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(mainSizer);

  wxStaticBox* mmsiBox =
      new wxStaticBox(this, wxID_ANY, _("MMSI Extended Properties"));

  wxStaticBoxSizer* mmsiSizer = new wxStaticBoxSizer(mmsiBox, wxVERTICAL);
  mainSizer->Add(mmsiSizer, 0, wxEXPAND | wxALL, 5);

  mmsiSizer->Add(new wxStaticText(this, wxID_STATIC, _("MMSI")), 0,
                 wxALIGN_LEFT | wxLEFT | wxRIGHT | wxTOP, 5);

  m_MMSICtl = new wxTextCtrl(this, ID_MMSI_CTL, wxEmptyString,
                             wxDefaultPosition, wxSize(180, -1), 0);
  mmsiSizer->Add(m_MMSICtl, 0,
                 wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

  wxStaticBoxSizer* trackSizer = new wxStaticBoxSizer(
      new wxStaticBox(this, wxID_ANY, _("Tracking")), wxVERTICAL);

  wxGridSizer* gridSizer = new wxGridSizer(0, 3, 0, 0);

  m_rbTypeTrackDefault =
      new wxRadioButton(this, wxID_ANY, _("Default tracking"),
                        wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  m_rbTypeTrackDefault->SetValue(TRUE);
  gridSizer->Add(m_rbTypeTrackDefault, 0, wxALL, 5);

  m_rbTypeTrackAlways = new wxRadioButton(this, wxID_ANY, _("Always track"));
  gridSizer->Add(m_rbTypeTrackAlways, 0, wxALL, 5);

  m_rbTypeTrackNever = new wxRadioButton(this, wxID_ANY, _(" Never track"));
  gridSizer->Add(m_rbTypeTrackNever, 0, wxALL, 5);

  m_cbTrackPersist = new wxCheckBox(this, wxID_ANY, _("Persistent"));
  gridSizer->Add(m_cbTrackPersist, 0, wxALL, 5);

  trackSizer->Add(gridSizer, 0, wxEXPAND, 0);
  mmsiSizer->Add(trackSizer, 0, wxEXPAND, 0);

  m_IgnoreButton = new wxCheckBox(this, wxID_ANY, _("Ignore this MMSI"));
  mmsiSizer->Add(m_IgnoreButton, 0, wxEXPAND, 5);
  
  m_MOBButton = new wxCheckBox(this, wxID_ANY,
                               _("Handle this MMSI as SART/PLB(AIS) MOB."));
  mmsiSizer->Add(m_MOBButton, 0, wxEXPAND, 5);

  m_VDMButton = new wxCheckBox(this, wxID_ANY, _("Convert AIVDM to AIVDO for this MMSI"));
  mmsiSizer->Add(m_VDMButton, 0, wxEXPAND, 5);

  m_FollowerButton = new wxCheckBox(this, wxID_ANY, _("This MMSI is my Follower - No CPA Alert"));
  mmsiSizer->Add(m_FollowerButton, 0, wxEXPAND, 5);

  wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
  mainSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxALL, 5);

  m_CancelButton = new wxButton(this, ID_MMSIEDIT_CANCEL, _("Cancel"));
  btnSizer->Add(m_CancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_OKButton = new wxButton(this, ID_MMSIEDIT_OK, _("OK"));
  btnSizer->Add(m_OKButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  m_OKButton->SetDefault();

    //  Set initial values...
    if( m_props ){
        wxString sMMSI;
        if ( m_props->GetId() > 0){
            sMMSI.Printf(_T("%d"), m_props->GetId());
        }else{
            sMMSI = _T("");
            m_MMSICtl->AppendText(sMMSI);
        }

        if( m_props->ShouldForceDisplay()) {
            m_rbTypeTrackAlways->SetValue(TRUE);
        }else{
            m_rbTypeTrackNever->SetValue(TRUE);
        }

        m_cbTrackPersist->SetValue(m_props->ShouldPersist());
        m_IgnoreButton->SetValue( m_props->ShouldIgnore());
        m_MOBButton->SetValue(m_props->IsSAR());
        m_VDMButton->SetValue(m_props->IsOwnship());
        m_FollowerButton->SetValue(m_props->IsFollower());
    }
    
    SetColorScheme(GLOBAL_COLOR_SCHEME_RGB);
}

AIS::MMSIProperties* MMSIEditDialog::GetProperties(){
    return m_props;
}

void MMSIEditDialog::SetColorScheme(ColorScheme cs) { DimeControl(this); }

void MMSIEditDialog::OnMMSIEditCancelClick(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}


void MMSIEditDialog::OnMMSIEditOKClick(wxCommandEvent& event) {
  extern AIS::MMSITracker g_MMSIProps;
    long nmmsi;
    m_MMSICtl->GetValue().ToLong(&nmmsi);
    if ( m_props && (nmmsi == m_props->GetId()) ){
        delete m_props;
        m_props = nullptr;
    }
    if( !m_props ){
        m_props = new MMSIProperties( nmmsi);
    }
    
    if (m_MMSICtl->GetValue().Length() != 9)
    {
        if (wxID_CANCEL == OCPNMessageBox(this,
            _("An MMSI Id is generally a number of nine digits.\nPlease check your entries and cancel if necessary."),
            _("OpenCPN Info"),
            wxOK | wxCANCEL))
        {
            return;
        }
    }
    
    if (m_rbTypeTrackDefault->GetValue())
        m_props->SetDisplayDefault();
    else if (m_rbTypeTrackAlways->GetValue())
        m_props->SetDisplayForce();
    else
        m_props->SetDisplayNever();

    m_props->ShouldTrack( !m_IgnoreButton->GetValue());
    m_props->SetAsSAR( m_MOBButton->GetValue());
    m_props->SetAsOwnship( m_VDMButton->GetValue());
    m_props->SetAsFollower( m_FollowerButton->GetValue());
    m_props->ShouldPersist( m_cbTrackPersist->GetValue());
    if (m_props->GetShipName().empty()) {
        AIS_Target_Data *proptarget = g_pAIS->Get_Target_Data_From_MMSI(nmmsi);
        if (proptarget) {
            m_props->SetShipName( proptarget->GetFullName().ToStdString());
        }
    }
  
    EndModal(wxID_OK);
}

void MMSIEditDialog::OnCtlUpdated(wxCommandEvent& event) {}
