/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   keyboard_event.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the keyboard_event class.
 * 
 * 
 */

#include "keyboard_event.h"

using namespace input;

std::string keyboard_event::Key_symbol[keyboard_event::NBR_KEYS] =
{
    "unknown",
    "backspace",
    "tab",
    "clear",
    "return",
    "pause",
    "escape",
    "space",
    "!",
    "\"",
    "hash", // FIXME: Find correct symbol
    "$",
    "&",
    "'",
    "(",
    ")",
    "*",
    "+",
    ",",
    "-",
    ",",
    "/",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    ":",
    ";",
    "<",
    "=",
    ">",
    "?",
    "@",
    "[",
    "\\",
    "]",
    "#", // FIXME
    "_",
    "`",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "world 0",
    "world 1",
    "world 2",
    "world 3",
    "world 4",
    "world 5",
    "world 6",
    "world 7",
    "world 8",
    "world 9",
    "world 10",
    "world 11",
    "world 12",
    "world 13",
    "world 14",
    "world 15",
    "world 16",
    "world 17",
    "world 18",
    "world 19",
    "world 20",
    "world 21",
    "world 22",
    "world 23",
    "world 24",
    "world 25",
    "world 26",
    "world 27",
    "world 28",
    "world 29",
    "world 30",
    "world 31",
    "world 32",
    "world 33",
    "world 34",
    "world 35",
    "world 36",
    "world 37",
    "world 38",
    "world 39",
    "world 40",
    "world 41",
    "world 42",
    "world 43",
    "world 44",
    "world 45",
    "world 46",
    "world 47",
    "world 48",
    "world 49",
    "world 50",
    "world 51",
    "world 52",
    "world 53",
    "world 54",
    "world 55",
    "world 56",
    "world 57",
    "world 58",
    "world 59",
    "world 60",
    "world 61",
    "world 62",
    "world 63",
    "world 64",
    "world 65",
    "world 66",
    "world 67",
    "world 68",
    "world 69",
    "world 70",
    "world 71",
    "world 72",
    "world 73",
    "world 74",
    "world 75",
    "world 76",
    "world 77",
    "world 78",
    "world 79",
    "world 80",
    "world 81",
    "world 82",
    "world 83",
    "world 84",
    "world 85",
    "world 86",
    "world 87",
    "world 88",
    "world 89",
    "world 90",
    "world 91",
    "world 92",
    "world 93",
    "world 94",
    "world 95",
    "delete",
    "keypad 0",
    "keypad 1",
    "keypad 2",
    "keypad 3",
    "keypad 4",
    "keypad 5",
    "keypad 6",
    "keypad 7",
    "keypad 8",
    "keypad 9",
    "keypad .",
    "keypad /",
    "keypad *",
    "keypad -",
    "keypad +",
    "keypad enter",
    "keypad =",
    "up",
    "down",
    "right",
    "left",
    "insert",
    "home",
    "end",
    "pageup",
    "pagedown",
    "f1",
    "f2",
    "f3",
    "f4",
    "f5",
    "f6",
    "f7",
    "f8",
    "f9",
    "f10",
    "f11",
    "f12",
    "f13",
    "f14",
    "f15",
    "num lock",
    "caps lock",
    "scroll lock",
    "right shift",
    "left shift",
    "right control",
    "left control",
    "right alt",
    "left alt",
    "right meta",
    "left meta",
    "left super",
    "right super",
    "mode",
    "compose",
    "help",
    "print",
    "sysreq",
    "break",
    "menu",
    "power",
    "euro"
};

keyboard_event::keyboard_event (event_type t, key_type k) : event (KEYBOARD_EVENT)
{
    Type = t;
    Key = k; 
}

const std::string & keyboard_event::key_symbol() const
{
    return Key_symbol[key()];
}