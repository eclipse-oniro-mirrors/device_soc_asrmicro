/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 ****************************************************************************************
 *
 * @file sonata_att.h
 *
 * @brief Header file - ATT.
 *
 ****************************************************************************************
 */

#ifndef SONATA_ATT_H_
#define SONATA_ATT_H_

/**
 ****************************************************************************************
 * @addtogroup ATT Attribute Protocol
 * @ingroup HOST
 * @brief Attribute Protocol.
 *
 * The ATT block contains the procedures for discovering, reading, writing
 * and indicating attributes to peer device . It also defines a number of items
 * that caters to the security aspect of the block as access to some information
 * may require both authorization and an authenticated and encrypted physical
 * link before an attribute can be read or written
 *
 * @{
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// Macro used to convert CPU integer define to LSB first 16-bits UUID
#define SONATA_ATT_UUID_16(uuid)                          (uuid)

/// Invalid attribute handle
#define SONATA_ATT_INVALID_HDL                            (0x0000)
/// Invalid attribute idx (used for profiles)
#define SONATA_ATT_INVALID_IDX                            (0xff)

#define SONATA_ATT_1ST_REQ_START_HDL                      0x0001
#define SONATA_ATT_1ST_REQ_END_HDL                        0xFFFF

/// Maximum possible attribute handle
#define SONATA_ATT_MAX_ATTR_HDL                           SONATA_ATT_1ST_REQ_END_HDL

/// Offset of value in signed PDU
#define SONATA_ATT_SIGNED_PDU_VAL_OFFSET               0x03

/// Attribute Features
#define SONATA_ATT_SERVER_CONFIG                       0x0001
#define SONATA_ATT_SERVICE_DISC                        0x0002
#define SONATA_ATT_RELATIONSHIP_DISC                   0x0004
#define SONATA_ATT_CHAR_DISC                           0x0008
#define SONATA_ATT_CHAR_DESC_DISC                      0x0010
#define SONATA_ATT_RD_CHAR_VALUE                       0x0020
#define SONATA_ATT_WR_CHAR_VALUE                       0x0040
#define SONATA_ATT_NOTIF_CHAR_VALUE                    0x0080
#define SONATA_ATT_IND_CHAR_VALUE                      0x0100
#define SONATA_ATT_RD_CHAR_DESC                        0x0200
#define SONATA_ATT_WR_CHAR_DESC                        0x0400

/// Length, number, offset defines
#define SONATA_ATT_SVC_VALUE_MAX_LEN                   0x0030
#define SONATA_ATT_CHAR_NAME_MAX_LEN                   0x0030
#define SONATA_ATT_UUID_16_LEN                         0x0002
#define SONATA_ATT_UUID_32_LEN                         0x0004
#define SONATA_ATT_UUID_128_LEN                        0x0010

/// offset - l2cap header and ATT code
#define SONATA_ATT_PDU_DATA_OFFSET                     0x05

/// Characteristic Properties Bit
#define SONATA_ATT_CHAR_PROP_BCAST                     0x01
#define SONATA_ATT_CHAR_PROP_RD                        0x02
#define SONATA_ATT_CHAR_PROP_WR_NO_RESP                0x04
#define SONATA_ATT_CHAR_PROP_WR                        0x08
#define SONATA_ATT_CHAR_PROP_NTF                       0x10
#define SONATA_ATT_CHAR_PROP_IND                       0x20
#define SONATA_ATT_CHAR_PROP_AUTH                      0x40
#define SONATA_ATT_CHAR_PROP_EXT_PROP                  0x80
/// Invalid Attribute Handle
#define SONATA_ATT_INVALID_SEARCH_HANDLE               0x0000
#define SONATA_ATT_INVALID_HANDLE                      0x0000
/// Read Information Request
#define SONATA_ATT_UUID_FILTER_0                       0x00
#define SONATA_ATT_UUID_FILTER_2                       0x02
#define SONATA_ATT_UUID_FILTER_16                      0x10
/// Read Information Response
#define SONATA_ATT_FORMAT_LEN                          0x0001
#define SONATA_ATT_FORMAT_16BIT_UUID                   0x01
#define SONATA_ATT_FORMAT_128BIT_UUID                  0x02
/// For No fix length PDU
#define SONATA_ATT_HANDLE_LEN                          0x0002
#define SONATA_ATT_EACHLEN_LEN                         0x0001
#define SONATA_ATT_PROP_LEN                            0x0001
#define SONATA_ATT_CODE_LEN                            0x0001
#define SONATA_ATT_CODE_AND_DATA_LEN                   0x0002
#define SONATA_ATT_CODE_AND_HANDLE_LEN                 0x0003
#define SONATA_ATT_CODE_AND_HANDLE_LEN_AND_OFFSET      0x0005
#define SONATA_ATT_SIGNATURE_LEN                       0x0C

/// extended characteristics
#define SONATA_ATT_EXT_RELIABLE_WRITE                  0x0001
#define SONATA_ATT_EXT_WRITABLE_AUX                    0x0002
#define SONATA_ATT_EXT_RFU                             0xFFFC

/// PDU size for error response
#define SONATA_ATT_ERROR_RESP_LEN                      0x05

/// Offset of value in signed PDU
#define SONATA_ATT_SIGNED_PDU_VAL_OFFSET               0x03

/// Attribute operation code - Method (bit 5 - 0)
#define SONATA_ATT_OPCODE_METHOD_MASK                  0x3F
#define SONATA_ATT_OPCODE_METHOD_LSB                   0
/// Attribute operation code - Command Flag (bit 6)
#define SONATA_ATT_OPCODE_CMD_FLAG_BIT                 0x40
#define SONATA_ATT_OPCODE_CMD_FLAG_POS                 6
/// Attribute operation code - Authentication Signature Flag (bit 7)
#define SONATA_ATT_OPCODE_AUTH_SIGNATURE_FLAG_BIT      0x80
#define SONATA_ATT_OPCODE_AUTH_SIGNATURE_FLAG_POS      7

#define SONATA_ATT_BT_UUID_128 {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
        0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

/// MAC length
#define SONATA_ATT_SIGN_MAC_LEN                       (8)
/// SignCounter length
#define SONATA_ATT_SIGN_COUNTER_LEN                   (4)
/// Signature length
#define SONATA_ATT_SIGN_LEN                           (SONATA_ATT_SIGN_MAC_LEN + SONATA_ATT_SIGN_COUNTER_LEN)

/// This value ensure to have up to 128 client feature supported would be sufficient for several years.
#define SONATA_ATT_MAX_CLI_FEAT_LEN                   16

/* Attribute Specification Defines */

