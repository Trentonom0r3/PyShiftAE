// SuiteManager.h
#pragma once

#include "AEGP_SuiteHandler.h"
#include "AE_GeneralPlugPanels.h"
#include "SuiteHelper.h"#
/*
 * File: SuiteManager.h
 * Description: Singleton class managing the After Effects suite handler and plugin ID.
 *
 * Guidelines for Contributors:
 * 1. Singleton Pattern: Recognize that SuiteManager is a singleton and should not be instantiated directly.
 * 2. Suite Handling: Understand how SuiteManager provides access to AE suites.
 * 3. No Alteration: Do not modify this file. It is crucial for the stable operation of the entire plugin.
 */
template <>
const A_char* SuiteTraits<AEGP_PanelSuite1>::i_name = kAEGPPanelSuite;
template <>
const int32_t SuiteTraits<AEGP_PanelSuite1>::i_version = kAEGPPanelSuiteVersion1;

class SuiteManager {
public:
    // Gets the singleton instance of SuiteManager
    static SuiteManager& GetInstance() {
        static SuiteManager instance;
        return instance;
    }

    // Deleted copy constructor and assignment operator to ensure singleton
    SuiteManager(SuiteManager const&) = delete;
    void operator=(SuiteManager const&) = delete;

    // Method to initialize the suite handler
    void InitializeSuiteHandler(SPBasicSuite* pica_basicP) {
        if (!suitesInitialized) {
            suites = new AEGP_SuiteHandler(pica_basicP);
            suitesInitialized = true;
        }
    }

    void InitializePanelSuiteHandler(SPBasicSuite* pica_basicP) {
        if (!panelSuitesInitialized) {
            panelSuites = new SuiteHelper<AEGP_PanelSuite1>(pica_basicP);
            panelSuitesInitialized = true;  
		}
	}
    // Method to get the suite handler
    AEGP_SuiteHandler& GetSuiteHandler() {
        return *suites;
    }

    SuiteHelper<AEGP_PanelSuite1>& GetPanelSuiteHandler() {
		return *panelSuites;
	}

    // Method to set the plugin ID
    void SetPluginID(AEGP_PluginID* pluginIDPtr) {
        this->pluginIDPtr = pluginIDPtr;
    }

    // Method to get the plugin ID
    AEGP_PluginID* GetPluginID() const {
        return pluginIDPtr;
    }

private:
    SuiteManager() : suites(nullptr), suitesInitialized(false), pluginIDPtr(nullptr) {}

    AEGP_SuiteHandler* suites;
    SuiteHelper<AEGP_PanelSuite1>* panelSuites;
    bool suitesInitialized;
    bool panelSuitesInitialized;
    AEGP_PluginID* pluginIDPtr;
};
