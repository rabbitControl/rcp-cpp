/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_V10_TYPES_H
#define RCP_V10_TYPES_H

#include <string>


#ifdef __cplusplus
namespace rcp {
namespace v10 {
#endif

const char Terminator = 0x80;
const char TerminatorMask = 0x7F;
const char OptionsDefault = 0x30; //48;
const std::string language_string_any("any");


enum array_options_t {
    ARRAY_OPTIONS_DEFAULT = 48
};
static bool _is_defined_array_options_t(array_options_t v);

enum boolean_options_t {
    BOOLEAN_OPTIONS_DEFAULT = 48
};

enum button_widget_options_t {
    BUTTON_WIDGET_OPTIONS_BUTTON_LABEL = 86,
    BUTTON_WIDGET_OPTIONS_TRIGGER_ON_UP = 87
};

enum checkbox_widget_options_t {
    CHECKBOX_WIDGET_OPTIONS_INDETERMINATE = 86
};

enum customtype_options_t {
    CUSTOMTYPE_OPTIONS_DEFAULT = 48,
    CUSTOMTYPE_OPTIONS_TYPEID = 49,
    CUSTOMTYPE_OPTIONS_CONFIG = 50
};

enum customwidget_options_t {
    CUSTOMWIDGET_OPTIONS_WIDGETID = 86,
    CUSTOMWIDGET_OPTIONS_CONFIG = 87
};

enum datatype_t {
    DATATYPE_CUSTOMTYPE = 1,
    DATATYPE_BOOLEAN = 16,
    DATATYPE_INT8 = 17,
    DATATYPE_UINT8 = 18,
    DATATYPE_INT16 = 19,
    DATATYPE_UINT16 = 20,
    DATATYPE_INT32 = 21,
    DATATYPE_UINT32 = 22,
    DATATYPE_INT64 = 23,
    DATATYPE_UINT64 = 24,
    DATATYPE_FLOAT32 = 25,
    DATATYPE_FLOAT64 = 26,
    DATATYPE_VECTOR2I32 = 27,
    DATATYPE_VECTOR2F32 = 28,
    DATATYPE_VECTOR3I32 = 29,
    DATATYPE_VECTOR3F32 = 30,
    DATATYPE_VECTOR4I32 = 31,
    DATATYPE_VECTOR4F32 = 32,
    DATATYPE_STRING = 33,
    DATATYPE_RGB = 34,
    DATATYPE_RGBA = 35,
    DATATYPE_RGB_FLOAT = 36,
    DATATYPE_RGBA_FLOAT = 37,
    DATATYPE_ENUM = 38,
    DATATYPE_ARRAY = 39,
    DATATYPE_BANG = 40,
    DATATYPE_GROUP = 41,
    DATATYPE_URI = 42,
    DATATYPE_IPV4 = 43,
    DATATYPE_IPV6 = 44,
    DATATYPE_RANGE = 45,
    DATATYPE_IMAGE = 46,
    DATATYPE_MAX_
};

enum dial_widget_options_t {
    DIAL_WIDGET_OPTIONS_PRECISION = 86,
    DIAL_WIDGET_OPTIONS_STEPSIZE_MULTIPLIER = 87,
    DIAL_WIDGET_OPTIONS_CYCLIC = 88,
    DIAL_WIDGET_OPTIONS_NAN_MEANING = 89
};

enum enum_options_t {
    ENUM_OPTIONS_DEFAULT = 48,
    ENUM_OPTIONS_ENTRIES = 49,
    ENUM_OPTIONS_MINIMUM_SELECTION_COUNT = 50,
    ENUM_OPTIONS_MAXIMUM_SELECTION_COUNT = 51
};

enum image_options_t {
    IMAGE_OPTIONS_DEFAULT = 48
};

enum image_widget_options_t {
    IMAGE_WIDGET_OPTIONS_OVERLAY_TEXT = 86
};

enum infodata_options_t {
    INFODATA_OPTIONS_APPLICATIONID = 26,
    INFODATA_OPTIONS_APPLICATIONVERSION = 27
};

enum ipv4_options_t {
    IPV4_OPTIONS_DEFAULT = 48
};

enum ipv6_options_t {
    IPV6_OPTIONS_DEFAULT = 48
};

enum number_options_t {
    NUMBER_OPTIONS_DEFAULT = 48,
    NUMBER_OPTIONS_MINIMUM = 49,
    NUMBER_OPTIONS_MAXIMUM = 50,
    NUMBER_OPTIONS_STEPSIZE = 51,
    NUMBER_OPTIONS_UNIT = 52
};

enum numberbox_widget_options_t {
    NUMBERBOX_WIDGET_OPTIONS_PRECISION = 86,
    NUMBERBOX_WIDGET_OPTIONS_STEPSIZE_MULTIPLIER = 87,
    NUMBERBOX_WIDGET_OPTIONS_CYCLIC = 88,
    NUMBERBOX_WIDGET_OPTIONS_NAN_MEANING = 89
};

enum packet_types_t {
    PACKET_TYPES_INVALID = 6,
    PACKET_TYPES_INFO = 7,
    PACKET_TYPES_INITIALIZE = 8,
    PACKET_TYPES_UPDATE = 9,
    PACKET_TYPES_UPDATEVALUE = 10,
    PACKET_TYPES_REMOVE = 11
};

enum parameter_options_t {
    PARAMETER_OPTIONS_VALUE = 32,
    PARAMETER_OPTIONS_LABEL = 33,
    PARAMETER_OPTIONS_DESCRIPTION = 34,
    PARAMETER_OPTIONS_TAGS = 35,
    PARAMETER_OPTIONS_ORDER = 36,
    PARAMETER_OPTIONS_PARENTID = 37,
    PARAMETER_OPTIONS_WIDGET = 38,
    PARAMETER_OPTIONS_USERDATA = 39,
    PARAMETER_OPTIONS_USERID = 40,
    PARAMETER_OPTIONS_READONLY = 41,
    PARAMETER_OPTIONS_ENABLED = 42
};

enum press_widget_options_t {
    PRESS_WIDGET_OPTIONS_PRESS_LABEL_ON = 86,
    PRESS_WIDGET_OPTIONS_PRESS_LABEL_OFF = 87
};

enum range_options_t {
    RANGE_OPTIONS_DEFAULT = 48
};

enum range_widget_options_t {
    RANGE_WIDGET_OPTIONS_PRECISION = 86,
    RANGE_WIDGET_OPTIONS_STEPSIZE_MULTIPLIER = 87,
    RANGE_WIDGET_OPTIONS_NAN_MEANING = 88
};

enum rgb_float_options_t {
    RGB_FLOAT_OPTIONS_DEFAULT = 48
};

enum rgb_options_t {
    RGB_OPTIONS_DEFAULT = 48
};

enum rgba_float_options_t {
    RGBA_FLOAT_OPTIONS_DEFAULT = 48
};

enum rgba_options_t {
    RGBA_OPTIONS_DEFAULT = 48
};

enum slider_widget_options_t {
    SLIDER_WIDGET_OPTIONS_PRECISION = 86,
    SLIDER_WIDGET_OPTIONS_STEPSIZE_MULTIPLIER = 87,
    SLIDER_WIDGET_OPTIONS_HORIZONTAL = 88,
    SLIDER_WIDGET_OPTIONS_NAN_MEANING = 89,
    SLIDER_WIDGET_OPTIONS_TRACKFILL_MODE = 90
};

enum string_options_t {
    STRING_OPTIONS_DEFAULT = 48,
    STRING_OPTIONS_REGULAR_EXPRESSION = 49
};

enum switch_widget_options_t {
    SWITCH_WIDGET_OPTIONS_SWITCH_LABEL_ON = 86,
    SWITCH_WIDGET_OPTIONS_SWITCH_LABEL_OFF = 87
};

enum textbox_widget_options_t {
    TEXTBOX_WIDGET_OPTIONS_MULTILINE = 86,
    TEXTBOX_WIDGET_OPTIONS_PASSWORD = 87,
    TEXTBOX_WIDGET_OPTIONS_PLACEHOLDER = 88
};

enum trackfill_mode_t {
    TRACKFILL_MODE_NONE = 0,
    TRACKFILL_MODE_LEFT = 1,
    TRACKFILL_MODE_CENTER = 2,
    TRACKFILL_MODE_RIGHT = 3
};

enum uri_options_t {
    URI_OPTIONS_DEFAULT = 48,
    URI_OPTIONS_FILTER = 49,
    URI_OPTIONS_SCHEMA = 50
};

enum uri_widget_options_t {
    URI_WIDGET_OPTIONS_PLACEHOLDER = 86,
    URI_WIDGET_OPTIONS_BUTTON_LABEL = 87
};

enum vector_options_t {
    VECTOR_OPTIONS_DEFAULT = 48,
    VECTOR_OPTIONS_MINIMUM = 49,
    VECTOR_OPTIONS_MAXIMUM = 50,
    VECTOR_OPTIONS_MULTIPLEOF = 51,
    VECTOR_OPTIONS_SCALE = 52,
    VECTOR_OPTIONS_UNIT = 53
};

enum widget_options_t {
    WIDGET_OPTIONS_LABEL_VISIBLE = 81,
    WIDGET_OPTIONS_VALUE_VISIBLE = 82,
    WIDGET_OPTIONS_NEEDS_CONFIRMATION = 83,
    WIDGET_OPTIONS_USERDATA = 84
};

enum widgettype_t {
    WIDGETTYPE_DEFAULT = 1,
    WIDGETTYPE_CUSTOM = 2,
    WIDGETTYPE_INFO = 16,
    WIDGETTYPE_TEXTBOX = 17,
    WIDGETTYPE_BUTTON = 18,
    WIDGETTYPE_SWITCH = 19,
    WIDGETTYPE_CHECKBOX = 20,
    WIDGETTYPE_PRESS = 21,
    WIDGETTYPE_NUMBERBOX = 22,
    WIDGETTYPE_DIAL = 23,
    WIDGETTYPE_SLIDER = 24,
    WIDGETTYPE_SLIDER2D = 25,
    WIDGETTYPE_RANGE = 26,
    WIDGETTYPE_DROPDOWN = 27,
    WIDGETTYPE_RADIOBUTTON = 28,
    WIDGETTYPE_COLORCHOOSER = 29,
    WIDGETTYPE_TABLE = 30,
    WIDGETTYPE_URI = 31,
    WIDGETTYPE_IP = 32,
    WIDGETTYPE_IMAGE = 33,
    WIDGETTYPE_LIST = 16384,
    WIDGETTYPE_TABS = 16385
};


const packet_types_t PacketInvalid = PACKET_TYPES_INVALID;


#ifdef __cplusplus
} // namespace v10
} // namespace rcp
#endif


#endif // RCP_V010_TYPES_H