/// Common 16-bit Universal Unique Identifier
enum {
    SONATA_ATT_INVALID_UUID                            = SONATA_ATT_UUID_16(0x0000),
    /*----------------- SERVICES ---------------------*/
    /// Generic Access Profile
    SONATA_ATT_SVC_GENERIC_ACCESS                      = SONATA_ATT_UUID_16(0x1800),
    /// Attribute Profile
    SONATA_ATT_SVC_GENERIC_ATTRIBUTE                   = SONATA_ATT_UUID_16(0x1801),
    /// Immediate alert Service
    SONATA_ATT_SVC_IMMEDIATE_ALERT                     = SONATA_ATT_UUID_16(0x1802),
    /// Link Loss Service
    SONATA_ATT_SVC_LINK_LOSS                           = SONATA_ATT_UUID_16(0x1803),
    /// Tx Power Service
    SONATA_ATT_SVC_TX_POWER                            = SONATA_ATT_UUID_16(0x1804),
    /// Current Time Service Service
    SONATA_ATT_SVC_CURRENT_TIME                        = SONATA_ATT_UUID_16(0x1805),
    /// Reference Time Update Service
    SONATA_ATT_SVC_REF_TIME_UPDATE                     = SONATA_ATT_UUID_16(0x1806),
    /// Next DST Change Service
    SONATA_ATT_SVC_NEXT_DST_CHANGE                     = SONATA_ATT_UUID_16(0x1807),
    /// Glucose Service
    SONATA_ATT_SVC_GLUCOSE                             = SONATA_ATT_UUID_16(0x1808),
    /// Health Thermometer Service
    SONATA_ATT_SVC_HEALTH_THERMOM                      = SONATA_ATT_UUID_16(0x1809),
    /// Device Information Service
    SONATA_ATT_SVC_DEVICE_INFO                         = SONATA_ATT_UUID_16(0x180A),
    /// Heart Rate Service
    SONATA_ATT_SVC_HEART_RATE                          = SONATA_ATT_UUID_16(0x180D),
    /// Phone Alert Status Service
    SONATA_ATT_SVC_PHONE_ALERT_STATUS                  = SONATA_ATT_UUID_16(0x180E),
    /// Battery Service
    SONATA_ATT_SVC_BATTERY_SERVICE                     = SONATA_ATT_UUID_16(0x180F),
    /// Blood Pressure Service
    SONATA_ATT_SVC_BLOOD_PRESSURE                      = SONATA_ATT_UUID_16(0x1810),
    /// Alert Notification Service
    SONATA_ATT_SVC_ALERT_NTF                           = SONATA_ATT_UUID_16(0x1811),
    /// HID Service
    SONATA_ATT_SVC_HID                                 = SONATA_ATT_UUID_16(0x1812),
    /// Scan Parameters Service
    SONATA_ATT_SVC_SCAN_PARAMETERS                     = SONATA_ATT_UUID_16(0x1813),
    /// Running Speed and Cadence Service
    SONATA_ATT_SVC_RUNNING_SPEED_CADENCE               = SONATA_ATT_UUID_16(0x1814),
    /// Cycling Speed and Cadence Service
    SONATA_ATT_SVC_CYCLING_SPEED_CADENCE               = SONATA_ATT_UUID_16(0x1816),
    /// Cycling Power Service
    SONATA_ATT_SVC_CYCLING_POWER                       = SONATA_ATT_UUID_16(0x1818),
    /// Location and Navigation Service
    SONATA_ATT_SVC_LOCATION_AND_NAVIGATION             = SONATA_ATT_UUID_16(0x1819),
    /// Environmental Sensing Service
    SONATA_ATT_SVC_ENVIRONMENTAL_SENSING               = SONATA_ATT_UUID_16(0x181A),
    /// Body Composition Service
    SONATA_ATT_SVC_BODY_COMPOSITION                    = SONATA_ATT_UUID_16(0x181B),
    /// User Data Service
    SONATA_ATT_SVC_USER_DATA                           = SONATA_ATT_UUID_16(0x181C),
    /// Weight Scale Service
    SONATA_ATT_SVC_WEIGHT_SCALE                        = SONATA_ATT_UUID_16(0x181D),
    /// Bond Management Service
    SONATA_ATT_SVC_BOND_MANAGEMENT                     = SONATA_ATT_UUID_16(0x181E),
    /// Continuous Glucose Monitoring Service
    SONATA_ATT_SVC_CONTINUOUS_GLUCOSE_MONITORING       = SONATA_ATT_UUID_16(0x181F),
    /// Internet Protocol Support Service
    SONATA_ATT_SVC_IP_SUPPORT                          = SONATA_ATT_UUID_16(0x1820),
    /// Indoor Positioning Service
    SONATA_ATT_SVC_INDOOR_POSITIONING                  = SONATA_ATT_UUID_16(0x1821),
    /// Pulse Oximeter Service
    SONATA_ATT_SVC_PULSE_OXIMETER                      = SONATA_ATT_UUID_16(0x1822),
    /// HTTP Proxy Service
    SONATA_ATT_SVC_HTTP_PROXY                          = SONATA_ATT_UUID_16(0x1823),
    /// Transport Discovery Service
    SONATA_ATT_SVC_TRANSPORT_DISCOVERY                 = SONATA_ATT_UUID_16(0x1824),
    /// Object Transfer Service
    SONATA_ATT_SVC_OBJECT_TRANSFER                     = SONATA_ATT_UUID_16(0x1825),
    /// Mesh Provisioning Service
    SONATA_ATT_SVC_MESH_PROVISIONING                   = SONATA_ATT_UUID_16(0x1827),
    /// Mesh Proxy Service
    SONATA_ATT_SVC_MESH_PROXY                          = SONATA_ATT_UUID_16(0x1828),
    /// OTA Service
    SONATA_ATT_SVC_OTA                                 = SONATA_ATT_UUID_16(0x1850),

