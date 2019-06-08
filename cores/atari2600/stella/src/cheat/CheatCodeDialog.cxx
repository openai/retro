//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: CheatCodeDialog.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <sstream>

#include "bspf.hxx"

#include "Cheat.hxx"
#include "CheatManager.hxx"
#include "CheckListWidget.hxx"
#include "DialogContainer.hxx"
#include "Dialog.hxx"
#include "InputTextDialog.hxx"
#include "OSystem.hxx"
#include "Props.hxx"
#include "StringList.hxx"
#include "Widget.hxx"

#include "CheatCodeDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CheatCodeDialog::CheatCodeDialog(OSystem* osystem, DialogContainer* parent,
                                 const GUI::Font& font)
  : Dialog(osystem, parent, 0, 0, 0, 0)
{
  const int lineHeight   = font.getLineHeight(),
            fontWidth    = font.getMaxCharWidth(),
            buttonWidth  = font.getStringWidth("Defaults") + 20,
            buttonHeight = font.getLineHeight() + 4;
  int xpos, ypos;
  WidgetArray wid;
  ButtonWidget* b;

  // Set real dimensions
  _w = 46 * fontWidth + 10;
  _h = 11 * (lineHeight + 4) + 10;

  // List of cheats, with checkboxes to enable/disable
  xpos = 10;  ypos = 10;
  myCheatList =
    new CheckListWidget(this, font, xpos, ypos, _w - buttonWidth - 25,
                        _h - 2*buttonHeight - 10);
  myCheatList->setEditable(false);
  wid.push_back(myCheatList);

  xpos += myCheatList->getWidth() + 5;  ypos = 15;

  b = new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                       "Add", kAddCheatCmd);
  wid.push_back(b);
  ypos += lineHeight + 10;

  myEditButton =
    new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                     "Edit", kEditCheatCmd);
  wid.push_back(myEditButton);
  ypos += lineHeight + 10;

  myRemoveButton =
    new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                     "Remove", kRemCheatCmd);
  wid.push_back(myRemoveButton);
  ypos += lineHeight + 10;

  b = new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                       "One shot", kAddOneShotCmd);
  wid.push_back(b);

  // Inputbox which will pop up when adding/editing a cheat
  StringList labels;
  labels.push_back("Name: ");
  labels.push_back("Code: ");
  myCheatInput = new InputTextDialog(this, font, labels);
  myCheatInput->setTarget(this);

  addToFocusList(wid);

  // Add OK and Cancel buttons
  wid.clear();
  addOKCancelBGroup(wid, font);
  addBGroupToFocusList(wid);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CheatCodeDialog::~CheatCodeDialog()
{
  delete myCheatInput;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::loadConfig()
{
  // Load items from CheatManager
  // Note that the items are always in the same order/number as given in
  // the CheatManager, so the arrays will be one-to-one
  StringList l;
  BoolArray b;

  const CheatList& list = instance().cheat().list();
  for(unsigned int i = 0; i < list.size(); ++i)
  {
    l.push_back(list[i]->name());
    b.push_back(bool(list[i]->enabled()));
  }
  myCheatList->setList(l, b);

  // Redraw the list, auto-selecting the first item if possible
  myCheatList->setSelected(l.size() > 0 ? 0 : -1);

  bool enabled = (list.size() > 0);
  myEditButton->setEnabled(enabled);
  myRemoveButton->setEnabled(enabled);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::saveConfig()
{
  // Inspect checkboxes for enable/disable codes
  const CheatList& list = instance().cheat().list();
  for(unsigned int i = 0; i < myCheatList->getList().size(); ++i)
  {
    if(myCheatList->getState(i))
      list[i]->enable();
    else
      list[i]->disable();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::addCheat()
{
  myCheatInput->show();    // Center input dialog over entire screen
  myCheatInput->setText("", 0);
  myCheatInput->setText("", 1);
  myCheatInput->setTitle("");
  myCheatInput->setFocus(0);
  myCheatInput->setEmitSignal(kCheatAdded);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::editCheat()
{
  int idx = myCheatList->getSelected();
  if(idx < 0)
    return;

  const CheatList& list = instance().cheat().list();
  const string& name = list[idx]->name();
  const string& code = list[idx]->code();

  myCheatInput->show();    // Center input dialog over entire screen
  myCheatInput->setText(name, 0);
  myCheatInput->setText(code, 1);
  myCheatInput->setTitle("");
  myCheatInput->setFocus(1);
  myCheatInput->setEmitSignal(kCheatEdited);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::removeCheat()
{
  instance().cheat().remove(myCheatList->getSelected());
  loadConfig();  // reload the cheat list
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::addOneShotCheat()
{
  myCheatInput->show();    // Center input dialog over entire screen
  myCheatInput->setText("One-shot cheat", 0);
  myCheatInput->setText("", 1);
  myCheatInput->setTitle("");
  myCheatInput->setFocus(1);
  myCheatInput->setEmitSignal(kOneShotCheatAdded);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheatCodeDialog::handleCommand(CommandSender* sender, int cmd,
                                    int data, int id)
{
  switch(cmd)
  {
    case kOKCmd:
      saveConfig();
      close();
      break;

    case kCloseCmd:
      close();
      break;

    case ListWidget::kDoubleClickedCmd:
      editCheat();
      break;

    case kAddCheatCmd:
      addCheat();
      break;

    case kEditCheatCmd:
      editCheat();
      break;

    case kCheatAdded:
    {
      const string& name = myCheatInput->getResult(0);
      const string& code = myCheatInput->getResult(1);
      if(instance().cheat().isValidCode(code))
      {
        myCheatInput->close();
        instance().cheat().add(name, code);
        loadConfig();  // show changes onscreen
      }
      else
        myCheatInput->setTitle("Invalid code");
      break;
    }

    case kCheatEdited:
    {
      const string& name = myCheatInput->getResult(0);
      const string& code = myCheatInput->getResult(1);
      bool enable = myCheatList->getSelectedState();
      int idx = myCheatList->getSelected();
      if(instance().cheat().isValidCode(code))
      {
        myCheatInput->close();
        instance().cheat().add(name, code, enable, idx);
        loadConfig();  // show changes onscreen
      }
      else
        myCheatInput->setTitle("Invalid code");
      break;
    }

    case kRemCheatCmd:
      removeCheat();
      break;

    case kAddOneShotCmd:
      addOneShotCheat();
      break;

    case kOneShotCheatAdded:
    {
      const string& name = myCheatInput->getResult(0);
      const string& code = myCheatInput->getResult(1);
      if(instance().cheat().isValidCode(code))
      {
        myCheatInput->close();
        instance().cheat().addOneShot(name, code);
      }
      else
        myCheatInput->setTitle("Invalid code");
      break;
    }

    default:
      Dialog::handleCommand(sender, cmd, data, 0);
      break;
  }
}
