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
 ******************************************************************************
 *
 * @file system_version.h
 *
 * @brief lega version info provide
 *
 ******************************************************************************
 */

#ifndef __SYSTEM_VERSION_H__
#define __SYSTEM_VERSION_H__

/**
 * Get wifi version.
 *
 * @return  wifi version success, 0 failure.
 */
const char *lega_get_wifi_version(void);

#endif // __SYSTEM_VERSION_H__
