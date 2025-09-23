# @(#)generate_config_xml_from_opcode_spec.py 1.0
#
# Copyright (c) 2023, Oracle and/or its affiliates.

import os
import re
import sys
from queue import LifoQueue 

pattern = ["<String", "<Array", "</Array>", "<Substruct", "</Substruct>"]
patterns = '|'.join(pattern)


xml_name = re.compile(r'PCM_OP_[A-Z_]*\d*\.xml')
op_name = xml_name.search(sys.argv[1])
flag = 0
if op_name:
    with open('tmp', 'w') as file_tmp:
        file_tmp.write(sys.argv[1])
    file_list = open('tmp', 'r')
    flag = 0
else:
    file_list = open(sys.argv[1], 'r')
    flag = 1

for file_name in file_list:
    xml_name = re.compile(r'PCM_OP_[A-Z_]*\d*\.xml')
    op_name = xml_name.search(file_name)
    xml_name = op_name.group()
    opcode_name = xml_name[:-4]
    config_filename = "config_"+xml_name[7:]
    if flag:
        file_name = file_name[:-1]

    la = '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
    la += '<ObjectList\n'
    la += '    xmlns="http://www.oracle.com/schemas/BusinessConfig"\n'
    la += '    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\n'
    la += '    xsi:schemaLocation="http://www.oracle.com/schemas/BusinessConfig ../../../xsd/config_maxlength_validate.xsd">\n\n'
    la += '    <ConfigObject>\n'
    la += '        <DESCR>List of flds and maxlength of opcode '+opcode_name+'</DESCR>\n'
    la += '        <NAME>'+opcode_name+'</NAME>\n'
    with open(config_filename, 'w') as file_source:
        level = 0
        elval = 0
        ptstack = LifoQueue(maxsize = 100)
        parent = opcode_name
        ptstack.put(parent)
        file_source.write(la)
        with open(file_name, 'r') as f1:
            contents = f1.read()
        for match in re.finditer(patterns, contents):
            matched = match.group()
            if matched == "<String":
                #Get the start and end index of the line matching the pattern
                start_index = contents.rfind('\n', 0, match.start()) + 1
                end_index = contents.find('\n', match.end())
                # Extract the line containing the match
                line = contents[start_index:end_index]
                #get the start and end index of the field name
                pat_str = r'name="([^"]+)"'
                result = re.search(pat_str, line)
                if result:
                    fld_name = result.group(1)
                    #Get the maxlen value
                    pat_maxlen = r'maxlen="(\d+)"'
                    result = re.search(pat_maxlen, line)
                    if result:
                        maxlen = int(result.group(1))
                    else:
                        # if max length is not mentioned in opcode for any field 
                        # default has been given as 255
                        maxlen = 255
    
                #Get the parent name from stack and push again into the stack
                parent = ptstack.get()
                ptstack.put(parent)
                #Start of printing the value into xml file
                params = "        <PARAMS elem=\"{}\">".format(elval)+"\n"
                params += "           <NAME>"+fld_name+"</NAME>\n"
                params += "           <PARENT_NAME>"+parent+"</PARENT_NAME>\n"
                params += "           <FIELD_LEVEL>"+str(level)+"</FIELD_LEVEL>\n"
                params += "           <MAX_LENGTH>"+str(maxlen)+"</MAX_LENGTH>\n"
                params += "       </PARAMS>\n"
                file_source.write(params)
                #print(params, file=file_source)
                elval = elval + 1
    
            elif matched == "<Array" or matched == "<Substruct":
                #Get the line by finding the start and end index
                partart_index = contents.rfind('\n', 0, match.start()) + 1
                parend_index = contents.find('\n', match.end())
                # Extract the line containing the match
                parent_line = contents[partart_index:parend_index]
                #extract the parent name
                pat_str = r'name="([^"]+)"'
                result = re.search(pat_str, parent_line)
                if result:
                    parent = result.group(1)
                    #increment the level
                    level += 1
                    #Put the parent name in the stack
                    ptstack.put(parent)
                else:
                    print(parent_line+ " No Array name in same line")
    
            elif matched == "</Array>" or matched == "</Substruct>":
                #Decrement the level and pop the stack
                if level > 0:
                    level -= 1
                    ptstack.get()
            else:
                    print("Should not come here")
        ld = '    </ConfigObject>\n'
        ld += '</ObjectList>\n'
        file_source.write(ld)

if os.path.exists('tmp'):
    os.remove('tmp')
