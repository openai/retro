/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 *
 *  This file contains constants and definitions that can be used commonly between JNI and stack layer
 *
 ******************************************************************************/
#ifndef ANDROID_INCLUDE_BT_COMMON_TYPES_H
#define ANDROID_INCLUDE_BT_COMMON_TYPES_H

#include "bluetooth.h"

typedef struct
{
    uint8_t  client_if;
    uint8_t  filt_index;
    uint8_t  advertiser_state;
    uint8_t  advertiser_info_present;
    uint8_t  addr_type;
    uint8_t  tx_power;
    int8_t  rssi_value;
    uint16_t time_stamp;
    bt_bdaddr_t bd_addr;
    uint8_t  adv_pkt_len;
    uint8_t  *p_adv_pkt_data;
    uint8_t  scan_rsp_len;
    uint8_t  *p_scan_rsp_data;
} btgatt_track_adv_info_t;

typedef enum
{
    BTGATT_DB_PRIMARY_SERVICE,
    BTGATT_DB_SECONDARY_SERVICE,
    BTGATT_DB_INCLUDED_SERVICE,
    BTGATT_DB_CHARACTERISTIC,
    BTGATT_DB_DESCRIPTOR,
} bt_gatt_db_attribute_type_t;

typedef struct
{
    uint8_t             id;
    bt_uuid_t           uuid;
    bt_gatt_db_attribute_type_t type;
    uint16_t            attribute_handle;

    /*
     * If |type| is |BTGATT_DB_PRIMARY_SERVICE|, or
     * |BTGATT_DB_SECONDARY_SERVICE|, this contains the start and end attribute
     * handles.
     */
    uint16_t            start_handle;
    uint16_t            end_handle;

    /*
     * If |type| is |BTGATT_DB_CHARACTERISTIC|, this contains the properties of
     * the characteristic.
     */
    uint8_t             properties;
} btgatt_db_element_t;

#endif  /* ANDROID_INCLUDE_BT_COMMON_TYPES_H */
