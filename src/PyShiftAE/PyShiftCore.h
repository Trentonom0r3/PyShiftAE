#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING  // Suppress deprecation warnings

#pragma once
//PyShiftCore.h
#include <string>
#include <stdexcept>
#include <memory>  // Include for std::unique_ptr
#include <codecvt>
#include "AEGP_SuiteHandler.h"
#include <vector>
static AEGP_PluginID		PyShiftAE = 10L;

class Item {
public:
    // Constructor and Destructor
    explicit Item(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle) : suites_(suites), itemHandle_(itemHandle) {}
    virtual ~Item() = default;

    // Member functions for Item
    virtual std::string getType() const {
        return "Item";
    }

    std::string getName() const;
    void setName(const std::string& name);

    std::string getComment() const;
    void setComment(const std::string& comment);

    bool isSelected() const;
    void setSelected(bool isSelected);

    int getLabel() const;
    void setLabel(int labelColor);

    int addGuide(int orientationType, double position);  // Assuming the return type is an int representing the guide's ID/index
    void removeGuide(int guideIndex);
    void setGuide(double position, int guideIndex);

    void remove();

protected:
    AEGP_SuiteHandler* suites_;
    AEGP_ItemH itemHandle_;

    // Assuming that the following attributes are common to all items
    int id_;
    std::string name_;
    std::string comment_;
    bool selected_;
    int label_;
};

class FootageItem : public Item {
public:
    // Constructors and Destructors
    explicit FootageItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~FootageItem() = default;
    virtual std::string getType() const override {
        return "Footage";
    }
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

class CompItem : public Item {
public:
    // Constructors and Destructors
    explicit CompItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~CompItem() = default;

protected:
    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};

class SolidItem : public Item {
public:
    // Constructors and Destructors
    explicit SolidItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle);
    virtual ~SolidItem() = default;

protected:
    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};
class ItemCollection {
public:
    ItemCollection(AEGP_SuiteHandler* suites, AEGP_ProjectH* projectH);


private:
    void populateItems();

    AEGP_SuiteHandler* suites_;
    AEGP_ProjectH* projH_;  // Now a pointer.
    std::vector<std::unique_ptr<Item>> items_;
};


//Project class definition
class Project {
public:
    Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH);  // Change to pointer

    std::string getName() const;
    std::string GetProjectPath() const;
    const Item& getActiveItem() const;
    const ItemCollection& getItems() const;

    std::unique_ptr<Item> activeItem;
    std::unique_ptr<ItemCollection> items;
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
