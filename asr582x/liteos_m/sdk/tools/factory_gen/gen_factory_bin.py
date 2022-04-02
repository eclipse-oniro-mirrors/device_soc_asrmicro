# Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
import struct
import binascii

def crc32(data):
    return (binascii.crc32(data) & 0xffffffff)

def generate_header_data(val,len):
    if len == 3:
        conver_data = [ struct.pack('B', (val & 0x00FF0000) >> 16),
                        struct.pack('B', (val & 0xFF00) >> 8),
                        struct.pack('B', val & 0xFF)]
    elif len == 4:
        conver_data = [ struct.pack('B', (val & 0xFF000000) >> 24),
                        struct.pack('B', (val & 0x00FF0000) >> 16),
                        struct.pack('B', (val & 0xFF00) >> 8),
                        struct.pack('B', val & 0xFF)]
    else:
        print("len para invalid")
    return conver_data

def merge_process(src_app,src_others_path,target_path):
    if src_app == "" or src_others_path == "":
        print("no path")
        return
    bin_file_list = os.listdir(src_others_path)
    for files in bin_file_list:
        if "bootload" in files:
            bootload_name = files
        if "ate" in files:
            ate_name = files
    bootload_path = src_others_path + '/' + bootload_name
    app_path = src_app
    ate_path = src_others_path + '/' + ate_name
    bin_result_path = target_path +'/asr_image.bin'
    print(bootload_path)
    print(ate_path)
    print(bin_result_path)
    try:
        bootload_bin_f = open(bootload_path,'rb')
        app_bin_f = open(app_path, 'rb')
        ate_bin_f = open(ate_path, 'rb')
    except:
        print("open file error")
        return
    bootload_data = bootload_bin_f.read()
    app_data = app_bin_f.read()
    ate_data = ate_bin_f.read()

    bootload_size = os.path.getsize(bootload_path)
    app_size = os.path.getsize(app_path)
    ate_size = os.path.getsize(ate_path)
    print("app size")
    print(app_size)

    bootload_crc32 = crc32(bootload_data)
    app_crc32 = crc32(app_data)
    ate_crc32 = crc32(ate_data)

    bootload_size_val = generate_header_data(bootload_size, 3)
    app_size_val = generate_header_data(app_size, 3)
    ate_size_val = generate_header_data(ate_size, 3)

    bootload_crc32_val = generate_header_data(bootload_crc32, 4)
    app_crc32_val = generate_header_data(app_crc32, 4)
    ate_crc32_val = generate_header_data(ate_crc32, 4)

    head_data = [bootload_size_val,app_size_val,ate_size_val,bootload_crc32_val,app_crc32_val,ate_crc32_val]

    bin_merge = open(bin_result_path,'w+')
    bin_merge.truncate()
    bin_merge.close()

    bin_merge = open(bin_result_path, 'ab')
    for i in range(0,len(head_data)):
        for j in range(0,len(head_data[i])):
            bin_merge.write(head_data[i][j])

    bin_merge.write(bootload_data)
    bin_merge.write(app_data)
    bin_merge.write(ate_data)
    bootload_bin_f.close()
    app_bin_f.close()
    ate_bin_f.close()
    bin_merge.close()

if __name__ == '__main__':
    print("merging..")
    if len(sys.argv) == 4:
        merge_process(sys.argv[1], sys.argv[2], sys.argv[3])  # src app , src others path