
set(EXTRA_SRC)
if(BUILD_TESTING)
	list(APPEND EXTRA_SRC
		MockLink.cc
		MockLink.h
		MockLinkFTP.cc
		MockLinkFTP.h
		MockLinkMissionItemHandler.cc
		MockLinkMissionItemHandler.h
	)
endif()

if (NOT NO_SERIAL_LINK)
	list(APPEND EXTRA_SRC
		QGCSerialPortInfo.cc
		QGCSerialPortInfo.h
		SerialLink.cc
		SerialLink.h
	)
endif ()

if (QGC_ENABLE_BLUETOOTH)
	list(APPEND EXTRA_SRC
		BluetoothLink.cc
		BluetoothLink.h
	)
endif()

add_library(comm
	LinkConfiguration.cc
	LinkConfiguration.h
	LinkInterface.cc
	LinkInterface.h
	LinkManager.cc
	LinkManager.h
	LogReplayLink.cc
	LogReplayLink.h
	MavlinkMessagesTimer.cc
	MavlinkMessagesTimer.h
	MAVLinkProtocol.cc
	MAVLinkProtocol.h
	QGCMAVLink.cc
	QGCMAVLink.h
	TCPLink.cc
	TCPLink.h
	UdpIODevice.cc
	UdpIODevice.h
	UDPLink.cc
	UDPLink.h

	${EXTRA_SRC}
)

target_link_libraries(comm
	PUBLIC
		qgc
		Qt5::Location
		Qt5::SerialPort
		Qt5::Test
		Qt5::TextToSpeech
		Qt5::Widgets
		qmdnsengine
)

target_include_directories(comm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
