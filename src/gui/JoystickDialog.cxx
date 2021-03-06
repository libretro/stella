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
#include "Font.hxx"
#include "EditTextWidget.hxx"
#include "StringListWidget.hxx"
#include "Variant.hxx"
#include "JoystickDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JoystickDialog::JoystickDialog(GuiObject* boss, const GUI::Font& font,
                               int max_w, int max_h)
  : Dialog(boss->instance(), boss->parent(), font, "Joystick database", 0, 0, max_w, max_h)
{
  int xpos, ypos;
  WidgetArray wid;

  int buttonWidth = font.getStringWidth("Remove ") + 20,
      buttonHeight = font.getLineHeight() + 4;

  // Joystick list
  xpos = 10;  ypos = 10 + _th;
  int w = _w - 2 * xpos;
  int h = _h - buttonHeight - ypos - 20;
  myJoyList = new StringListWidget(this, font, xpos, ypos, w, h);
  myJoyList->setEditable(false);
  wid.push_back(myJoyList);

  // Joystick ID
  ypos = _h - buttonHeight - 10;
  StaticTextWidget* t = new StaticTextWidget(this, font, xpos, ypos+2, "Joystick ID ");
  xpos += t->getWidth() + 4;
  myJoyText = new EditTextWidget(this, font, xpos, ypos,
      font.getStringWidth("Unplugged")+8, font.getLineHeight(), "");
  myJoyText->setEditable(false);

  // Add buttons at bottom
  xpos = _w - buttonWidth - 10;
  myCloseBtn = new ButtonWidget(this, font, xpos, ypos,
      buttonWidth, buttonHeight, "Close", GuiObject::kCloseCmd);
  addOKWidget(myCloseBtn);  addCancelWidget(myCloseBtn);

  buttonWidth = font.getStringWidth("Remove") + 20;
  xpos -= buttonWidth + 8;
  myRemoveBtn = new ButtonWidget(this, font, xpos, ypos,
      buttonWidth, buttonHeight, "Remove", kRemoveCmd);
  myRemoveBtn->clearFlags(Widget::FLAG_ENABLED);

  // Now we can finally add the widgets to the focus list
  wid.push_back(myRemoveBtn);
  wid.push_back(myCloseBtn);
  addToFocusList(wid);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void JoystickDialog::loadConfig()
{
  myJoyIDs.clear();

  StringList sticks;
  for(const auto& i: instance().eventHandler().physicalJoystickDatabase())
  {
    sticks.push_back(i.first);
    myJoyIDs.push_back(i.second.toInt());
  }
  myJoyList->setList(sticks);
  myJoyList->setSelected(0);
  if(sticks.size() == 0)
  {
    myRemoveBtn->setEnabled(false);
    myJoyText->setText("");
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void JoystickDialog::handleCommand(CommandSender* sender, int cmd, int data, int id)
{
  switch(cmd)
  {
    case GuiObject::kOKCmd:
      close();
      break;

    case kRemoveCmd:
      instance().eventHandler().removePhysicalJoystickFromDatabase(
          myJoyList->getSelectedString());
      loadConfig();
      break;

    case ListWidget::kSelectionChangedCmd:
      if(myJoyIDs[data] >= 0)
      {
        myRemoveBtn->setEnabled(false);
        ostringstream buf;
        buf << "J" << myJoyIDs[data];
        myJoyText->setText(buf.str());
      }
      else
      {
        myRemoveBtn->setEnabled(true);
        myJoyText->setText("Unplugged");
      }
      break;

    default:
      Dialog::handleCommand(sender, cmd, data, id);
      break;
  }
}
