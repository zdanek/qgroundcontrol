
if(QGC_AIRMAP)
message(STATUS "Building Airmap")
	add_library(Airmap
		AirMapAdvisoryManagerasdfsa.cc
		AirMapFlightManager.cc
		AirMapFlightPlanManager.cc
		AirMapManager.cc
		AirMapRestrictionManager.cc
		AirMapRulesetsManager.cc
		AirMapSettings.cc
		AirMapSharedState.cc
		AirMapTelemetry.cc
		AirMapTrafficMonitor.cc
		AirMapVehicleManager.cc
		AirMapWeatherInfoManager.cc

		airmap.qrc
	)
	add_custom_target(AirmapQml
		SOURCES
		AirmapSettings.qml
		AirspaceRegulation.qml
		ComplianceRules.qml
		FlightDetails.qml
		RuleSelector.qml
		AirspaceControl.qml
		AirspaceWeather.qml
		FlightBrief.qml
		FlightFeature.qml
	)
else()
	message(STATUS "Building Airmap dummy")
	add_library(Airmap
		dummy/AirspaceManager.cc
		dummy/airmap_dummy.qrc
	)
	target_include_directories(Airmap PUBLIC dummy)
endif()

target_link_libraries(Airmap
	Qt5::Core
	Qt5::Location
	Qt5::Widgets

	qgc
)

target_include_directories(Airmap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

