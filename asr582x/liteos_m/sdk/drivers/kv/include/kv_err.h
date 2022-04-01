/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited.
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

#ifndef KV_ERR_H
#define KV_ERR_H

/* Key-value function return code description */
#define KV_OK                (0)     /* Successed */
#define KV_LOOP_CONTINUE     (10000) /* Loop Continue */
#define KV_ERR_NO_SPACE      (-10001) /* The space is out of range */
#define KV_ERR_INVALID_PARAM (-10002) /* The parameter is invalid */
#define KV_ERR_MALLOC_FAILED (-10003) /* The os memory malloc error */
#define KV_ERR_NOT_FOUND     (-10004) /* Could not found the item */
#define KV_ERR_FLASH_READ    (-10005) /* The flash read operation error */
#define KV_ERR_FLASH_WRITE   (-10006) /* The flash write operation error */
#define KV_ERR_FLASH_ERASE   (-10007) /* The flash erase operation error */
#define KV_ERR_OS_LOCK       (-10008) /* The error related to os lock */
#define KV_ERR_OS_SEM        (-10009) /* The error related to os semaphose */

#define KV_ERR_ENCRYPT       (-10010) /* Data encryption error */
#define KV_ERR_DECRYPT       (-10011) /* Data decryption error */
#define KV_ERR_NOT_SUPPORT   (-10012) /* The function is not support yet */

#endif  /* KV_ERR_H */

