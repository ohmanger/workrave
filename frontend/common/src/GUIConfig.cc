// GUIConfig.cc
//
// Copyright (C) 2007 - 2013 Rob Caelers & Raymond Penners
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "preinclude.h"

#include "GUIConfig.hh"

#include "config/IConfigurator.hh"
#include "CoreFactory.hh"
#include "ICore.hh"
#include "IBreak.hh"

using namespace std;
using namespace workrave::config;

const string GUIConfig::CFG_KEY_BREAK_IGNORABLE    = "gui/breaks/%b/ignorable_break";
const string GUIConfig::CFG_KEY_BREAK_SKIPPABLE    = "gui/breaks/%b/skippable_break";
const string GUIConfig::CFG_KEY_BREAK_EXERCISES    = "gui/breaks/%b/exercises";
const string GUIConfig::CFG_KEY_BREAK_AUTO_NATURAL = "gui/breaks/%b/auto_natural";
const string GUIConfig::CFG_KEY_BLOCK_MODE         = "gui/breaks/block_mode";
const string GUIConfig::CFG_KEY_LOCALE             = "gui/locale";
const string GUIConfig::CFG_KEY_TRAYICON_ENABLED   = "gui/trayicon_enabled";
const string GUIConfig::CFG_KEY_CLOSEWARN_ENABLED  = "gui/closewarn_enabled";
const string GUIConfig::CFG_KEY_AUTOSTART          = "gui/autostart";

const string GUIConfig::CFG_KEY_MAIN_WINDOW               = "gui/main_window";
const string GUIConfig::CFG_KEY_MAIN_WINDOW_ALWAYS_ON_TOP = "gui/main_window/always_on_top";
const string GUIConfig::CFG_KEY_MAIN_WINDOW_START_IN_TRAY = "gui/main_window/start_in_tray";
const string GUIConfig::CFG_KEY_MAIN_WINDOW_X             = "gui/main_window/x";
const string GUIConfig::CFG_KEY_MAIN_WINDOW_Y             = "gui/main_window/y";
const string GUIConfig::CFG_KEY_MAIN_WINDOW_HEAD          = "gui/main_window/head";

const string GUIConfig::CFG_KEY_TIMERBOX = "gui/";
const string GUIConfig::CFG_KEY_TIMERBOX_CYCLE_TIME = "/cycle_time";
const string GUIConfig::CFG_KEY_TIMERBOX_ENABLED = "/enabled";
const string GUIConfig::CFG_KEY_TIMERBOX_POSITION = "/position";
const string GUIConfig::CFG_KEY_TIMERBOX_FLAGS = "/flags";
const string GUIConfig::CFG_KEY_TIMERBOX_IMMINENT = "/imminent";

const string GUIConfig::CFG_KEY_SOUND_ENABLED = "sound/enabled";
const string GUIConfig::CFG_KEY_SOUND_DEVICE = "sound/device";
const string GUIConfig::CFG_KEY_SOUND_VOLUME = "sound/volume";
const string GUIConfig::CFG_KEY_SOUND_MUTE = "sound/mute";
const string GUIConfig::CFG_KEY_SOUND_EVENT = "sound/events/";
const string GUIConfig::CFG_KEY_SOUND_EVENT_ENABLED = "_enabled";



void
GUIConfig::init()
{
  IConfigurator::Ptr config = CoreFactory::get_configurator();

  for (int i = 0; i < workrave::BREAK_ID_SIZEOF; i++)
    {
      workrave::BreakId breakId = (workrave::BreakId)i;
      
      config->set_value(break_ignorable(breakId).key(), true, CONFIG_FLAG_INITIAL);
      config->set_value(break_exercises(breakId).key(), i == workrave::BREAK_ID_REST_BREAK ? 3 : 0, CONFIG_FLAG_INITIAL);
      config->set_value(break_auto_natural(breakId).key(), false, CONFIG_FLAG_INITIAL);

      // for backward compatibility with settings of older versions, we set the default
      // default value of `skippable` to whatever `ignorable`. This works because the old
      // meaning of `ignorable` was "show postpone and skip"; the new meaning is
      // "show postpone".
      bool ignorable;
      config->get_value_with_default(break_ignorable(breakId).key(), ignorable, true);
      config->set_value(break_skippable(breakId).key(), ignorable, CONFIG_FLAG_INITIAL);

    }

  config->set_value(CFG_KEY_BLOCK_MODE, BLOCK_MODE_INPUT, CONFIG_FLAG_INITIAL);
  config->set_value(CFG_KEY_TRAYICON_ENABLED, true, CONFIG_FLAG_INITIAL);
  config->set_value(CFG_KEY_CLOSEWARN_ENABLED, true, CONFIG_FLAG_INITIAL);
  config->set_value(CFG_KEY_AUTOSTART, true, CONFIG_FLAG_INITIAL);
  config->set_value(CFG_KEY_LOCALE, "", CONFIG_FLAG_INITIAL);
}



string
GUIConfig::expand(const string &key, workrave::BreakId id)
{
  workrave::IBreak::Ptr b = CoreFactory::get_core()->get_break(id);

  string str = key;
  string::size_type pos = 0;
  string name = b->get_name();

  while ((pos = str.find("%b", pos)) != string::npos)
    {
      str.replace(pos, 2, name);
      pos++;
    }

  return str;
}

