find_program(iwyu_tool NAMES iwyu_tool.py)
set(IWYU_MAPPING_FILE ${CMAKE_SOURCE_DIR}/cmake/include_rules.imp)
set(IWYU_WORK_DIR ${CMAKE_BINARY_DIR}/iwyu)
add_custom_target(run-iwyu
  COMMAND ${iwyu_tool} -p ${IWYU_WORK_DIR}
  -- mapping_file=${IWYU_MAPPING_FILE}
  WORKING_DIR=${IWYU_WORK_DIR}
  DEPENDS ${GODHeaders}
  COMMENT "run iwyu"
  )

# TODO: exclude dicts
# TODO: exclude GOD-generated classes

file(WRITE ${CMAKE_BINARY_DIR}/strip-json.py "#!/bin/python
import json
import re
with open('${CMAKE_BINARY_DIR}/compile_commands.json') as f:
    jjj = json.load(f)
jm = []
for j in jjj:
    if re.match('.*Dict.cpp',j['file']) is None:
        jm.append([j])
for j in jm:
    j['command'] = re.sub('-Wall','',j['command'])
    j['command'] = re.sub('-Wextra','',j['command'])
    j['command'] = re.sub('-fabi-version=0','',j['command'])
    j['command'] = re.sub('-Wsuggest-override','-Wno-gnu-include-next -Wno-c99-extensions -Wno-unkown-warning-option -I/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-centos7/lib/clang/3.9.0/include/',j['command'])

with open('${IWYU_WORK_DIR}/compile_commands.json','w') as outfile:
    json.dump(jm, outfile)")

add_custom_target(manipulate_compilation_database
  COMMAND python ${CMAKE_BINARY_DIR}/strip-json.py
  WORKING_DIR=${CMAKE_BINARY_DIR}
  DEPENDS configure
  )
add_dependency(run-iwyu manipulate_compilation_database)
