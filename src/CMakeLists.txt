set(RCPLIB_NAME RabbitControl)

# to compile universal binary on macos
set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
SET(CMAKE_DEBUG_POSTFIX "-d")

if (MSVC)
    add_compile_options(-bigobj)
endif()

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()


set(LIB_HEADERS

  TypeDefinition.h
  Parameter.h
  TypeDefinitionString.h
  TypeDefinitionDefault.h
  GroupParameter.h
  ValueParameter.h
  IGroupParameter.h
  IParameter.h
  StreamTools.h
  Option.h
  ParameterListener.h
  TypeDefinitionNumber.h
  TypeDefinitionURI.h
  ClientTransporter.h
  Color.h
  Ip.h
  ServerTransporter.h
  StreamWriter.h
  StringStreamWriter.h
  Vector2.h
  Vector3.h
  Vector4.h
  Writeable.h
  Writer.h
  Range.h
  ParameterManager.h
  ParameterServer.h
  ParameterClient.h
  TypeHelper.h
  Version.h
  ColorFloat.h
  MemoryTools.h

  0.1.0/types.h
  0.1.0/TypeDefinitions.h
  0.1.0/ParameterDefinitions.h
  0.1.0/GroupParameter.h
  0.1.0/ParameterParser.h
  0.1.0/ParameterFactory.h
  0.1.0/TypeDefinitionNumber.h
  0.1.0/ParameterWriter.h
  0.1.0/TypeDefinitionString.h
  0.1.0/v010StreamTools.h
  0.1.0/TypeDefinitionBoolean.h
  0.1.0/TypeDefinitionGroup.h
  0.1.0/TypeDefinitionBang.h
  0.1.0/TypeDefinitionEnum.h
  0.1.0/TinyString.h
  0.1.0/TypeDefinitionURI.h
  0.1.0/TypeDefinitionRange.h
  0.1.0/TypeUtils.h
  0.1.0/TypeDefinitionRGB.h
  0.1.0/TypeDefinitionIPv4.h
  0.1.0/TypeDefinitionIPv6.h
  0.1.0/TypeDefinitionCustom.h
  0.1.0/CustomParameter.h
  0.1.0/ValueParameter.h
  0.1.0/ParameterServer.h
  0.1.0/IdData.h
  0.1.0/InfoData.h
  0.1.0/Version.h
  0.1.0/ParameterClient.h
  0.1.0/ParameterDumper.h
  0.1.0/PacketParser.h
  0.1.0/BangParameter.h
  0.1.0/TypeDefinitionDefault.h
  0.1.0/PacketWriter.h

  1.0/types.h
  1.0/RcpInt.h
  1.0/RcpVersion.h
  1.0/RcpString.h
  1.0/RcpLanguageString.h
  1.0/InfoData.h
  1.0/TypeDefinitions.h
  1.0/TypeDefinitionNumber.h
  1.0/PacketParser.h
  1.0/TypeDefinitionNoOptions.h
  1.0/TypeDefinitionBoolean.h
  1.0/ParameterDefinitions.h
  1.0/ValueParameter.h
  1.0/TypeUtils.h
  1.0/ParameterWriter.h
  1.0/IParameter.h
  1.0/Parameter.h
  1.0/GroupParameter.h
  1.0/TypeDefinitionDefault.h
  1.0/TypeDefinitionString.h
  1.0/ParameterParser.h
  1.0/ParameterFactory.h
  1.0/BangParameter.h
  1.0/TypeDefinitionRange.h
  1.0/CustomParameter.h
  1.0/TypeDefinitionCustom.h
  1.0/TypeDefinitionIPv4.h
  1.0/TypeDefinitionIPv6.h
  1.0/TypeDefinitionEnum.h
  1.0/TypeDefinitionRGB.h
  1.0/TypeDefinitionURI.h
  1.0/TypeDefinitionRGBFloat.h
  1.0/ParameterServer.h
  1.0/ParameterClient.h
  1.0/ParameterClientState.h
  1.0/Version.h
  1.0/PacketWriter.h
)


add_library(RabbitControl SHARED

  rcp-cpp_global.h

  ${LIB_HEADERS}

  StreamTools.cpp  
  Color.cpp
  Ip.cpp
  StreamWriter.cpp
  StringStreamWriter.cpp  
  Writeable.cpp
  Writer.cpp  
  ParameterServer.cpp
  ParameterClient.cpp  
  ColorFloat.cpp  
  MemoryTools.cpp

  0.1.0/ParameterParser.cpp
  0.1.0/ParameterFactory.cpp
  0.1.0/ParameterWriter.cpp
  0.1.0/ParameterServer.cpp
  0.1.0/ParameterClient.cpp
  0.1.0/ParameterDumper.cpp

  1.0/RcpVersion.cpp
  1.0/InfoData.cpp
  1.0/ParameterWriter.cpp
  1.0/ParameterParser.cpp
  1.0/ParameterFactory.cpp
  1.0/ParameterServer.cpp
  1.0/ParameterClient.cpp
  1.0/ParameterClientState.cpp
)

target_include_directories(${RCPLIB_NAME} PRIVATE .)
target_compile_definitions(${RCPLIB_NAME} PRIVATE RCP_CPP_LIBRARY)

set_target_properties(${RCPLIB_NAME} PROPERTIES
  # "current version" in semantic format in Mach-O binary file
  VERSION 1.0.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  # PUBLIC_HEADER "${LIB_HEADERS}"
)

install(TARGETS ${RCPLIB_NAME}
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
  FRAMEWORK DESTINATION ${CMAKE_BINARY_DIR}
  #PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include
)

# install public header
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/"
        DESTINATION "${CMAKE_BINARY_DIR}/include/rcp"
        FILES_MATCHING
        PATTERN "*.h"
)