    /*------------------- UNITS ---------------------*/
    /// No defined unit
    SONATA_ATT_UNIT_UNITLESS                           = SONATA_ATT_UUID_16(0x2700),
    /// Length Unit - Metre
    SONATA_ATT_UNIT_METRE                              = SONATA_ATT_UUID_16(0x2701),
    /// Mass unit - Kilogram
    SONATA_ATT_UNIT_KG                                 = SONATA_ATT_UUID_16(0x2702),
    /// Time unit - second
    SONATA_ATT_UNIT_SECOND                             = SONATA_ATT_UUID_16(0x2703),
    /// Electric current unit - Ampere
    SONATA_ATT_UNIT_AMPERE                             = SONATA_ATT_UUID_16(0x2704),
    /// Thermodynamic Temperature unit - Kelvin
    SONATA_ATT_UNIT_KELVIN                             = SONATA_ATT_UUID_16(0x2705),
    /// Amount of substance unit - mole
    SONATA_ATT_UNIT_MOLE                               = SONATA_ATT_UUID_16(0x2706),
    /// Luminous intensity unit - candela
    SONATA_ATT_UNIT_CANDELA                            = SONATA_ATT_UUID_16(0x2707),
    /// Area unit - square metres
    SONATA_ATT_UNIT_SQ_METRE                           = SONATA_ATT_UUID_16(0x2710),
    /// Colume unit - cubic metres
    SONATA_ATT_UNIT_CUBIC_METRE                        = SONATA_ATT_UUID_16(0x2710),
    /// Velocity unit - metres per second
    SONATA_ATT_UNIT_METRE_PER_SECOND                   = SONATA_ATT_UUID_16(0x2711),
    /// Acceleration unit - metres per second squared
    SONATA_ATT_UNIT_METRES_PER_SEC_SQ                  = SONATA_ATT_UUID_16(0x2712),
    /// Wavenumber unit - reciprocal metre
    SONATA_ATT_UNIT_RECIPROCAL_METRE                   = SONATA_ATT_UUID_16(0x2713),
    /// Density unit - kilogram per cubic metre
    SONATA_ATT_UNIT_DENS_KG_PER_CUBIC_METRE            = SONATA_ATT_UUID_16(0x2714),
    /// Surface density unit - kilogram per square metre
    SONATA_ATT_UNIT_KG_PER_SQ_METRE                    = SONATA_ATT_UUID_16(0x2715),
    /// Specific volume unit - cubic metre per kilogram
    SONATA_ATT_UNIT_CUBIC_METRE_PER_KG                 = SONATA_ATT_UUID_16(0x2716),
    /// Current density unit - ampere per square metre
    SONATA_ATT_UNIT_AMPERE_PER_SQ_METRE                = SONATA_ATT_UUID_16(0x2717),
    /// Magnetic field strength unit - Ampere per metre
    SONATA_ATT_UNIT_AMPERE_PER_METRE                   = SONATA_ATT_UUID_16(0x2718),
    /// Amount concentration unit - mole per cubic metre
    SONATA_ATT_UNIT_MOLE_PER_CUBIC_METRE               = SONATA_ATT_UUID_16(0x2719),
    /// Mass Concentration unit - kilogram per cubic metre
    SONATA_ATT_UNIT_MASS_KG_PER_CUBIC_METRE            = SONATA_ATT_UUID_16(0x271A),
    /// Luminance unit - candela per square metre
    SONATA_ATT_UNIT_CANDELA_PER_SQ_METRE               = SONATA_ATT_UUID_16(0x271B),
    /// Refractive index unit
    SONATA_ATT_UNIT_REFRACTIVE_INDEX                   = SONATA_ATT_UUID_16(0x271C),
    /// Relative permeability unit
    SONATA_ATT_UNIT_RELATIVE_PERMEABILITY              = SONATA_ATT_UUID_16(0x271D),
    /// Plane angle unit - radian
    SONATA_ATT_UNIT_RADIAN                             = SONATA_ATT_UUID_16(0x2720),
    /// Solid angle unit - steradian
    SONATA_ATT_UNIT_STERADIAN                          = SONATA_ATT_UUID_16(0x2721),
    /// Frequency unit - Hertz
    SONATA_ATT_UNIT_HERTZ                              = SONATA_ATT_UUID_16(0x2722),
    /// Force unit - Newton
    SONATA_ATT_UNIT_NEWTON                             = SONATA_ATT_UUID_16(0x2723),
    /// Pressure unit - Pascal
    SONATA_ATT_UNIT_PASCAL                             = SONATA_ATT_UUID_16(0x2724),
    /// Energy unit - Joule
    SONATA_ATT_UNIT_JOULE                              = SONATA_ATT_UUID_16(0x2725),
    /// Power unit - Watt
    SONATA_ATT_UNIT_WATT                               = SONATA_ATT_UUID_16(0x2726),
    /// electric Charge unit - Coulomb
    SONATA_ATT_UNIT_COULOMB                            = SONATA_ATT_UUID_16(0x2727),
    /// Electric potential difference - Volt
    SONATA_ATT_UNIT_VOLT                               = SONATA_ATT_UUID_16(0x2728),
    /// Capacitance unit - Farad
    SONATA_ATT_UNIT_FARAD                              = SONATA_ATT_UUID_16(0x2729),
    /// electric resistance unit - Ohm
    SONATA_ATT_UNIT_OHM                                = SONATA_ATT_UUID_16(0x272A),
    /// Electric conductance - Siemens
    SONATA_ATT_UNIT_SIEMENS                            = SONATA_ATT_UUID_16(0x272B),
    /// Magnetic flux unit - Weber
    SONATA_ATT_UNIT_WEBER                              = SONATA_ATT_UUID_16(0x272C),
    /// Magnetic flux density unit - Tesla
    SONATA_ATT_UNIT_TESLA                              = SONATA_ATT_UUID_16(0x272D),
    /// Inductance unit - Henry
    SONATA_ATT_UNIT_HENRY                              = SONATA_ATT_UUID_16(0x272E),
    /// Temperature unit - degree Celsius
    SONATA_ATT_UNIT_CELSIUS                            = SONATA_ATT_UUID_16(0x272F),
    /// Luminous flux unit - lumen
    SONATA_ATT_UNIT_LUMEN                              = SONATA_ATT_UUID_16(0x2730),
    /// Illuminance unit - lux
    SONATA_ATT_UNIT_LUX                                = SONATA_ATT_UUID_16(0x2731),
    /// Activity referred to a radionuclide unit - becquerel
    SONATA_ATT_UNIT_BECQUEREL                          = SONATA_ATT_UUID_16(0x2732),
    /// Absorbed dose unit - Gray
    SONATA_ATT_UNIT_GRAY                               = SONATA_ATT_UUID_16(0x2733),
    /// Dose equivalent unit - Sievert
    SONATA_ATT_UNIT_SIEVERT                            = SONATA_ATT_UUID_16(0x2734),
    /// Catalytic activity unit - Katal
    SONATA_ATT_UNIT_KATAL                              = SONATA_ATT_UUID_16(0x2735),
    /// Synamic viscosity unit - Pascal second
    SONATA_ATT_UNIT_PASCAL_SECOND                      = SONATA_ATT_UUID_16(0x2740),
    /// Moment of force unit - Newton metre
    SONATA_ATT_UNIT_NEWTON_METRE                       = SONATA_ATT_UUID_16(0x2741),
    /// surface tension unit - Newton per metre
    SONATA_ATT_UNIT_NEWTON_PER_METRE                   = SONATA_ATT_UUID_16(0x2742),
    /// Angular velocity unit - radian per second
    SONATA_ATT_UNIT_RADIAN_PER_SECOND                  = SONATA_ATT_UUID_16(0x2743),
    /// Angular acceleration unit - radian per second squared
    SONATA_ATT_UNIT_RADIAN_PER_SECOND_SQ               = SONATA_ATT_UUID_16(0x2744),
    /// Heat flux density unit - Watt per square metre
    SONATA_ATT_UNIT_WSONATA_ATT_PER_SQ_METRE           = SONATA_ATT_UUID_16(0x2745),
    /// HEat capacity unit - Joule per Kelvin
    SONATA_ATT_UNIT_JOULE_PER_KELVIN                   = SONATA_ATT_UUID_16(0x2746),
    /// Specific heat capacity unit - Joule per kilogram kelvin
    SONATA_ATT_UNIT_JOULE_PER_KG_KELVIN                = SONATA_ATT_UUID_16(0x2747),
    /// Specific Energy unit - Joule per kilogram
    SONATA_ATT_UNIT_JOULE_PER_KG                       = SONATA_ATT_UUID_16(0x2748),
    /// Thermal conductivity - Watt per metre Kelvin
    SONATA_ATT_UNIT_WSONATA_ATT_PER_METRE_KELVIN       = SONATA_ATT_UUID_16(0x2749),
    /// Energy Density unit - joule per cubic metre
    SONATA_ATT_UNIT_JOULE_PER_CUBIC_METRE              = SONATA_ATT_UUID_16(0x274A),
    /// Electric field strength unit - volt per metre
    SONATA_ATT_UNIT_VOLT_PER_METRE                     = SONATA_ATT_UUID_16(0x274B),
    /// Electric charge density unit - coulomb per cubic metre
    SONATA_ATT_UNIT_COULOMB_PER_CUBIC_METRE            = SONATA_ATT_UUID_16(0x274C),
    /// Surface charge density unit - coulomb per square metre
    SONATA_ATT_UNIT_SURF_COULOMB_PER_SQ_METRE          = SONATA_ATT_UUID_16(0x274D),
    /// Electric flux density unit - coulomb per square metre
    SONATA_ATT_UNIT_FLUX_COULOMB_PER_SQ_METRE          = SONATA_ATT_UUID_16(0x274E),
    /// Permittivity unit - farad per metre
    SONATA_ATT_UNIT_FARAD_PER_METRE                    = SONATA_ATT_UUID_16(0x274F),
    /// Permeability unit - henry per metre
    SONATA_ATT_UNIT_HENRY_PER_METRE                    = SONATA_ATT_UUID_16(0x2750),
    /// Molar energy unit - joule per mole
    SONATA_ATT_UNIT_JOULE_PER_MOLE                     = SONATA_ATT_UUID_16(0x2751),
    /// Molar entropy unit - joule per mole kelvin
    SONATA_ATT_UNIT_JOULE_PER_MOLE_KELVIN              = SONATA_ATT_UUID_16(0x2752),
    /// Exposure unit - coulomb per kilogram
    SONATA_ATT_UNIT_COULOMB_PER_KG                     = SONATA_ATT_UUID_16(0x2753),
    /// Absorbed dose rate unit - gray per second
    SONATA_ATT_UNIT_GRAY_PER_SECOND                    = SONATA_ATT_UUID_16(0x2754),
    /// Radiant intensity unit - watt per steradian
    SONATA_ATT_UNIT_WSONATA_ATT_PER_STERADIAN          = SONATA_ATT_UUID_16(0x2755),
    /// Radiance unit - watt per square meter steradian
    SONATA_ATT_UNIT_WSONATA_ATT_PER_SQ_METRE_STERADIAN = SONATA_ATT_UUID_16(0x2756),
    /// Catalytic activity concentration unit - katal per cubic metre
    SONATA_ATT_UNIT_KATAL_PER_CUBIC_METRE              = SONATA_ATT_UUID_16(0x2757),
    /// Time unit - minute
    SONATA_ATT_UNIT_MINUTE                             = SONATA_ATT_UUID_16(0x2760),
    /// Time unit - hour
    SONATA_ATT_UNIT_HOUR                               = SONATA_ATT_UUID_16(0x2761),
    /// Time unit - day
    SONATA_ATT_UNIT_DAY                                = SONATA_ATT_UUID_16(0x2762),
    /// Plane angle unit - degree
    SONATA_ATT_UNIT_ANGLE_DEGREE                       = SONATA_ATT_UUID_16(0x2763),
    /// Plane angle unit - minute
    SONATA_ATT_UNIT_ANGLE_MINUTE                       = SONATA_ATT_UUID_16(0x2764),
    /// Plane angle unit - second
    SONATA_ATT_UNIT_ANGLE_SECOND                       = SONATA_ATT_UUID_16(0x2765),
    /// Area unit - hectare
    SONATA_ATT_UNIT_HECTARE                            = SONATA_ATT_UUID_16(0x2766),
    /// Volume unit - litre
    SONATA_ATT_UNIT_LITRE                              = SONATA_ATT_UUID_16(0x2767),
    /// Mass unit - tonne
    SONATA_ATT_UNIT_TONNE                              = SONATA_ATT_UUID_16(0x2768),
    /// Pressure unit - bar
    SONATA_ATT_UNIT_BAR                                = SONATA_ATT_UUID_16(0x2780),
    /// Pressure unit - millimetre of mercury
    SONATA_ATT_UNIT_MM_MERCURY                         = SONATA_ATT_UUID_16(0x2781),
    /// Length unit - angstrom
    SONATA_ATT_UNIT_ANGSTROM                           = SONATA_ATT_UUID_16(0x2782),
    /// Length unit - nautical mile
    SONATA_ATT_UNIT_NAUTICAL_MILE                      = SONATA_ATT_UUID_16(0x2783),
    /// Area unit - barn
    SONATA_ATT_UNIT_BARN                               = SONATA_ATT_UUID_16(0x2784),
    /// Velocity unit - knot
    SONATA_ATT_UNIT_KNOT                               = SONATA_ATT_UUID_16(0x2785),
    /// Logarithmic radio quantity unit - neper
    SONATA_ATT_UNIT_NEPER                              = SONATA_ATT_UUID_16(0x2786),
    /// Logarithmic radio quantity unit - bel
    SONATA_ATT_UNIT_BEL                                = SONATA_ATT_UUID_16(0x2787),
    /// Length unit - yard
    SONATA_ATT_UNIT_YARD                               = SONATA_ATT_UUID_16(0x27A0),
    /// Length unit - parsec
    SONATA_ATT_UNIT_PARSEC                             = SONATA_ATT_UUID_16(0x27A1),
    /// length unit - inch
    SONATA_ATT_UNIT_INCH                               = SONATA_ATT_UUID_16(0x27A2),
    /// length unit - foot
    SONATA_ATT_UNIT_FOOT                               = SONATA_ATT_UUID_16(0x27A3),
    /// length unit - mile
    SONATA_ATT_UNIT_MILE                               = SONATA_ATT_UUID_16(0x27A4),
    /// pressure unit - pound-force per square inch
    SONATA_ATT_UNIT_POUND_FORCE_PER_SQ_INCH            = SONATA_ATT_UUID_16(0x27A5),
    /// velocity unit - kilometre per hour
    SONATA_ATT_UNIT_KM_PER_HOUR                        = SONATA_ATT_UUID_16(0x27A6),
    /// velocity unit - mile per hour
    SONATA_ATT_UNIT_MILE_PER_HOUR                      = SONATA_ATT_UUID_16(0x27A7),
    /// angular velocity unit - revolution per minute
    SONATA_ATT_UNIT_REVOLUTION_PER_MINUTE              = SONATA_ATT_UUID_16(0x27A8),
    /// energy unit - gram calorie
    SONATA_ATT_UNIT_GRAM_CALORIE                       = SONATA_ATT_UUID_16(0x27A9),
    /// energy unit - kilogram calorie
    SONATA_ATT_UNIT_KG_CALORIE                         = SONATA_ATT_UUID_16(0x27AA),
    /// energy unit - kilowatt hour
    SONATA_ATT_UNIT_KILOWSONATA_ATT_HOUR               = SONATA_ATT_UUID_16(0x27AB),
    /// thermodynamic temperature unit - degree Fahrenheit
    SONATA_ATT_UNIT_FAHRENHEIT                         = SONATA_ATT_UUID_16(0x27AC),
    /// percentage
    SONATA_ATT_UNIT_PERCENTAGE                         = SONATA_ATT_UUID_16(0x27AD),
    /// per mille
    SONATA_ATT_UNIT_PER_MILLE                          = SONATA_ATT_UUID_16(0x27AE),
    /// period unit - beats per minute)
    SONATA_ATT_UNIT_BEATS_PER_MINUTE                   = SONATA_ATT_UUID_16(0x27AF),
    /// electric charge unit - ampere hours
    SONATA_ATT_UNIT_AMPERE_HOURS                       = SONATA_ATT_UUID_16(0x27B0),
    /// mass density unit - milligram per decilitre
    SONATA_ATT_UNIT_MILLIGRAM_PER_DECILITRE            = SONATA_ATT_UUID_16(0x27B1),
    /// mass density unit - millimole per litre
    SONATA_ATT_UNIT_MILLIMOLE_PER_LITRE                = SONATA_ATT_UUID_16(0x27B2),
    /// time unit - year
    SONATA_ATT_UNIT_YEAR                               = SONATA_ATT_UUID_16(0x27B3),
    //// time unit - month
    SONATA_ATT_UNIT_MONTH                              = SONATA_ATT_UUID_16(0x27B4),

