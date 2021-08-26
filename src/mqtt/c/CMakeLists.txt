## add build info
SET(CLIENT_VERSION 1.3.8)

STRING(TIMESTAMP BUILD_TIMESTAMP UTC)
MESSAGE(STATUS "Timestamp is ${BUILD_TIMESTAMP}")

## build options
SET(PAHO_WITH_SSL FALSE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
SET(PAHO_BUILD_SHARED FALSE CACHE BOOL "Build shared library")
SET(PAHO_BUILD_STATIC TRUE CACHE BOOL "Build static library")

IF (NOT PAHO_BUILD_SHARED AND NOT PAHO_BUILD_STATIC)
    MESSAGE(FATAL_ERROR "You must set either PAHO_BUILD_SHARED, PAHO_BUILD_STATIC, or both")
ENDIF()

SET(PAHO_HIGH_PERFORMANCE TRUE)
IF (PAHO_HIGH_PERFORMANCE)
    ADD_DEFINITIONS(-DHIGH_PERFORMANCE=1)
ENDIF()


## start to build
CONFIGURE_FILE(VersionInfo.h.in
        VersionInfo.h
        @ONLY
        )

SET(common_src
        MQTTTime.c
        MQTTProtocolClient.c
        Clients.c
        utf-8.c
        MQTTPacket.c
        MQTTPacketOut.c
        Messages.c
        Tree.c
        Socket.c
        Log.c
        MQTTPersistence.c
        Thread.c
        MQTTProtocolOut.c
        MQTTPersistenceDefault.c
        SocketBuffer.c
        LinkedList.c
        MQTTProperties.c
        MQTTReasonCodes.c
        Base64.c
        SHA1.c
        WebSocket.c
        )

IF (NOT PAHO_HIGH_PERFORMANCE)
    SET(common_src ${common_src}
            StackTrace.c
            Heap.c
            )
ENDIF()

SET(LIBS_SYSTEM c dl pthread rt)
# anl is only available with glibc so check if it is found before using
# it or build will fail on uclibc or musl
#[[FIND_LIBRARY(LIB_ANL anl)
IF(LIB_ANL)
    SET(LIBS_SYSTEM "${LIBS_SYSTEM} ${LIB_ANL}")
ENDIF()]]
ADD_DEFINITIONS(-D_GNU_SOURCE -fvisibility=hidden)

SET(PAHO_WITH_SSL FALSE)

IF (NOT PAHO_WITH_SSL)
    ADD_LIBRARY(paho-mqtt3c STATIC ${common_src} MQTTClient.c)
    ADD_LIBRARY(paho-mqtt3a STATIC ${common_src} MQTTAsync.c MQTTAsyncUtils.c)
    TARGET_LINK_LIBRARIES(paho-mqtt3c PUBLIC ${LIBS_SYSTEM})
    TARGET_LINK_LIBRARIES(paho-mqtt3a PUBLIC ${LIBS_SYSTEM})
ELSE()
    ADD_DEFINITIONS(-DOPENSSL=1 -DPAHO_MQTT_STATIC=1)
    ADD_LIBRARY(paho-mqtt3c STATIC ${common_src} MQTTClient.c SSLSocket.c)
    ADD_LIBRARY(paho-mqtt3a STATIC ${common_src} MQTTAsync.c MQTTAsyncUtils.c SSLSocket.c)

    SET(MQTTCLIENT_ENTRY_POINT MQTTClient_init)
    SET(MQTTASYNC_ENTRY_POINT MQTTAsync_init)
    SET_TARGET_PROPERTIES(
            paho-mqtt3c PROPERTIES
            LINK_FLAGS "-Wl,-init,${MQTTCLIENT_ENTRY_POINT}")
    SET_TARGET_PROPERTIES(
            paho-mqtt3a PROPERTIES
            LINK_FLAGS "-Wl,-init,${MQTTASYNC_ENTRY_POINT}")
ENDIF()