Setting<bool>
GUIConfig::break_auto_natural(workrave::BreakId break_id)
{
  return Setting<bool>(CoreFactory::get_configurator(), expand(CFG_KEY_BREAK_AUTO_NATURAL, break_id));
}

Setting<bool>
GUIConfig::break_ignorable(workrave::BreakId break_id)
{
  return Setting<bool>(CoreFactory::get_configurator(), expand(CFG_KEY_BREAK_IGNORABLE, break_id), true);
}

Setting<bool>
GUIConfig::break_skippable(workrave::BreakId break_id)
{
  return Setting<bool>(CoreFactory::get_configurator(), expand(CFG_KEY_BREAK_SKIPPABLE, break_id), true);
}

Setting<int>
GUIConfig::break_exercises(workrave::BreakId break_id)
{
  return Setting<int>(CoreFactory::get_configurator(), expand(CFG_KEY_BREAK_EXERCISES, break_id), 0);
}

Setting<int, GUIConfig::BlockMode>
GUIConfig::block_mode()
{
  return Setting<int, BlockMode>(CoreFactory::get_configurator(), CFG_KEY_BLOCK_MODE, BLOCK_MODE_INPUT);
}

Setting<std::string>
GUIConfig::locale()
{
  return Setting<std::string>(CoreFactory::get_configurator(), CFG_KEY_LOCALE, "");
}

Setting<bool>
GUIConfig::trayicon_enabled()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_TRAYICON_ENABLED, true);
}

Setting<bool>
GUIConfig::closewarn_enabled()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_CLOSEWARN_ENABLED);
}

Setting<bool>
GUIConfig::autostart_enabled()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_AUTOSTART);
}

const std::string
GUIConfig::key_main_window()
{
  return CFG_KEY_MAIN_WINDOW;
}

Setting<bool>
GUIConfig::main_window_always_on_top()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_MAIN_WINDOW_ALWAYS_ON_TOP, false);
}

Setting<bool>
GUIConfig::main_window_start_in_tray()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_MAIN_WINDOW_START_IN_TRAY, false);
}

Setting<int>
GUIConfig::main_window_x()
{
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_MAIN_WINDOW_X, 256);
}

Setting<int>
GUIConfig::main_window_y()
{
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_MAIN_WINDOW_Y, 256);
}

Setting<int>
GUIConfig::main_window_head()
{
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_MAIN_WINDOW_HEAD, 0);
}

const std::string
GUIConfig::key_timerbox(std::string box)
{
  return CFG_KEY_TIMERBOX + box;
}

Setting<int>
GUIConfig::timerbox_cycle_time(std::string box)
{
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_TIMERBOX + box + CFG_KEY_TIMERBOX_CYCLE_TIME, 10);
}

Setting<int>
GUIConfig::timerbox_slot(std::string box, workrave::BreakId break_id)
{
  workrave::IBreak::Ptr br = CoreFactory::get_core()->get_break(break_id);
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_TIMERBOX + box + "/" + br->get_name() + CFG_KEY_TIMERBOX_POSITION, (box == "applet" ? 0 : break_id));
}

Setting<int>
GUIConfig::timerbox_flags(std::string box, workrave::BreakId break_id)
{
  workrave::IBreak::Ptr br = CoreFactory::get_core()->get_break(break_id);
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_TIMERBOX + box + "/" + br->get_name() + CFG_KEY_TIMERBOX_FLAGS, 0);
}

Setting<int>
GUIConfig::timerbox_imminent(std::string box, workrave::BreakId break_id)
{
  workrave::IBreak::Ptr br = CoreFactory::get_core()->get_break(break_id);
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_TIMERBOX + box + "/" + br->get_name() + CFG_KEY_TIMERBOX_IMMINENT, 30);
}

Setting<bool>
GUIConfig::timerbox_enabled(std::string box)
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_TIMERBOX + box + CFG_KEY_TIMERBOX_ENABLED, true);
}

workrave::config::Setting<bool> GUIConfig::sound_enabled()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_SOUND_ENABLED, true);
}

workrave::config::Setting<std::string> GUIConfig::sound_device()
{
  return Setting<std::string>(CoreFactory::get_configurator(), CFG_KEY_SOUND_DEVICE, "");
}

workrave::config::Setting<int> GUIConfig::sound_volume()
{
  return Setting<int>(CoreFactory::get_configurator(), CFG_KEY_SOUND_VOLUME, 100);
}

workrave::config::Setting<bool> GUIConfig::sound_mute()
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_SOUND_MUTE, false);
}

workrave::config::Setting<bool> GUIConfig::sound_event_enabled(std::string event)
{
  return Setting<bool>(CoreFactory::get_configurator(), CFG_KEY_SOUND_EVENT + event + CFG_KEY_SOUND_EVENT_ENABLED, true);
}

workrave::config::Setting<std::string> GUIConfig::sound_event(std::string event)
{
  return Setting<std::string>(CoreFactory::get_configurator(), CFG_KEY_SOUND_EVENT + event, "");
}