    /*---------------- DECLARATIONS -----------------*/
    /// Primary service Declaration
    SONATA_ATT_DECL_PRIMARY_SERVICE                     = SONATA_ATT_UUID_16(0x2800),
    /// Secondary service Declaration
    SONATA_ATT_DECL_SECONDARY_SERVICE                   = SONATA_ATT_UUID_16(0x2801),
    /// Include Declaration
    SONATA_ATT_DECL_INCLUDE                             = SONATA_ATT_UUID_16(0x2802),
    /// Characteristic Declaration
    SONATA_ATT_DECL_CHARACTERISTIC                      = SONATA_ATT_UUID_16(0x2803),

    /*----------------- DESCRIPTORS -----------------*/
    /// Characteristic extended properties
    SONATA_ATT_DESC_CHAR_EXT_PROPERTIES                 = SONATA_ATT_UUID_16(0x2900),
    /// Characteristic user description
    SONATA_ATT_DESC_CHAR_USER_DESCRIPTION               = SONATA_ATT_UUID_16(0x2901),
    /// Client characteristic configuration
    SONATA_ATT_DESC_CLIENT_CHAR_CFG                     = SONATA_ATT_UUID_16(0x2902),
    /// Server characteristic configuration
    SONATA_ATT_DESC_SERVER_CHAR_CFG                     = SONATA_ATT_UUID_16(0x2903),
    /// Characteristic Presentation Format
    SONATA_ATT_DESC_CHAR_PRES_FORMAT                    = SONATA_ATT_UUID_16(0x2904),
    /// Characteristic Aggregate Format
    SONATA_ATT_DESC_CHAR_AGGREGATE_FORMAT               = SONATA_ATT_UUID_16(0x2905),
    /// Valid Range
    SONATA_ATT_DESC_VALID_RANGE                         = SONATA_ATT_UUID_16(0x2906),
    /// External Report Reference
    SONATA_ATT_DESC_EXT_REPORT_REF                      = SONATA_ATT_UUID_16(0x2907),
    /// Report Reference
    SONATA_ATT_DESC_REPORT_REF                          = SONATA_ATT_UUID_16(0x2908),
    /// Environmental Sensing Configuration
    SONATA_ATT_DESC_ES_CONFIGURATION                    = SONATA_ATT_UUID_16(0x290B),
    /// Environmental Sensing Measurement
    SONATA_ATT_DESC_ES_MEASUREMENT                      = SONATA_ATT_UUID_16(0x290C),
    /// Environmental Sensing Trigger Setting
    SONATA_ATT_DESC_ES_TRIGGER_SETTING                  = SONATA_ATT_UUID_16(0x290D),

