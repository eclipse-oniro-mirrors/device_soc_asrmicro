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

#ifndef SONATA_STACK_FIXED_CONFIG_H_
#define SONATA_STACK_FIXED_CONFIG_H_

/*
 * DEFINES
 ****************************************************************************************
 */
//////////////////////////////////////////
/* do not alter */
#ifndef CFG_BLE
#define CFG_BLE
#endif

#define CFG_EMB

#define CFG_ALLROLES

#ifdef CFG_PLF_SONATA
#define CFG_ACT            8
#endif

#ifdef CFG_PLF_DUET
#define CFG_ACT            12
#endif
#define CFG_RAL            50

#define CFG_CON_CTE_REQ

#define CFG_CON_CTE_RSP

#define CFG_CONLESS_CTE_TX

#define CFG_CONLESS_CTE_RX

#define CFG_AOD

#define CFG_AOA

#define CFG_WLAN_COEX

#define CFG_NB_PRF         10

#endif /* SONATA_STACK_FIXED_CONFIG_H_ */