// SuiteManager.h
#pragma once

#include "AEGP_SuiteHandler.h"

/*
 * File: SuiteManager.h
 * Description: Singleton class managing the After Effects suite handler and plugin ID.
 *
 * Guidelines for Contributors:
 * 1. Singleton Pattern: Recognize that SuiteManager is a singleton and should not be instantiated directly.
 * 2. Suite Handling: Understand how SuiteManager provides access to AE suites.
 * 3. No Alteration: Do not modify this file. It is crucial for the stable operation of the entire plugin.
 */


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

    // Method to get the suite handler
    AEGP_SuiteHandler& GetSuiteHandler() {
        return *suites;
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
    bool suitesInitialized;
    AEGP_PluginID* pluginIDPtr;
};
