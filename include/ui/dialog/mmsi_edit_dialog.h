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

#ifndef __MMSI_EDIT_DIALOG_H__
#define __MMSI_EDIT_DIALOG_H__

#include "ais/mmsi_properties.h"



#define ID_MMSIEDIT_OK 8191
#define ID_MMSIEDIT_CANCEL 8192
#define ID_MMSI_CTL 8193
#define ID_DEF_MENU_MMSI_EDIT 8194
#define ID_DEF_MENU_MMSI_DELETE 8195

class MMSIEditDialog : private Uncopyable, public wxDialog {
 public:
  explicit MMSIEditDialog(AIS::MMSIProperties *props, wxWindow *parent,
                          wxWindowID id = wxID_ANY,
                          const wxString &caption = wxEmptyString,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize, long style = 0);
  ~MMSIEditDialog(void);

  AIS::MMSIProperties* GetProperties();
  void SetColorScheme(ColorScheme cs);
  void CreateControls(void);
  void OnMMSIEditCancelClick(wxCommandEvent &event);
  void OnMMSIEditOKClick(wxCommandEvent &event);
  void OnCtlUpdated(wxCommandEvent &event);
  
protected:
  AIS::MMSIProperties *m_props;
  wxTextCtrl *m_MMSICtl, m_ShipNameCtl; //Has ToDo take away?
  wxRadioButton *m_rbTypeTrackDefault, *m_rbTypeTrackAlways;
  wxRadioButton *m_rbTypeTrackNever;
  wxCheckBox *m_cbTrackPersist, *m_IgnoreButton, *m_MOBButton, *m_VDMButton, *m_FollowerButton;
  wxButton *m_CancelButton, *m_OKButton;

  DECLARE_EVENT_TABLE()
};

class MMSI_Props_Panel : private Uncopyable, public wxPanel {
 public:
  explicit MMSI_Props_Panel(wxWindow *parent);
  ~MMSI_Props_Panel();

  void OnNewButton(wxCommandEvent &event);
  void SetColorScheme(ColorScheme cs);
  void UpdateMMSIList(void);

  MMSIListCtrl *m_pListCtrlMMSI;
  wxButton *m_pButtonNew;

 private:
  wxWindow *m_pparent;
};

#endif // __MMSI_EDIT_DIALOG_H__
