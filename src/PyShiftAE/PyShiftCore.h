#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING  // Suppress deprecation warnings

#pragma once
//PyShiftCore.h
#include <string>
#include <stdexcept>
#include <memory>  // Include for std::unique_ptr
#include <codecvt>
#include "AEGP_SuiteHandler.h"
#include "AE_Macros.h"
#include <vector>
static AEGP_PluginID		PyShiftAE = 10L;


struct ImageData {
    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;
};


class Item {
public:
    explicit Item(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~Item() = default;

    std::string getName() const;
    void setName(const std::string& name);

    std::string name;
protected:
    void populateAttributes();

    AEGP_SuiteHandler* suites_;
    AEGP_ItemH itemHandle_;
    std::string name_;
};

class CompItem : public Item {
public:
    explicit CompItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~CompItem() = default;

    ImageData frameAtTime(float time);
    void replaceFrameAtTime(ImageData& new_img, float time);
    float getDuration() const;

    float Duration;
protected:
    //void populateAttributes();

    AEGP_SuiteHandler* suites_;
    AEGP_ItemH itemHandle_;
};

/*
std::unique_ptr<Item> createItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemH);

class FootageItem : public Item {
public:
    // Constructors and Destructors
    explicit FootageItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~FootageItem() = default;

protected:
    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};

class FolderItem : public Item {
public:
    // Constructors and Destructors
    explicit FolderItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~FolderItem() = default;

protected:
    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};

class ItemCollection {
public:
    // Constructor: Wraps AEGP_NewCollection
    ItemCollection(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH);

    // Destructor: Wraps AEGP_DisposeCollection
    ~ItemCollection();

    // Method to get the number of items in the collection
    int getNumItems() const;

    // Method to get an item by index
    const Item& getItemByIndex(int index) const;
    const Item& getItembyName(std::string name) const;

private:
    void populateItems();

    AEGP_SuiteHandler* suites_;
    AEGP_ProjectH projH_;
    std::vector<std::unique_ptr<Item>> items_;
};
*/

//Project class definition
class Project {
public:
    Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH);  // Change to pointer

    std::string getName() const;
    std::string GetProjectPath() const;
    const Item& getActiveItem() const;
    //const ItemCollection& getItems() const;

    std::unique_ptr<Item> activeItem;
    //std::unique_ptr<ItemCollection> items;
    std::string name;
    std::string path;

private:
    void populateAttributes();
    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};

class App {
public:
    App(AEGP_SuiteHandler& suites); //Constructor -- Initialize this first.
    std::string getVersion() const; //Get version as string
    const Project& getProject() const; // Return by reference

    std::string version; //Public member variable for version
    std::unique_ptr<Project> project; // Use a smart pointer
    // Start an undo group with the given name or a default name if none is provided.
    void startUndoGroup(const std::string& undo_name = "Default Undo Group Name");
    // End the current undo group.
    void endUndoGroup();
    void executeCommand(const int commandId);
private:
    void populateAttributes(); //Internal method to populate attributes
    AEGP_SuiteHandler& suites_; //Suite handler
};