    /*--------------- CHARACTERISTICS ---------------*/
    /// Device name
    SONATA_ATT_CHAR_DEVICE_NAME                        = SONATA_ATT_UUID_16(0x2A00),
    /// Appearance
    SONATA_ATT_CHAR_APPEARANCE                         = SONATA_ATT_UUID_16(0x2A01),
    /// Privacy flag
    SONATA_ATT_CHAR_PRIVACY_FLAG                       = SONATA_ATT_UUID_16(0x2A02),
    /// Reconnection address
    SONATA_ATT_CHAR_RECONNECTION_ADDR                  = SONATA_ATT_UUID_16(0x2A03),
    /// Peripheral preferred connection parameters
    SONATA_ATT_CHAR_PERIPH_PREF_CON_PARAM              = SONATA_ATT_UUID_16(0x2A04),
    /// Service handles changed
    SONATA_ATT_CHAR_SERVICE_CHANGED                    = SONATA_ATT_UUID_16(0x2A05),
    /// Alert Level characteristic
    SONATA_ATT_CHAR_ALERT_LEVEL                        = SONATA_ATT_UUID_16(0x2A06),
    /// Tx Power Level
    SONATA_ATT_CHAR_TX_POWER_LEVEL                     = SONATA_ATT_UUID_16(0x2A07),
    /// Date Time
    SONATA_ATT_CHAR_DATE_TIME                          = SONATA_ATT_UUID_16(0x2A08),
    /// Day of Week
    SONATA_ATT_CHAR_DAY_WEEK                           = SONATA_ATT_UUID_16(0x2A09),
    /// Day Date Time
    SONATA_ATT_CHAR_DAY_DATE_TIME                      = SONATA_ATT_UUID_16(0x2A0A),
    /// Exact time 256
    SONATA_ATT_CHAR_EXACT_TIME_256                     = SONATA_ATT_UUID_16(0x2A0C),
    /// DST Offset
    SONATA_ATT_CHAR_DST_OFFSET                         = SONATA_ATT_UUID_16(0x2A0D),
    /// Time zone
    SONATA_ATT_CHAR_TIME_ZONE                          = SONATA_ATT_UUID_16(0x2A0E),
    /// Local time Information
    SONATA_ATT_CHAR_LOCAL_TIME_INFO                    = SONATA_ATT_UUID_16(0x2A0F),
    /// Time with DST
    SONATA_ATT_CHAR_TIME_WITH_DST                      = SONATA_ATT_UUID_16(0x2A11),
    /// Time Accuracy
    SONATA_ATT_CHAR_TIME_ACCURACY                      = SONATA_ATT_UUID_16(0x2A12),
    /// Time Source
    SONATA_ATT_CHAR_TIME_SOURCE                        = SONATA_ATT_UUID_16(0x2A13),
    /// Reference Time Information
    SONATA_ATT_CHAR_REFERENCE_TIME_INFO                = SONATA_ATT_UUID_16(0x2A14),
    /// Time Update Control Point
    SONATA_ATT_CHAR_TIME_UPDATE_CNTL_POINT             = SONATA_ATT_UUID_16(0x2A16),
    /// Time Update State
    SONATA_ATT_CHAR_TIME_UPDATE_STATE                  = SONATA_ATT_UUID_16(0x2A17),
    /// Glucose Measurement
    SONATA_ATT_CHAR_GLUCOSE_MEAS                       = SONATA_ATT_UUID_16(0x2A18),
    /// Battery Level
    SONATA_ATT_CHAR_BATTERY_LEVEL                      = SONATA_ATT_UUID_16(0x2A19),
    /// Temperature Measurement
    SONATA_ATT_CHAR_TEMPERATURE_MEAS                   = SONATA_ATT_UUID_16(0x2A1C),
    /// Temperature Type
    SONATA_ATT_CHAR_TEMPERATURE_TYPE                   = SONATA_ATT_UUID_16(0x2A1D),
    /// Intermediate Temperature
    SONATA_ATT_CHAR_INTERMED_TEMPERATURE               = SONATA_ATT_UUID_16(0x2A1E),
    /// Measurement Interval
    SONATA_ATT_CHAR_MEAS_INTERVAL                      = SONATA_ATT_UUID_16(0x2A21),
    /// Boot Keyboard Input Report
    SONATA_ATT_CHAR_BOOT_KB_IN_REPORT                  = SONATA_ATT_UUID_16(0x2A22),
    /// System ID
    SONATA_ATT_CHAR_SYS_ID                             = SONATA_ATT_UUID_16(0x2A23),
    /// Model Number String
    SONATA_ATT_CHAR_MODEL_NB                           = SONATA_ATT_UUID_16(0x2A24),
    /// Serial Number String
    SONATA_ATT_CHAR_SERIAL_NB                          = SONATA_ATT_UUID_16(0x2A25),
    /// Firmware Revision String
    SONATA_ATT_CHAR_FW_REV                             = SONATA_ATT_UUID_16(0x2A26),
    /// Hardware revision String
    SONATA_ATT_CHAR_HW_REV                             = SONATA_ATT_UUID_16(0x2A27),
    /// Software Revision String
    SONATA_ATT_CHAR_SW_REV                             = SONATA_ATT_UUID_16(0x2A28),
    /// Manufacturer Name String
    SONATA_ATT_CHAR_MANUF_NAME                         = SONATA_ATT_UUID_16(0x2A29),
    /// IEEE Regulatory Certification Data List
    SONATA_ATT_CHAR_IEEE_CERTIF                        = SONATA_ATT_UUID_16(0x2A2A),
    /// CT Time
    SONATA_ATT_CHAR_CT_TIME                            = SONATA_ATT_UUID_16(0x2A2B),
    /// Magnetic Declination
    SONATA_ATT_CHAR_MAGN_DECLINE                       = SONATA_ATT_UUID_16(0x2A2C),
    /// Scan Refresh
    SONATA_ATT_CHAR_SCAN_REFRESH                       = SONATA_ATT_UUID_16(0x2A31),
    /// Boot Keyboard Output Report
    SONATA_ATT_CHAR_BOOT_KB_OUT_REPORT                 = SONATA_ATT_UUID_16(0x2A32),
    /// Boot Mouse Input Report
    SONATA_ATT_CHAR_BOOT_MOUSE_IN_REPORT               = SONATA_ATT_UUID_16(0x2A33),
    /// Glucose Measurement Context
    SONATA_ATT_CHAR_GLUCOSE_MEAS_CTX                   = SONATA_ATT_UUID_16(0x2A34),
    /// Blood Pressure Measurement
    SONATA_ATT_CHAR_BLOOD_PRESSURE_MEAS                = SONATA_ATT_UUID_16(0x2A35),
    /// Intermediate Cuff Pressure
    SONATA_ATT_CHAR_INTERMEDIATE_CUFF_PRESSURE         = SONATA_ATT_UUID_16(0x2A36),
    /// Heart Rate Measurement
    SONATA_ATT_CHAR_HEART_RATE_MEAS                    = SONATA_ATT_UUID_16(0x2A37),
    /// Body Sensor Location
    SONATA_ATT_CHAR_BODY_SENSOR_LOCATION               = SONATA_ATT_UUID_16(0x2A38),
    /// Heart Rate Control Point
    SONATA_ATT_CHAR_HEART_RATE_CNTL_POINT              = SONATA_ATT_UUID_16(0x2A39),
    /// Alert Status
    SONATA_ATT_CHAR_ALERT_STATUS                       = SONATA_ATT_UUID_16(0x2A3F),
    /// Ringer Control Point
    SONATA_ATT_CHAR_RINGER_CNTL_POINT                  = SONATA_ATT_UUID_16(0x2A40),
    /// Ringer Setting
    SONATA_ATT_CHAR_RINGER_SETTING                     = SONATA_ATT_UUID_16(0x2A41),
    /// Alert Category ID Bit Mask
    SONATA_ATT_CHAR_ALERT_CAT_ID_BIT_MASK              = SONATA_ATT_UUID_16(0x2A42),
    /// Alert Category ID
    SONATA_ATT_CHAR_ALERT_CAT_ID                       = SONATA_ATT_UUID_16(0x2A43),
    /// Alert Notification Control Point
    SONATA_ATT_CHAR_ALERT_NTF_CTNL_PT                  = SONATA_ATT_UUID_16(0x2A44),
    /// Unread Alert Status
    SONATA_ATT_CHAR_UNREAD_ALERT_STATUS                = SONATA_ATT_UUID_16(0x2A45),
    /// New Alert
    SONATA_ATT_CHAR_NEW_ALERT                          = SONATA_ATT_UUID_16(0x2A46),
    /// Supported New Alert Category
    SONATA_ATT_CHAR_SUP_NEW_ALERT_CAT                  = SONATA_ATT_UUID_16(0x2A47),
    /// Supported Unread Alert Category
    SONATA_ATT_CHAR_SUP_UNREAD_ALERT_CAT               = SONATA_ATT_UUID_16(0x2A48),
    /// Blood Pressure Feature
    SONATA_ATT_CHAR_BLOOD_PRESSURE_FEATURE             = SONATA_ATT_UUID_16(0x2A49),
    /// HID Information
    SONATA_ATT_CHAR_HID_INFO                           = SONATA_ATT_UUID_16(0x2A4A),
    /// Report Map
    SONATA_ATT_CHAR_REPORT_MAP                         = SONATA_ATT_UUID_16(0x2A4B),
    /// HID Control Point
    SONATA_ATT_CHAR_HID_CTNL_PT                        = SONATA_ATT_UUID_16(0x2A4C),
    /// Report
    SONATA_ATT_CHAR_REPORT                             = SONATA_ATT_UUID_16(0x2A4D),
    /// Protocol Mode
    SONATA_ATT_CHAR_PROTOCOL_MODE                      = SONATA_ATT_UUID_16(0x2A4E),
    /// Scan Interval Window
    SONATA_ATT_CHAR_SCAN_INTV_WD                       = SONATA_ATT_UUID_16(0x2A4F),
    /// PnP ID
    SONATA_ATT_CHAR_PNP_ID                             = SONATA_ATT_UUID_16(0x2A50),
    /// Glucose Feature
    SONATA_ATT_CHAR_GLUCOSE_FEATURE                    = SONATA_ATT_UUID_16(0x2A51),
    /// Record access control point
    SONATA_ATT_CHAR_REC_ACCESS_CTRL_PT                 = SONATA_ATT_UUID_16(0x2A52),
    /// RSC Measurement
    SONATA_ATT_CHAR_RSC_MEAS                           = SONATA_ATT_UUID_16(0x2A53),
    /// RSC Feature
    SONATA_ATT_CHAR_RSC_FEAT                           = SONATA_ATT_UUID_16(0x2A54),
    /// SC Control Point
    SONATA_ATT_CHAR_SC_CNTL_PT                         = SONATA_ATT_UUID_16(0x2A55),
    /// CSC Measurement
    SONATA_ATT_CHAR_CSC_MEAS                           = SONATA_ATT_UUID_16(0x2A5B),
    /// CSC Feature
    SONATA_ATT_CHAR_CSC_FEAT                           = SONATA_ATT_UUID_16(0x2A5C),
    /// Sensor Location
    SONATA_ATT_CHAR_SENSOR_LOC                         = SONATA_ATT_UUID_16(0x2A5D),
    /// PLX Spot-Check Measurement
    SONATA_ATT_CHAR_PLX_SPOT_CHECK_MEASUREMENT_LOC     = SONATA_ATT_UUID_16(0x2A5E),
    /// PLX Continuous Measurement
    SONATA_ATT_CHAR_PLX_CONTINUOUS_MEASUREMENT_LOC     = SONATA_ATT_UUID_16(0x2A5F),
    /// PLX Features
    SONATA_ATT_CHAR_PLX_FEATURES_LOC                   = SONATA_ATT_UUID_16(0x2A60),
    /// CP Measurement
    SONATA_ATT_CHAR_CP_MEAS                            = SONATA_ATT_UUID_16(0x2A63),
    /// CP Vector
    SONATA_ATT_CHAR_CP_VECTOR                          = SONATA_ATT_UUID_16(0x2A64),
    /// CP Feature
    SONATA_ATT_CHAR_CP_FEAT                            = SONATA_ATT_UUID_16(0x2A65),
    /// CP Control Point
    SONATA_ATT_CHAR_CP_CNTL_PT                         = SONATA_ATT_UUID_16(0x2A66),
    /// Location and Speed
    SONATA_ATT_CHAR_LOC_SPEED                          = SONATA_ATT_UUID_16(0x2A67),
    /// Navigation
    SONATA_ATT_CHAR_NAVIGATION                         = SONATA_ATT_UUID_16(0x2A68),
    /// Position Quality
    SONATA_ATT_CHAR_POS_QUALITY                        = SONATA_ATT_UUID_16(0x2A69),
    /// LN Feature
    SONATA_ATT_CHAR_LN_FEAT                            = SONATA_ATT_UUID_16(0x2A6A),
    /// LN Control Point
    SONATA_ATT_CHAR_LN_CNTL_PT                         = SONATA_ATT_UUID_16(0x2A6B),
    /// Elevation
    SONATA_ATT_CHAR_ELEVATION                          = SONATA_ATT_UUID_16(0x2A6C),
    /// Pressure
    SONATA_ATT_CHAR_PRESSURE                           = SONATA_ATT_UUID_16(0x2A6D),
    /// Temperature
    SONATA_ATT_CHAR_TEMPERATURE                        = SONATA_ATT_UUID_16(0x2A6E),
    /// Humidity
    SONATA_ATT_CHAR_HUMIDITY                           = SONATA_ATT_UUID_16(0x2A6F),
    /// True Wind Speed
    SONATA_ATT_CHAR_TRUE_WIND_SPEED                    = SONATA_ATT_UUID_16(0x2A70),
    /// True Wind Direction
    SONATA_ATT_CHAR_TRUE_WIND_DIR                      = SONATA_ATT_UUID_16(0x2A71),
    /// Apparent Wind Speed
    SONATA_ATT_CHAR_APRNT_WIND_SPEED                   = SONATA_ATT_UUID_16(0x2A72),
    /// Apparent Wind Direction
    SONATA_ATT_CHAR_APRNT_WIND_DIRECTION               = SONATA_ATT_UUID_16(0x2A73),
    /// Gust Factor
    SONATA_ATT_CHAR_GUST_FACTOR                        = SONATA_ATT_UUID_16(0x2A74),
    /// Pollen Concentration
    SONATA_ATT_CHAR_POLLEN_CONC                        = SONATA_ATT_UUID_16(0x2A75),
    /// UV Index
    SONATA_ATT_CHAR_UV_INDEX                           = SONATA_ATT_UUID_16(0x2A76),
    /// Irradiance
    SONATA_ATT_CHAR_IRRADIANCE                         = SONATA_ATT_UUID_16(0x2A77),
    /// Rainfall
    SONATA_ATT_CHAR_RAINFALL                           = SONATA_ATT_UUID_16(0x2A78),
    /// Wind Chill
    SONATA_ATT_CHAR_WIND_CHILL                         = SONATA_ATT_UUID_16(0x2A79),
    /// Heat Index
    SONATA_ATT_CHAR_HEAT_INDEX                         = SONATA_ATT_UUID_16(0x2A7A),
    /// Dew Point
    SONATA_ATT_CHAR_DEW_POINT                          = SONATA_ATT_UUID_16(0x2A7B),
    /// Descriptor Value Changed
    SONATA_ATT_CHAR_DESCRIPTOR_VALUE_CHANGED           = SONATA_ATT_UUID_16(0x2A7D),
    /// Aerobic Heart Rate Lower Limit
    SONATA_ATT_CHAR_AEROBIC_HEART_RATE_LOWER_LIMIT     = SONATA_ATT_UUID_16(0x2A7E),
    /// Aerobic Threshhold
    SONATA_ATT_CHAR_AEROBIC_THRESHHOLD                 = SONATA_ATT_UUID_16(0x2A7F),
    /// Age
    SONATA_ATT_CHAR_AGE                                = SONATA_ATT_UUID_16(0x2A80),
    /// Anaerobic Heart Rate Lower Limit
    SONATA_ATT_CHAR_ANAEROBIC_HEART_RATE_LOWER_LIMIT   = SONATA_ATT_UUID_16(0x2A81),
    /// Anaerobic Heart Rate Upper Limit
    SONATA_ATT_CHAR_ANAEROBIC_HEART_RATE_UPPER_LIMIT   = SONATA_ATT_UUID_16(0x2A82),
    /// Anaerobic Threshhold
    SONATA_ATT_CHAR_ANAEROBIC_THRESHHOLD               = SONATA_ATT_UUID_16(0x2A83),
    /// Aerobic Heart Rate Upper Limit
    SONATA_ATT_CHAR_AEROBIC_HEART_RATE_UPPER_LIMIT     = SONATA_ATT_UUID_16(0x2A84),
    /// Date Of Birth
    SONATA_ATT_CHAR_DATE_OF_BIRTH                      = SONATA_ATT_UUID_16(0x2A85),
    /// Date Of Threshold Assessment
    SONATA_ATT_CHAR_DATE_OF_THRESHOLD_ASSESSMENT       = SONATA_ATT_UUID_16(0x2A86),
    /// Email Address
    SONATA_ATT_CHAR_EMAIL_ADDRESS                      = SONATA_ATT_UUID_16(0x2A87),
    /// Fat Burn Heart Rate Lower Limit
    SONATA_ATT_CHAR_FAT_BURN_HEART_RATE_LOWER_LIMIT    = SONATA_ATT_UUID_16(0x2A88),
    /// Fat Burn Heart Rate Upper Limit
    SONATA_ATT_CHAR_FAT_BURN_HEART_RATE_UPPER_LIMIT    = SONATA_ATT_UUID_16(0x2A89),
    /// First Name
    SONATA_ATT_CHAR_FIRST_NAME                         = SONATA_ATT_UUID_16(0x2A8A),
    /// Five Zone Heart Rate Limits
    SONATA_ATT_CHAR_FIVE_ZONE_HEART_RATE_LIMITS        = SONATA_ATT_UUID_16(0x2A8B),
    /// Gender
    SONATA_ATT_CHAR_GENDER                             = SONATA_ATT_UUID_16(0x2A8C),
    /// Max Heart Rate
    SONATA_ATT_CHAR_MAX_HEART_RATE                     = SONATA_ATT_UUID_16(0x2A8D),
    /// Height
    SONATA_ATT_CHAR_HEIGHT                             = SONATA_ATT_UUID_16(0x2A8E),
    /// Hip Circumference
    SONATA_ATT_CHAR_HIP_CIRCUMFERENCE                  = SONATA_ATT_UUID_16(0x2A8F),
    /// Last Name
    SONATA_ATT_CHAR_LAST_NAME                          = SONATA_ATT_UUID_16(0x2A90),
    /// Maximum Recommended Heart Rate
    SONATA_ATT_CHAR_MAXIMUM_RECOMMENDED_HEART_RATE     = SONATA_ATT_UUID_16(0x2A91),
    /// Resting Heart Rate
    SONATA_ATT_CHAR_RESTING_HEART_RATE                 = SONATA_ATT_UUID_16(0x2A92),
    /// Sport Type For Aerobic And Anaerobic Thresholds
    SONATA_ATT_CHAR_SPORT_TYPE_FOR_AEROBIC_AND_ANAEROBIC_THRESHOLDS = SONATA_ATT_UUID_16(0x2A93),
    /// Three Zone Heart Rate Limits
    SONATA_ATT_CHAR_THREE_ZONE_HEART_RATE_LIMITS       = SONATA_ATT_UUID_16(0x2A94),
    /// Two Zone Heart Rate Limit
    SONATA_ATT_CHAR_TWO_ZONE_HEART_RATE_LIMIT          = SONATA_ATT_UUID_16(0x2A95),
    /// Vo2 Max
    SONATA_ATT_CHAR_VO2_MAX                            = SONATA_ATT_UUID_16(0x2A96),
    /// Waist Circumference
    SONATA_ATT_CHAR_WAIST_CIRCUMFERENCE                = SONATA_ATT_UUID_16(0x2A97),
    /// Weight
    SONATA_ATT_CHAR_WEIGHT                             = SONATA_ATT_UUID_16(0x2A98),
    /// Database Change Increment
    SONATA_ATT_CHAR_DATABASE_CHANGE_INCREMENT          = SONATA_ATT_UUID_16(0x2A99),
    /// User Index
    SONATA_ATT_CHAR_USER_INDEX                         = SONATA_ATT_UUID_16(0x2A9A),
    /// Body Composition Feature
    SONATA_ATT_CHAR_BODY_COMPOSITION_FEATURE           = SONATA_ATT_UUID_16(0x2A9B),
    /// Body Composition Measurement
    SONATA_ATT_CHAR_BODY_COMPOSITION_MEASUREMENT       = SONATA_ATT_UUID_16(0x2A9C),
    /// Weight Measurement
    SONATA_ATT_CHAR_WEIGHT_MEASUREMENT                 = SONATA_ATT_UUID_16(0x2A9D),
    /// Weight Scale Feature
    SONATA_ATT_CHAR_WEIGHT_SCALE_FEATURE               = SONATA_ATT_UUID_16(0x2A9E),
    /// User Control Point
    SONATA_ATT_CHAR_USER_CONTROL_POINT                 = SONATA_ATT_UUID_16(0x2A9F),
    /// Flux Density - 2D
    SONATA_ATT_CHAR_MAGN_FLUX_2D                       = SONATA_ATT_UUID_16(0x2AA0),
    /// Magnetic Flux Density - 3D
    SONATA_ATT_CHAR_MAGN_FLUX_3D                       = SONATA_ATT_UUID_16(0x2AA1),
    /// Language string
    SONATA_ATT_CHAR_LANGUAGE                           = SONATA_ATT_UUID_16(0x2AA2),
    /// Barometric Pressure Trend
    SONATA_ATT_CHAR_BAR_PRES_TREND                     = SONATA_ATT_UUID_16(0x2AA3),
    /// Central Address Resolution Support
    SONATA_ATT_CHAR_CTL_ADDR_RESOL_SUPP                = SONATA_ATT_UUID_16(0x2AA6),
    /// CGM Measurement
    SONATA_ATT_CHAR_CGM_MEASUREMENT                    = SONATA_ATT_UUID_16(0x2AA7),
    /// CGM Features
    SONATA_ATT_CHAR_CGM_FEATURES                       = SONATA_ATT_UUID_16(0x2AA8),
    /// CGM Status
    SONATA_ATT_CHAR_CGM_STATUS                         = SONATA_ATT_UUID_16(0x2AA9),
    /// CGM Session Start
    SONATA_ATT_CHAR_CGM_SESSION_START                  = SONATA_ATT_UUID_16(0x2AAA),
    /// CGM Session Run
    SONATA_ATT_CHAR_CGM_SESSION_RUN                    = SONATA_ATT_UUID_16(0x2AAB),
    /// CGM Specific Ops Control Point
    SONATA_ATT_CHAR_CGM_SPECIFIC_OPS_CTRL_PT           = SONATA_ATT_UUID_16(0x2AAC),
    /// Resolvable Private Address only
    SONATA_ATT_CHAR_RSLV_PRIV_ADDR_ONLY                = SONATA_ATT_UUID_16(0x2AC9),
    /// Mesh Provisioning Data In
    SONATA_ATT_CHAR_MESH_PROV_DATA_IN                  = SONATA_ATT_UUID_16(0x2ADB),
    /// Mesh Provisioning Data Out
    SONATA_ATT_CHAR_MESH_PROV_DATA_OUT                 = SONATA_ATT_UUID_16(0x2ADC),
    /// Mesh Proxy Data In
    SONATA_ATT_CHAR_MESH_PROXY_DATA_IN                 = SONATA_ATT_UUID_16(0x2ADD),
    /// Mesh Proxy Data Out
    SONATA_ATT_CHAR_MESH_PROXY_DATA_OUT                = SONATA_ATT_UUID_16(0x2ADE),

