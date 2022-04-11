/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

#include "hal_file.h"
#include "duet_flash_kv.h"
#include "kv_err.h"
#include "kv_conf.h"
#include "kv_store.h"
#include "ohos_errno.h"
#include "utils_file.h"
/* not support file system yet, use kv to get/set */
/* just for param xts */

char *duet_kv_key;
int HalFileRead(int fd, char *buf, unsigned int len);

int HalFileOpen(const char *path, int oflag, int mode)
{
    (void)mode;
    int32_t retval = 0;
    char temp[128] = {0};
    duet_kv_key = path;

    if (oflag & O_CREAT_FS) {
        return 0;    // new item
    }

    retval = HalFileRead(0, temp, 128);
    if (retval < 0) {
        return -1;
    } else {
        return 0;
    }
}

int HalFileClose(int fd)
{
    return 0;
}

int HalFileRead(int fd, char *buf, unsigned int len)
{
    int32_t retval = 0;
    int templen = len + 1;
    retval = duet_flash_kv_get(duet_kv_key, (void *)buf, (int32_t *)&templen);
    if (retval == KV_ERR_INVALID_PARAM || len <= 1) {
        printf("KV_ERR_INVALID_PARAM \n");
        return EC_INVALID;
    }
    if (retval == KV_OK) {
        return (templen - 1);
    }
    // printf("kv error %d\n",retval);
    return EC_FAILURE;
}

int HalFileWrite(int fd, const char *buf, unsigned int len)
{
    int32_t retval = 0;
    int32_t buflen = strlen(buf) + 1;

    retval = duet_flash_kv_set(duet_kv_key, (void *)buf, buflen, 1);
    if (retval == KV_ERR_INVALID_PARAM) {
        printf("KV_ERR_INVALID_PARAM \n");
        return EC_INVALID;
    }
    if (retval == KV_OK) {
        return 0;
    }
    if (retval == KV_ERR_NO_SPACE) {
        printf("KV_ERR_NO_SPACE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
    //       return EC_NOSPACE;
    printf("kv error %d\n", retval);
    return EC_FAILURE;
}

int HalFileDelete(const char *path)
{
    int32_t retval = 0;

    retval = duet_flash_kv_del(path);
    if (retval == KV_ERR_INVALID_PARAM) {
        printf("KV_ERR_INVALID_PARAM \n");
        return EC_INVALID;
    }
    if (retval == KV_OK) {
        return 0;
    }
    // printf("kv error %d\n",retval);
    return EC_FAILURE;
}

int HalFileStat(const char *path, unsigned int *fileSize)
{
    int len = 150;
    char temp[150] = {0};

    int32_t retval = 0;

    retval = duet_flash_kv_get(path, (void *)temp, (int32_t *)&len);
    if (retval == KV_ERR_INVALID_PARAM || len <= 1) {
        printf("kv error %d\n", retval);
        *fileSize = 0;
    }

    if (retval == KV_OK) {
        *fileSize = (len - 1);
    }

    return 0;
}

int HalFileSeek(int fd, int offset, unsigned int whence)
{
    return 0;
}
