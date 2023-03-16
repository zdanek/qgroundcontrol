
if(QGC_AIRMAP)
	add_library(AirspaceManagement
		AirspaceAdvisoryProvider.cc
		AirspaceFlightPlanProvider.cc
		AirspaceManager.cc
		AirspaceRestriction.cc
		AirspaceRestrictionProvider.cc
		AirspaceRulesetsProvider.cc
		AirspaceVehicleManager.cc
		AirspaceWeatherInfoProvider.cc
	)

	target_include_directories(AirspaceManagement INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

	target_link_libraries(AirspaceManagement
		PRIVATE
			QmlControls
			Qt5::Core
	)
endif()