    /// Client Supported Features
    SONATA_ATT_CHAR_CLI_SUP_FEAT                       = SONATA_ATT_UUID_16(0x2B29),
    /// Database Hash
    SONATA_ATT_CHAR_DB_HASH                            = SONATA_ATT_UUID_16(0x2B2A),
    /// OTA Data In
    SONATA_ATT_CHAR_OTA_DATA_IN                        = SONATA_ATT_UUID_16(0x2C00),
    /// OTA Data Out
    SONATA_ATT_CHAR_OTA_DATA_OUT                       = SONATA_ATT_UUID_16(0x2C01),
};

/// Format for Characteristic Presentation
enum {
    /// unsigned 1-bit: true or false
    SONATA_ATT_FORMAT_BOOL     = 0x01,
    /// unsigned 2-bit integer
    SONATA_ATT_FORMAT_2BIT,
    /// unsigned 4-bit integer
    SONATA_ATT_FORMAT_NIBBLE,
    /// unsigned 8-bit integer
    SONATA_ATT_FORMAT_UINT8,
    /// unsigned 12-bit integer
    SONATA_ATT_FORMAT_UINT12,
    /// unsigned 16-bit integer
    SONATA_ATT_FORMAT_UINT16,
    /// unsigned 24-bit integer
    SONATA_ATT_FORMAT_UINT24,
    /// unsigned 32-bit integer
    SONATA_ATT_FORMAT_UINT32,
    /// unsigned 48-bit integer
    SONATA_ATT_FORMAT_UINT48,
    /// unsigned 64-bit integer
    SONATA_ATT_FORMAT_UINT64,
    /// unsigned 128-bit integer
    SONATA_ATT_FORMAT_UINT128,
    /// signed 8-bit integer
    SONATA_ATT_FORMAT_SINT8,
    /// signed 12-bit integer
    SONATA_ATT_FORMAT_SINT12,
    /// signed 16-bit integer
    SONATA_ATT_FORMAT_SINT16,
    /// signed 24-bit integer
    SONATA_ATT_FORMAT_SINT24,
    /// signed 32-bit integer
    SONATA_ATT_FORMAT_SINT32,
    /// signed 48-bit integer
    SONATA_ATT_FORMAT_SINT48,
    /// signed 64-bit integer
    SONATA_ATT_FORMAT_SINT64,
    /// signed 128-bit integer
    SONATA_ATT_FORMAT_SINT128,
    /// IEEE-754 32-bit floating point
    SONATA_ATT_FORMAT_FLOAT32,
    /// IEEE-754 64-bit floating point
    SONATA_ATT_FORMAT_FLOAT64,
    /// IEEE-11073 16-bit SFLOAT
    SONATA_ATT_FORMAT_SFLOAT,
    /// IEEE-11073 32-bit FLOAT
    SONATA_ATT_FORMAT_FLOAT,
    /// IEEE-20601 format
    SONATA_ATT_FORMAT_DUINT16,
    /// UTF-8 string
    SONATA_ATT_FORMAT_UTF8S,
    /// UTF-16 string
    SONATA_ATT_FORMAT_UTF16S,
    /// Opaque structure
    SONATA_ATT_FORMAT_STRUCT,
    /// Last format
    SONATA_ATT_FORMAT_LAST
};

