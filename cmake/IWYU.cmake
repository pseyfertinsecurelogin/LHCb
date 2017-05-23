find_program(iwyu_tool NAMES iwyu_tool.py)
set(IWYU_MAPPING_FILE ${CMAKE_SOURCE_DIR}/cmake/include_rules.imp)
set(IWYU_WORK_DIR ${CMAKE_BINARY_DIR}/iwyu)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/iwyu)
if(iwyu_tool)
  message(FATAL "FOUND IWYU w/o PATH SETUP???")
  add_custom_target(run-iwyu
    COMMAND ${iwyu_tool} -p ${IWYU_WORK_DIR}
    -- --mapping_file=${IWYU_MAPPING_FILE} > ${IWYU_WORK_DIR}/iwyu.log 2>&1
    WORKING_DIR=${IWYU_WORK_DIR}
    DEPENDS ${GODHeaders}
    COMMENT "run iwyu"
    )
else()
  find_program(iwyu_tool NAMES iwyu_tool.py
    HINTS "/tmp/pseyfert/tmp.iCzgEqwMfJ/install/bin/")
  set(gcc_config_version 4.9.3)
  set(clang_config_version 3.9)
  set(LCGPLAT x86_64-centos7)
  set(LCG_lib_name lib64)
  set(LCG_contdir /afs/cern.ch/sw/lcg/external)
  set(LCG_gcc_home ${LCG_contdir}/gcc/${gcc_config_version}/${LCGPLAT})
  set(LCG_clang_home ${LCG_contdir}/llvm/${clang_config_version}/${LCGPLAT})
  if(DEFINED ENV{PATH})
    message(STATUS "extending PATH")
    set(iwyu_path ${LCG_clang_home}/bin:${LCG_gcc_home}/bin:/tmp/pseyfert/tmp.iCzgEqwMfJ/install/bin/:$ENV{PATH})
  else()
    message(FATAL "not extending PATH")
    set(iwyu_path ${LCG_clang_home}/bin:${LCG_gcc_home}/bin:/tmp/pseyfert/tmp.iCzgEqwMfJ/install/bin/)
  endif()
  if(DEFINED ENV{LD_LIBRARY_PATH})
    message(STATUS "extending LD_LIBRARY_PATH")
    set(iwyu_ld_path ${LCG_gcc_home}/${LCG_lib_name}:${LCG_clang_home}/lib:$ENV{LD_LIBRARY_PATH})
  else()
    message(FATAL "not extending LD_LIBRARY_PATH")
    set(iwyu_ld_path ${LCG_gcc_home}/${LCG_lib_name}:${LCG_clang_home}/lib)
  endif()

  add_custom_target(run-iwyu
    ${CMAKE_COMMAND} -E env
    PATH=${iwyu_path}
    LD_LIBRARY_PATH=${iwyu_ld_path}
    ${iwyu_tool} -p ${IWYU_WORK_DIR}
    -- --mapping_file=${IWYU_MAPPING_FILE} > ${IWYU_WORK_DIR}/iwyu.log 2>&1
    WORKING_DIR=${IWYU_WORK_DIR}
    DEPENDS ${GODHeaders}
    COMMENT "run iwyu"
    )
endif()

file(WRITE ${CMAKE_BINARY_DIR}/strip-json.py "#!/bin/python
godfiles=[]\n")
get_property(god_gen_file GLOBAL PROPERTY GOD_GENERATED_FILES)
foreach(genfile ${god_gen_file})
  file(APPEND ${CMAKE_BINARY_DIR}/strip-json.py
    "godfiles+=['${genfile}']\n")
endforeach(genfile)

file(APPEND ${CMAKE_BINARY_DIR}/strip-json.py "
import json
import re
with open('${CMAKE_BINARY_DIR}/compile_commands.json') as f:
    jjj = json.load(f)
jm = []
for j in jjj:
    if j['file'] not in godfiles and re.match('.*Dict.cpp',j['file']) is None:
        jm.append(j)
for j in jm:
    j['command'] = re.sub('-Wall','',j['command'])
    j['command'] = re.sub('-Wextra','',j['command'])
    j['command'] = re.sub('-fabi-version=0','',j['command'])
    j['command'] = re.sub('-Wsuggest-override','-Wno-gnu-include-next -Wno-c99-extensions -Wno-unknown-warning-option -I/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-centos7/lib/clang/3.9.0/include/',j['command'])

with open('${IWYU_WORK_DIR}/compile_commands.json','w') as outfile:
    json.dump(jm, outfile)")

add_custom_target(manipulate_compilation_database
  COMMAND python ${CMAKE_BINARY_DIR}/strip-json.py
  WORKING_DIR=${CMAKE_BINARY_DIR}
  )
add_dependencies(run-iwyu manipulate_compilation_database)
