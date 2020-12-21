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
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "OSystem.hxx"
#include "EventHandler.hxx"
#include "Widget.hxx"
#include "PopUpWidget.hxx"
#include "Font.hxx"
#include "Variant.hxx"
#include "Props.hxx"
#include "PropsSet.hxx"
#include "QuadTariDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
QuadTariDialog::QuadTariDialog(GuiObject* boss, const GUI::Font& font, int max_w, int max_h,
                               Properties& properties)
  : Dialog(boss->instance(), boss->parent(), font, "QuadTari controllers", 0, 0, max_w, max_h),
    myGameProperties{properties}
{
  const int lineHeight = font.getLineHeight(),
    fontHeight = font.getFontHeight(),
    fontWidth = font.getMaxCharWidth();
  const int VBORDER = fontHeight / 2;
  const int HBORDER = fontWidth * 1.25;
  const int VGAP = fontHeight / 4;

  int xpos, ypos;
  WidgetArray wid;
  VariantList ctrls;

  xpos = HBORDER; ypos = VBORDER + _th;

  ctrls.clear();
  //VarList::push_back(ctrls, "Auto-detect", "AUTO");
  VarList::push_back(ctrls, "Joystick", "JOYSTICK");
  VarList::push_back(ctrls, "Paddles", "PADDLES");
  //VarList::push_back(ctrls, "Paddles_IAxis", "PADDLES_IAXIS");
  //VarList::push_back(ctrls, "Paddles_IAxDr", "PADDLES_IAXDR");
  //VarList::push_back(ctrls, "BoosterGrip", "BOOSTERGRIP");
  VarList::push_back(ctrls, "Driving", "DRIVING");
  //VarList::push_back(ctrls, "Keyboard", "KEYBOARD");
  //VarList::push_back(ctrls, "AmigaMouse", "AMIGAMOUSE");
  //VarList::push_back(ctrls, "AtariMouse", "ATARIMOUSE");
  //VarList::push_back(ctrls, "Trakball", "TRAKBALL");
  VarList::push_back(ctrls, "AtariVox", "ATARIVOX");
  VarList::push_back(ctrls, "SaveKey", "SAVEKEY");
  //VarList::push_back(ctrls, "Sega Genesis", "GENESIS");
  //VarList::push_back(ctrls, "KidVid", "KIDVID");
  //VarList::push_back(ctrls, "Lightgun", "LIGHTGUN");
  //VarList::push_back(ctrls, "MindLink", "MINDLINK");
  //VarList::push_back(ctrls, "QuadTari", "QUADTARI");

  int pwidth = font.getStringWidth("Joystick12"); // a bit wider looks better overall

  myLeftPortLabel = new StaticTextWidget(this, font, xpos, ypos + 1, "Left port");

  ypos += lineHeight + VGAP * 2;
  myLeft1Port = new PopUpWidget(this, font, xpos, ypos,
                               pwidth, lineHeight, ctrls, "P1 ");
  wid.push_back(myLeft1Port);

  ypos += lineHeight + VGAP * 2;
  myLeft2Port = new PopUpWidget(this, font, xpos, ypos,
                               pwidth, lineHeight, ctrls, "P2 ");
  wid.push_back(myLeft2Port);

  xpos = _w - HBORDER - myLeft1Port->getWidth(); // aligned right
  ypos = myLeftPortLabel->getTop() - 1;
  myRightPortLabel = new StaticTextWidget(this, font, xpos, ypos + 1, "Right port");

  ypos += lineHeight + VGAP * 2;
  myRight1Port = new PopUpWidget(this, font, xpos, ypos,
                                pwidth, lineHeight, ctrls, "P3 ");
  wid.push_back(myRight1Port);

  ypos += lineHeight + VGAP * 2;
  myRight2Port = new PopUpWidget(this, font, xpos, ypos,
                                pwidth, lineHeight, ctrls, "P4 ");
  wid.push_back(myRight2Port);

  addDefaultsOKCancelBGroup(wid, _font);
  addBGroupToFocusList(wid);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::show(bool enableLeft, bool enableRight)
{
  myLeftPortLabel->setEnabled(enableLeft);
  myLeft1Port->setEnabled(enableLeft);
  myLeft2Port->setEnabled(enableLeft);
  myRightPortLabel->setEnabled(enableRight);
  myRight1Port->setEnabled(enableRight);
  myRight2Port->setEnabled(enableRight);

  open();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::loadControllerProperties(const Properties& props)
{
  string controller;

  if(myLeftPortLabel->isEnabled())
  {
    controller = props.get(PropType::Controller_Left1);
    myLeft1Port->setSelected(controller, "Joystick");
    controller = props.get(PropType::Controller_Left2);
    myLeft2Port->setSelected(controller, "Joystick");
  }

  if(myRightPortLabel->isEnabled())
  {
    controller = props.get(PropType::Controller_Right1);
    myRight1Port->setSelected(controller, "Joystick");
    controller = props.get(PropType::Controller_Right2);
    myRight2Port->setSelected(controller, "Joystick");
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::loadConfig()
{
  loadControllerProperties(myGameProperties);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::saveConfig()
{
  if(myLeftPortLabel->isEnabled())
  {
    string controller = myLeft1Port->getSelectedTag().toString();
    myGameProperties.set(PropType::Controller_Left1, controller);
    controller = myLeft2Port->getSelectedTag().toString();
    myGameProperties.set(PropType::Controller_Left2, controller);
  }
  else
  {
    myGameProperties.set(PropType::Controller_Left1, "");
    myGameProperties.set(PropType::Controller_Left2, "");
  }

  if(myRightPortLabel->isEnabled())
  {
    string controller = myRight1Port->getSelectedTag().toString();
    myGameProperties.set(PropType::Controller_Right1, controller);
    controller = myRight2Port->getSelectedTag().toString();
    myGameProperties.set(PropType::Controller_Right2, controller);
  }
  else
  {
    myGameProperties.set(PropType::Controller_Right1, "");
    myGameProperties.set(PropType::Controller_Right2, "");
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::setDefaults()
{
  // Load the default properties
  const string& md5 = myGameProperties.get(PropType::Cart_MD5);
  Properties defaultProperties;

  instance().propSet().getMD5(md5, defaultProperties, true);
  loadControllerProperties(defaultProperties);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void QuadTariDialog::handleCommand(CommandSender* sender, int cmd, int data, int id)
{
  switch(cmd)
  {
    case GuiObject::kOKCmd:
      saveConfig();
      close();
      break;

    case GuiObject::kDefaultsCmd:
      setDefaults();
      break;

    default:
      Dialog::handleCommand(sender, cmd, data, id);
      break;
  }
}