/// Client Characteristic Configuration Codes
enum sonata_att_ccc_val {
    /// Stop notification/indication
    SONATA_ATT_CCC_STOP_NTFIND = 0x0000,
    /// Start notification
    SONATA_ATT_CCC_START_NTF,
    /// Start indication
    SONATA_ATT_CCC_START_IND
};

/*
 * Type Definition
 ****************************************************************************************
 */

/// Attribute length type

/// UUID - 128-bit type
struct sonata_att_uuid_128 {
    /// 128-bit UUID
    uint8_t uuid[SONATA_ATT_UUID_128_LEN];
};

/// UUID - 32-bit type
struct sonata_att_uuid_32 {
    /// 32-bit UUID
    uint8_t uuid[SONATA_ATT_UUID_32_LEN];
};

/// Characteristic Value Descriptor
struct sonata_att_char_desc {
    /// properties
    uint8_t prop;
    /// attribute handle
    uint8_t attr_hdl[SONATA_ATT_HANDLE_LEN];
    /// attribute type
    uint8_t attr_type[SONATA_ATT_UUID_16_LEN];
};

/// Characteristic Value Descriptor
struct sonata_att_char128_desc {
    /// properties
    uint8_t prop;
    /// attribute handle
    uint8_t attr_hdl[SONATA_ATT_HANDLE_LEN];
    /// attribute type
    uint8_t attr_type[SONATA_ATT_UUID_128_LEN];
};

/// Service Value Descriptor - 16-bit
typedef uint16_t att_svc_desc_t;

/// include service entry element
struct sonata_att_incl_desc {
    /// start handle value of included service
    uint16_t start_hdl;
    /// end handle value of included service
    uint16_t end_hdl;
    /// attribute value UUID
    uint16_t uuid;
};

/// include service entry element
struct sonata_att_incl128_desc {
    /// start handle value of included service
    uint16_t start_hdl;
    /// end handle value of included service
    uint16_t end_hdl;
};

// --------------------------- Database permissions -----------------------------

