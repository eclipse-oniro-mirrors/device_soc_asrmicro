from os import path
import sys
import subprocess
import os

print("Generate the bin file")
shell_script =  '''{buildTool}/arm-none-eabi-size {buildDir}/bin/duetiot_582x.elf
{buildTool}/arm-none-eabi-objcopy -O binary -S {buildDir}/bin/duetiot_582x.elf {buildDir}/bin/duetiot_582x.bin'''.format(buildDir=sys.argv[2], buildTool=sys.argv[4])
print(shell_script)
cmd_output = os.system(shell_script)
print(cmd_output)

print("Generate the lst file")
shell_script =  '''{buildTool}/arm-none-eabi-objdump -s -x -l -d -w -C {buildDir}/bin/duetiot_582x.elf>{buildDir}/bin/duetiot_582x.lst'''.format(buildDir=sys.argv[2], buildTool=sys.argv[4])
print(shell_script)
cmd_output = os.system(shell_script)
print(cmd_output)

print("Generate the ota file")
shell_script = '''{libPath}/../../../tools/image_gen_header/image_gen_header {buildDir}/bin/duetiot_582x.bin flash_remapping'''.format(libPath=sys.argv[3], buildDir=sys.argv[2])
print(shell_script)
cmd_output = os.system(shell_script)
print(cmd_output)

print("Generate the factory bin")
shell_script = '''python {libPath}/../../../tools/factory_gen/gen_factory_bin.py {buildDir}/bin/duetiot_582x.bin {libPath}/../../../tools/factory_gen/factory_bin_files {buildDir}/bin'''.format(libPath=sys.argv[3], buildDir=sys.argv[2])
print(shell_script)
cmd_output = os.system(shell_script)
print(cmd_output)

if sys.argv[1] == "debug":
    print("copy general lib files to dest dir")
    shell_script = '''cp {buildDir}/libs/libasr_combo.a {buildDir}/libs/libasr_lwip.a {libPath}'''.format(buildDir=sys.argv[2], libPath=sys.argv[3])
    print(shell_script)
    cmd_output = os.system(shell_script)
    print(cmd_output)
    