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
// $Id: MouseControl.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "Console.hxx"
#include "Control.hxx"
#include "Props.hxx"

#include "MouseControl.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
MouseControl::MouseControl(Console& console, const string& mode)
  : myProps(console.properties()),
    myLeftController(console.controller(Controller::Left)),
    myRightController(console.controller(Controller::Right)),
    myCurrentModeNum(0)
{
  if(BSPF_equalsIgnoreCase(mode, "none"))
  {
    myModeList.push_back(MouseMode("Mouse input is disabled"));
    return;
  }
  else if(!BSPF_equalsIgnoreCase(mode, "auto") && mode.length() == 2 &&
          mode[0] >= '0' && mode[0] <= '8' &&
          mode[1] >= '0' && mode[1] <= '8')
  {
    Axis xaxis = (Axis) ((int)mode[0] - '0');
    Axis yaxis = (Axis) ((int)mode[1] - '0');
    ostringstream msg;
    msg << "Mouse X-axis is ";
    Controller::Type xtype = Controller::Joystick, ytype = Controller::Joystick;
    int xid = -1, yid = -1;
    switch(xaxis)
    {
      case NoControl:
        msg << "not used";
        break;
      case Paddle0:
        xtype = Controller::Paddles;
        xid = 0;
        msg << "Paddle 0";
        break;
      case Paddle1:
        xtype = Controller::Paddles;
        xid = 1;
        msg << "Paddle 1";
        break;
      case Paddle2:
        xtype = Controller::Paddles;
        xid = 2;
        msg << "Paddle 2";
        break;
      case Paddle3:
        xtype = Controller::Paddles;
        xid = 3;
        msg << "Paddle 3";
        break;
      case Driving0:
        xtype = Controller::Driving;
        xid = 0;
        msg << "Driving 0";
        break;
      case Driving1:
        xtype = Controller::Driving;
        xid = 1;
        msg << "Driving 1";
        break;
      case MindLink0:
        xtype = Controller::MindLink;
        xid = 0;
        msg << "MindLink 0";
        break;
      case MindLink1:
        xtype = Controller::MindLink;
        xid = 1;
        msg << "MindLink 1";
        break;
      default:  break;
    }
    msg << ", Y-axis is ";
    switch(yaxis)
    {
      case NoControl:
        msg << "not used";
        break;
      case Paddle0:
        ytype = Controller::Paddles;
        yid = 0;
        msg << "Paddle 0";
        break;
      case Paddle1:
        ytype = Controller::Paddles;
        yid = 1;
        msg << "Paddle 1";
        break;
      case Paddle2:
        ytype = Controller::Paddles;
        yid = 2;
        msg << "Paddle 2";
        break;
      case Paddle3:
        ytype = Controller::Paddles;
        yid = 3;
        msg << "Paddle 3";
        break;
      case Driving0:
        ytype = Controller::Driving;
        yid = 0;
        msg << "Driving 0";
        break;
      case Driving1:
        ytype = Controller::Driving;
        yid = 1;
        msg << "Driving 1";
        break;
      case MindLink0:
        ytype = Controller::MindLink;
        yid = 0;
        msg << "MindLink 0";
        break;
      case MindLink1:
        ytype = Controller::MindLink;
        yid = 1;
        msg << "MindLink 1";
        break;
      default:  break;
    }
    myModeList.push_back(MouseMode(xtype, xid, ytype, yid, msg.str()));
  }

  // Now consider the possible modes for the mouse based on the left
  // and right controllers
  bool noswap = BSPF_equalsIgnoreCase(myProps.get(Console_SwapPorts), "NO");
  if(noswap)
  {
    addLeftControllerModes(noswap);
    addRightControllerModes(noswap);
  }
  else
  {
    addRightControllerModes(noswap);
    addLeftControllerModes(noswap);
  }

  // If the mouse isn't used at all, we still need one item in the list
  if(myModeList.size() == 0)
    myModeList.push_back(MouseMode("Mouse not used for current controllers"));

#if 0
  for(unsigned int i = 0; i < myModeList.size(); ++i)
    cerr << myModeList[i] << endl;
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
MouseControl::~MouseControl()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string& MouseControl::next()
{
  const MouseMode& mode = myModeList[myCurrentModeNum];
  myCurrentModeNum = (myCurrentModeNum + 1) % myModeList.size();

  myLeftController.setMouseControl(mode.xtype, mode.xid, mode.ytype, mode.yid);
  myRightController.setMouseControl(mode.xtype, mode.xid, mode.ytype, mode.yid);

  return mode.message;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void MouseControl::addLeftControllerModes(bool noswap)
{
  if(controllerSupportsMouse(myLeftController))
  {
    if(myLeftController.type() == Controller::Paddles)
    {
      if(noswap)  addPaddleModes(0, 1, 0, 1);
      else        addPaddleModes(2, 3, 0, 1);
    }
    else
    {
      ostringstream msg;
      msg << "Mouse is left " << myLeftController.name() << " controller";
      Controller::Type type = myLeftController.type();
      int id = noswap ? 0 : 1;
      myModeList.push_back(MouseMode(type, id, type, id, msg.str()));
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void MouseControl::addRightControllerModes(bool noswap)
{
  if(controllerSupportsMouse(myRightController))
  {
    if(myRightController.type() == Controller::Paddles)
    {
      if(noswap)  addPaddleModes(2, 3, 2, 3);
      else        addPaddleModes(0, 1, 2, 3);
    }
    else
    {
      ostringstream msg;
      msg << "Mouse is right " << myRightController.name() << " controller";
      Controller::Type type = myRightController.type();
      int id = noswap ? 1 : 0;
      myModeList.push_back(MouseMode(type, id, type, id, msg.str()));
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void MouseControl::addPaddleModes(int lport, int rport, int lname, int rname)
{
  Controller::Type type = Controller::Paddles;
  ostringstream msg;
  msg << "Mouse is Paddle " << lname << " controller";
  MouseMode mode0(type, lport, type, lport, msg.str());

  msg.str("");
  msg << "Mouse is Paddle " << rname << " controller";
  MouseMode mode1(type, rport, type, rport, msg.str());

  if(BSPF_equalsIgnoreCase(myProps.get(Controller_SwapPaddles), "NO"))
  {
    myModeList.push_back(mode0);
    myModeList.push_back(mode1);
  }
  else
  {
    myModeList.push_back(mode1);
    myModeList.push_back(mode0);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool MouseControl::controllerSupportsMouse(Controller& controller)
{
  // Test whether the controller uses the mouse at all
  // We can pass in dummy values here, since the controllers will be
  // initialized by a call to next() once the system is up and running
  return controller.setMouseControl(
      Controller::Joystick, -1, Controller::Joystick, -1);
}
