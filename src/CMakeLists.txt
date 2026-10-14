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

  include/rcp/TypeDefinition.h
  include/rcp/Parameter.h
  include/rcp/TypeDefinitionString.h
  include/rcp/TypeDefinitionDefault.h
  include/rcp/GroupParameter.h
  include/rcp/ValueParameter.h
  include/rcp/IGroupParameter.h
  include/rcp/IParameter.h
  include/rcp/StreamTools.h
  include/rcp/Option.h
  include/rcp/ParameterListener.h
  include/rcp/TypeDefinitionNumber.h
  include/rcp/TypeDefinitionURI.h
  include/rcp/ClientTransporter.h
  include/rcp/Color.h
  include/rcp/Ip.h
  include/rcp/ServerTransporter.h
  include/rcp/StreamWriter.h
  include/rcp/StringStreamWriter.h
  include/rcp/Vector2.h
  include/rcp/Vector3.h
  include/rcp/Vector4.h
  include/rcp/Writeable.h
  include/rcp/Writer.h
  include/rcp/Range.h
  include/rcp/ParameterManager.h
  include/rcp/ParameterServer.h
  include/rcp/ParameterClient.h
  include/rcp/TypeHelper.h
  include/rcp/Version.h
  include/rcp/ColorFloat.h
  include/rcp/MemoryTools.h

  include/rcp/0.1.0/types.h
  include/rcp/0.1.0/TypeDefinitions.h
  include/rcp/0.1.0/ParameterDefinitions.h
  include/rcp/0.1.0/GroupParameter.h
  include/rcp/0.1.0/ParameterParser.h
  include/rcp/0.1.0/ParameterFactory.h
  include/rcp/0.1.0/TypeDefinitionNumber.h
  include/rcp/0.1.0/ParameterWriter.h
  include/rcp/0.1.0/TypeDefinitionString.h
  include/rcp/0.1.0/v010StreamTools.h
  include/rcp/0.1.0/TypeDefinitionBoolean.h
  include/rcp/0.1.0/TypeDefinitionGroup.h
  include/rcp/0.1.0/TypeDefinitionBang.h
  include/rcp/0.1.0/TypeDefinitionEnum.h
  include/rcp/0.1.0/TinyString.h
  include/rcp/0.1.0/TypeDefinitionURI.h
  include/rcp/0.1.0/TypeDefinitionRange.h
  include/rcp/0.1.0/TypeUtils.h
  include/rcp/0.1.0/TypeDefinitionRGB.h
  include/rcp/0.1.0/TypeDefinitionIPv4.h
  include/rcp/0.1.0/TypeDefinitionIPv6.h
  include/rcp/0.1.0/TypeDefinitionCustom.h
  include/rcp/0.1.0/CustomParameter.h
  include/rcp/0.1.0/ValueParameter.h
  include/rcp/0.1.0/ParameterServer.h
  include/rcp/0.1.0/IdData.h
  include/rcp/0.1.0/InfoData.h
  include/rcp/0.1.0/Version.h
  include/rcp/0.1.0/ParameterClient.h
  include/rcp/0.1.0/ParameterDumper.h
  include/rcp/0.1.0/PacketParser.h
  include/rcp/0.1.0/BangParameter.h
  include/rcp/0.1.0/TypeDefinitionDefault.h
  include/rcp/0.1.0/PacketWriter.h

  include/rcp/1.0/types.h
  include/rcp/1.0/RcpInt.h
  include/rcp/1.0/RcpVersion.h
  include/rcp/1.0/RcpString.h
  include/rcp/1.0/RcpLanguageString.h
  include/rcp/1.0/InfoData.h
  include/rcp/1.0/TypeDefinitions.h
  include/rcp/1.0/TypeDefinitionNumber.h
  include/rcp/1.0/PacketParser.h
  include/rcp/1.0/TypeDefinitionNoOptions.h
  include/rcp/1.0/TypeDefinitionBoolean.h
  include/rcp/1.0/ParameterDefinitions.h
  include/rcp/1.0/ValueParameter.h
  include/rcp/1.0/TypeUtils.h
  include/rcp/1.0/ParameterWriter.h
  include/rcp/1.0/IParameter.h
  include/rcp/1.0/Parameter.h
  include/rcp/1.0/GroupParameter.h
  include/rcp/1.0/TypeDefinitionDefault.h
  include/rcp/1.0/TypeDefinitionString.h
  include/rcp/1.0/ParameterParser.h
  include/rcp/1.0/ParameterFactory.h
  include/rcp/1.0/BangParameter.h
  include/rcp/1.0/TypeDefinitionRange.h
  include/rcp/1.0/CustomParameter.h
  include/rcp/1.0/TypeDefinitionCustom.h
  include/rcp/1.0/TypeDefinitionIPv4.h
  include/rcp/1.0/TypeDefinitionIPv6.h
  include/rcp/1.0/TypeDefinitionEnum.h
  include/rcp/1.0/TypeDefinitionRGB.h
  include/rcp/1.0/TypeDefinitionURI.h
  include/rcp/1.0/TypeDefinitionRGBFloat.h
  include/rcp/1.0/ParameterServer.h
  include/rcp/1.0/ParameterClient.h
  include/rcp/1.0/ParameterClientState.h
  include/rcp/1.0/Version.h
  include/rcp/1.0/PacketWriter.h
  include/rcp/1.0/EnumParameter.h
)


set(LIB_SOURCES
  StreamTools.cpp
  Color.cpp
  Ip.cpp
  StreamWriter.cpp
  StringStreamWriter.cpp
  Writeable.cpp

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


include_directories(include)


add_library(objlib OBJECT ${LIB_SOURCES} ${LIB_HEADERS})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)



if(BUILD_SHARED)
  add_library(${RCPLIB_NAME} SHARED $<TARGET_OBJECTS:objlib>)
  target_compile_definitions(${RCPLIB_NAME} PRIVATE RCP_CPP_LIBRARY)

  set_target_properties(${RCPLIB_NAME} PROPERTIES
    # "current version" in semantic format in Mach-O binary file
    VERSION 1.0.0
    # "compatibility version" in semantic format in Mach-O binary file
    SOVERSION 1.0.0
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    PUBLIC_HEADER "${LIB_HEADERS}"
  )

  install(TARGETS ${RCPLIB_NAME}
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
    FRAMEWORK DESTINATION ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include
  )

endif()

if(BUILD_STATIC)
  add_library(${RCPLIB_NAME}Static STATIC $<TARGET_OBJECTS:objlib>)
  target_compile_definitions(${RCPLIB_NAME}Static PRIVATE RCP_CPP_LIBRARY)

  install(TARGETS ${RCPLIB_NAME}Static
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
    FRAMEWORK DESTINATION ${CMAKE_BINARY_DIR}
    # PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include
  )
endif()


# install public header
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/include/"
        DESTINATION "${CMAKE_BINARY_DIR}/include/"
        FILES_MATCHING
        PATTERN "*.h"
)