/// Macro used to retrieve access permission rights
#define SONATA_PERM_GET(perm, access) \
    (((perm) & (SONATA_PERM_MASK_ ## access)) >> (SONATA_PERM_POS_ ## access))

/// Macro used to set a permission value
#define SONATA_PERM_SET(perm, access, value) \
    perm = (((perm) & ~(SONATA_PERM_MASK_ ## access)) | (((value) << (SONATA_PERM_POS_ ## access)) & (SONATA_PERM_MASK_ ## access)))

/// Macro used to retrieve permission value from access and rights on attribute.
#define SONATA_PERM(access, right) \
    (((SONATA_PERM_RIGHT_ ## right) << (SONATA_PERM_POS_ ## access)) & (SONATA_PERM_MASK_ ## access))

/// Macro used know if permission is set or not.
#define SONATA_PERM_IS_SET(perm, access, right) \
    (((perm) & (((SONATA_PERM_RIGHT_ ## right) << (SONATA_PERM_POS_ ## access))) \
      & (SONATA_PERM_MASK_ ## access)) == SONATA_PERM(access, right))

/// Retrieve attribute security level from attribute right and service right

/// Retrieve UUID LEN from UUID Length Permission
#define SONATA_ATT_UUID_LEN(uuid_len_perm) (((uuid_len_perm) == 0) ? ATT_UUID_16_LEN : \
        (((uuid_len_perm) == 1) ? ATT_UUID_32_LEN  :                        \
         (((uuid_len_perm) == 2) ? ATT_UUID_128_LEN : 0)))

/// Initialization of attribute element
#define SONATA_ATT_ELEMT_INIT                                   {{NULL}, false}

/**
 *   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 * |EXT | WS | I  | N  | WR | WC | RD | B  |    NP   |    IP   |   WP    |    RP   |
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 *
 * Bit [0-1]  : Read mode         (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON)
 * Bit [2-3]  : Write mode        (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON)
 * Bit [4-5]  : Indication mode   (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON)
 * Bit [6-7]  : Notification mode (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = SEC_CON)
 *
 * Bit [8]    : Broadcast descriptor present       (only relevant for a characteristic value)
 * Bit [9]    : Read accepted
 * Bit [10]   : Write Command accepted
 * Bit [11]   : Write Request accepted
 * Bit [12]   : Notification accepted
 * Bit [13]   : Indication accepted
 * Bit [14]   : Write Signed accepted
 * Bit [15]   : Extended properties present (only relevant for a characteristic value)
 */
enum sonata_attm_perm_mask {
    /// retrieve all permission info
    SONATA_PERM_MASK_ALL           = 0x0000,
    /// Read Permission Mask
    SONATA_PERM_MASK_RP            = 0x0003,
    SONATA_PERM_POS_RP             = 0,
    /// Write Permission Mask
    SONATA_PERM_MASK_WP            = 0x000C,
    SONATA_PERM_POS_WP             = 2,
    /// Indication Access Mask
    SONATA_PERM_MASK_IP            = 0x0030,
    SONATA_PERM_POS_IP             = 4,
    /// Notification Access Mask
    SONATA_PERM_MASK_NP            = 0x00C0,
    SONATA_PERM_POS_NP             = 6,
    /// Broadcast descriptor present
    SONATA_PERM_MASK_BROADCAST     = 0x0100,
    SONATA_PERM_POS_BROADCAST      = 8,
    /// Read Access Mask
    SONATA_PERM_MASK_RD            = 0x0200,
    SONATA_PERM_POS_RD             = 9,
    /// Write Command Enabled attribute Mask
    SONATA_PERM_MASK_WRITE_COMMAND = 0x0400,
    SONATA_PERM_POS_WRITE_COMMAND  = 10,
    /// Write Request Enabled attribute Mask
    SONATA_PERM_MASK_WRITE_REQ     = 0x0800,
    SONATA_PERM_POS_WRITE_REQ      = 11,
    /// Notification Access Mask
    SONATA_PERM_MASK_NTF           = 0x1000,
    SONATA_PERM_POS_NTF            = 12,
    /// Indication Access Mask
    SONATA_PERM_MASK_IND           = 0x2000,
    SONATA_PERM_POS_IND            = 13,
    /// Write Signed Enabled attribute Mask
    SONATA_PERM_MASK_WRITE_SIGNED  = 0x4000,
    SONATA_PERM_POS_WRITE_SIGNED   = 14,
    /// Extended properties descriptor present
    SONATA_PERM_MASK_EXT           = 0x8000,
    SONATA_PERM_POS_EXT            = 15,

    /// Properties
    SONATA_PERM_MASK_PROP          = 0xFF00,
    SONATA_PERM_POS_PROP           = 8,
};

/**
 * Value permission bit field
 *
 *   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 * | RI |UUID_LEN |EKS |    MAX_LEN (RI = 1) / Value Offset  (RI = 0)              |
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 *
 * Bit [0-11] : Maximum Attribute Length or Value Offset pointer
 * Bit [12]   : Encryption key Size must be 16 bytes
 * Bit [14-13]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
 * Bit [15]   : Trigger Read Indication (0 = Value present in Database, 1 = Value not present in Database)
 */
enum sonata_attm_value_perm_mask {
    /// Maximum Attribute Length
    SONATA_PERM_MASK_MAX_LEN     = 0x0FFF,
    SONATA_PERM_POS_MAX_LEN      = 0,
    /// Attribute value Offset
    SONATA_PERM_MASK_VAL_OFFSET  = 0x0FFF,
    SONATA_PERM_POS_VAL_OFFSET   = 0,
    /// Check Encryption key size Mask
    SONATA_PERM_MASK_EKS         = 0x1000,
    SONATA_PERM_POS_EKS          = 12,
    /// UUID Length
    SONATA_PERM_MASK_UUID_LEN    = 0x6000,
    SONATA_PERM_POS_UUID_LEN     = 13,
    /// Read trigger Indication
    SONATA_PERM_MASK_RI          = 0x8000,
    SONATA_PERM_POS_RI           = 15,
};

/**
 * Service permissions
 *
 *    7    6    5    4    3    2    1    0
 * +----+----+----+----+----+----+----+----+
 * |SEC |UUID_LEN |DIS |  AUTH   |EKS | MI |
 * +----+----+----+----+----+----+----+----+
 *
 * Bit [0]  : Task that manage service is multi-instantiated (Connection index is conveyed)
 * Bit [1]  : Encryption key Size must be 16 bytes
 * Bit [2-3]: Service Permission      (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = Secure Connect)
 * Bit [4]  : Disable the service
 * Bit [5-6]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
 * Bit [7]  : Secondary Service       (0 = Primary Service, 1 = Secondary Service)
 */
enum sonata_attm_svc_perm_mask {
    /// Task that manage service is multi-instantiated
    SONATA_PERM_MASK_SVC_MI        = 0x01,
    SONATA_PERM_POS_SVC_MI         = 0,
    /// Check Encryption key size for service Access
    SONATA_PERM_MASK_SVC_EKS       = 0x02,
    SONATA_PERM_POS_SVC_EKS        = 1,
    /// Service Permission authentication
    SONATA_PERM_MASK_SVC_AUTH      = 0x0C,
    SONATA_PERM_POS_SVC_AUTH       = 2,
    /// Disable the service
    SONATA_PERM_MASK_SVC_DIS       = 0x10,
    SONATA_PERM_POS_SVC_DIS        = 4,
    /// Service UUID Length
    SONATA_PERM_MASK_SVC_UUID_LEN  = 0x60,
    SONATA_PERM_POS_SVC_UUID_LEN   = 5,
    /// Service type Secondary
    SONATA_PERM_MASK_SVC_SECONDARY = 0x80,
    SONATA_PERM_POS_SVC_SECONDARY  = 7,
};

/// Attribute & Service access mode
enum {
    /// Disable access
    SONATA_PERM_RIGHT_DISABLE  = 0,
    /// Enable access
    SONATA_PERM_RIGHT_ENABLE   = 1,
};

/// Attribute & Service access rights
enum {
    /// No Authentication
    SONATA_PERM_RIGHT_NO_AUTH  = 0,
    /// Access Requires Unauthenticated link
    SONATA_PERM_RIGHT_UNAUTH   = 1,
    /// Access Requires Authenticated link
    SONATA_PERM_RIGHT_AUTH     = 2,
    /// Access Requires Secure Connection link
    SONATA_PERM_RIGHT_SEC_CON  = 3,
};

/// Attribute & Service UUID Length
enum {
    /// 16  bits UUID
    SONATA_PERM_UUID_16         = 0,
    SONATA_PERM_RIGHT_UUID_16   = 0,
    /// 32  bits UUID
    SONATA_PERM_UUID_32         = 1,
    SONATA_PERM_RIGHT_UUID_32   = 1,
    /// 128 bits UUID
    SONATA_PERM_UUID_128        = 2,
    SONATA_PERM_RIGHT_UUID_128  = 2,
    /// Invalid
    SONATA_PERM_UUID_RFU        = 3,
};

/// execute flags
enum {
    /// Cancel All the Reliable Writes
    SONATA_ATT_CANCEL_ALL_PREPARED_WRITES = 0x00,
    /// Write All the Reliable Writes
    SONATA_ATT_EXECUTE_ALL_PREPARED_WRITES
};

/// @} ATT
#endif // SONATA_ATT_H_
