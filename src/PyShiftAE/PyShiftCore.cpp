#include "PyShiftCore.h"

// PyShiftCore.Cpp
/*
This is the core Implementation File. May Refactor into individual component files as
the library expands, but keeping everything here for now while I figure it all out.
*/
/*ITEM*/
// Constructor implementations
Item::Item(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle)
    : suites_(suites), itemHandle_(itemHandle) {
    populateAttributes();
}
// Implementations of populateAttributes method
void Item::populateAttributes() {
    name = getName();
}

// Method implementations for Item class
std::string Item::getName() const {
    AEGP_MemHandle unicode_nameMH = NULL;
    A_UTF16Char* unicode_nameP = NULL;
    std::string name;

    A_Err err = suites_->ItemSuite8()->AEGP_GetItemName(PyShiftAE, itemHandle_, &unicode_nameMH);

    if (err == A_Err_NONE && unicode_nameMH != NULL) {
        // Lock the memory handle to get the string pointer
        suites_->MemorySuite1()->AEGP_LockMemHandle(unicode_nameMH, (void**)&unicode_nameP);

        // Convert UTF-16 to UTF-8
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        name = convert.to_bytes((char16_t*)unicode_nameP);

        // Unlock and dispose of the memory handle
        suites_->MemorySuite1()->AEGP_UnlockMemHandle(unicode_nameMH);
        suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_nameMH);
    }

    return name; // Return the name as a UTF-8 string
}


void Item::setName(const std::string& name) {
    // Convert UTF-8 to UTF-16
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string unicode_name = convert.from_bytes(name);

    // Call the AE SDK function to set the item name
    A_Err err = suites_->ItemSuite8()->AEGP_SetItemName(itemHandle_, reinterpret_cast<const A_UTF16Char*>(unicode_name.c_str()));

    // Handle potential errors here
    if (err != A_Err_NONE) {
        // Error handling logic
    }
}

float Item::time() const {
    A_Time curr_timeT; // This will store the current time as A_Time.

    // Get the current time in A_Time.
    A_Err err = suites_->ItemSuite6()->AEGP_GetItemCurrentTime(itemHandle_, &curr_timeT);
    if (err == A_Err_NONE) {
        // Convert A_Time to float.
        return static_cast<float>(curr_timeT.value) / curr_timeT.scale;
    }
    return 0.0f; // In case of error, return 0 time.
}

float Item::duration() const {
    A_Time durationT; // This will store the duration as A_Time.

    // Get the duration in A_Time.
    A_Err err = suites_->ItemSuite6()->AEGP_GetItemDuration(itemHandle_, &durationT);
    if (err == A_Err_NONE) {
        // Convert A_Time to float.
        return static_cast<float>(durationT.value) / durationT.scale;
    }
    return 0.0f; // In case of error, return 0 duration.
}

/*ITEM END*/

FootageItem::FootageItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle) : Item(suites, itemHandle) {}

FolderItem::FolderItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle) : Item(suites, itemHandle) {}

/*COMPITEM*/
CompItem::CompItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemHandle)
    : Item(suites, itemHandle) {}
// Method implementations for CompItem class

ImageData CompItem::frameAtTime(float time) {
    AEGP_ItemH active_itemH = NULL;
    AEGP_RenderOptionsH roH = NULL;
    A_Err err = A_Err_NONE,
        err2 = A_Err_NONE;

    ERR(suites_->ItemSuite6()->AEGP_GetActiveItem(&active_itemH));

    if (active_itemH) {
        ERR(suites_->RenderOptionsSuite1()->AEGP_NewFromItem(PyShiftAE, active_itemH, &roH));

        if (!err && roH) {
            AEGP_FrameReceiptH receiptH = NULL;
            AEGP_WorldH frameH = NULL;
            A_Time timeT;
            timeT.value = static_cast<A_long>(time * 1000000);  // Convert seconds to microseconds.
            timeT.scale = 1000000;  // Set the scale factor to 1,000,000.
            AEGP_WorldType type = AEGP_WorldType_NONE;

            ERR(suites_->RenderOptionsSuite1()->AEGP_SetTime(roH, timeT));  // Set render time.
            ERR(suites_->RenderOptionsSuite1()->AEGP_GetWorldType(roH, &type));
            ERR(suites_->RenderSuite2()->AEGP_RenderAndCheckoutFrame(roH, NULL, NULL, &receiptH));

            if (receiptH) {
                ERR(suites_->RenderSuite2()->AEGP_GetReceiptWorld(receiptH, &frameH));

                if (!err && frameH) {
                    PF_Pixel8* baseAddr = nullptr;
                    ERR(suites_->WorldSuite3()->AEGP_GetBaseAddr8(frameH, &baseAddr));

                    if (!err && baseAddr) {
                        int width, height;
                        ERR(suites_->WorldSuite3()->AEGP_GetSize(frameH, &width, &height));

                        // Determine the total size of the image data in bytes.
                        size_t dataSize = width * height * 4;  // Assumes 4 bytes per pixel.

                        // Create a vector to hold the image data.
                        std::vector<uint8_t> img(dataSize);

                        // Manually copy the data from baseAddr to img.
                        std::memcpy(img.data(), baseAddr, dataSize);

                        // Convert ARGB (BGRA in memory on little-endian machines) to RGBA.
                        for (size_t i = 0; i < dataSize; i += 4) {
                            // Swap the blue (B) and the alpha (A).
                            std::swap(img[i], img[i + 3]);
                            // Swap the green (G) and blue (B).
                            std::swap(img[i + 1], img[i + 2]);
                        }

                        ERR2(suites_->RenderSuite2()->AEGP_CheckinFrame(receiptH));

                        // Create an ImageData object to hold the image data and dimensions.
                        ImageData image_data{ std::move(img), width, height, 4 };

                        return image_data;  // Return the populated ImageData object.
                    }

                }

                ERR2(suites_->RenderSuite2()->AEGP_CheckinFrame(receiptH));
            }
        }

        ERR(suites_->RenderOptionsSuite1()->AEGP_Dispose(roH));
    }

    return ImageData{};  // Return an empty vector if something goes wrong.
}

void CompItem::replaceFrameAtTime(ImageData& new_img, float time) {
    AEGP_ItemH active_itemH = NULL;
    AEGP_RenderOptionsH roH = NULL;
    A_Err err = A_Err_NONE,
         err2 = A_Err_NONE;

    ERR(suites_->ItemSuite6()->AEGP_GetActiveItem(&active_itemH));

    if (active_itemH) {
        ERR(suites_->RenderOptionsSuite1()->AEGP_NewFromItem(PyShiftAE, active_itemH, &roH));

        if (!err && roH) {
            AEGP_FrameReceiptH receiptH = NULL;
            AEGP_WorldH frameH = NULL;
            A_Time timeT;
            timeT.value = static_cast<A_long>(time * 1000000);  // Convert seconds to microseconds.
            timeT.scale = 1000000;  // Set the scale factor to 1,000,000.

            ERR(suites_->RenderOptionsSuite1()->AEGP_SetTime(roH, timeT));  // Set render time.
            ERR(suites_->RenderSuite2()->AEGP_RenderAndCheckoutFrame(roH, NULL, NULL, &receiptH));

            if (receiptH) {
                ERR(suites_->RenderSuite2()->AEGP_GetReceiptWorld(receiptH, &frameH));

                if (!err && frameH) {
                    PF_Pixel8* baseAddr = nullptr;
                    ERR(suites_->WorldSuite3()->AEGP_GetBaseAddr8(frameH, &baseAddr));

                    if (!err && baseAddr) {
                        int width, height;
                        ERR(suites_->WorldSuite3()->AEGP_GetSize(frameH, &width, &height));

                        // Ensure the dimensions match.
                        if (width == new_img.width && height == new_img.height) {
                            // Determine the total size of the image data in bytes.
                            size_t dataSize = static_cast<size_t>(width) * height * new_img.channels; // Use the number of channels in new_img.

                            // Manually copy the data from new_img to baseAddr.
                            std::memcpy(baseAddr, new_img.data.data(), dataSize);
                        }
                        else {
                            // Handle the error for mismatched dimensions.
                            err = A_Err_GENERIC;
                        }

                        ERR2(suites_->RenderSuite2()->AEGP_CheckinFrame(receiptH));
                    }
                }

                if (err) {
                    ERR2(suites_->RenderSuite2()->AEGP_CheckinFrame(receiptH));
                }
            }
        }

        ERR(suites_->RenderOptionsSuite1()->AEGP_Dispose(roH));
    }
}



/* THE PROJECT CLASS*/
Project::Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH)
    : suites_(suites), projH_(projH) {  // Initializes member variables with constructor arguments.
    populateAttributes();  // Calls method to populate attribute values.
}

void Project::populateAttributes() {
    name = getName();  // Gets the project name using the getName method and sets it as the name attribute.
    path = GetProjectPath();  // Gets the project path using the GetProjectPath method and sets it as the path attribute.
    //items = std::make_unique<ItemCollection>(&suites_, projH_);
    AEGP_ItemH item;
    suites_->ItemSuite8()->AEGP_GetActiveItem(&item);
    activeItem = createItem(suites_, item);
}

std::string Project::getName() const {  // Method to get the project name.
    A_char nameZ[AEGP_MAX_PROJ_NAME_SIZE];  // Array to hold the project name.
    A_Err err = suites_->ProjSuite6()->AEGP_GetProjectName(projH_, nameZ);  // Calls the AEGP_GetProjectName function to get the project name.
    if (err) {
        throw std::runtime_error("Error obtaining project name");  // Throws an exception if there's an error.
    }
    return std::string(nameZ);  // Converts the project name to a std::string and returns it.
}

std::string Project::GetProjectPath() const {  // Method to get the project path.
    AEGP_MemHandle unicode_pathPH = NULL;  // Handle for memory allocation.
    AEGP_MemSize resultSize = 0;  // Variable to hold the size of the allocated memory.
    A_UTF16Char* resultStr = NULL;  // Pointer to hold the project path string.

    A_Err err = suites_->ProjSuite6()->AEGP_GetProjectPath(projH_, &unicode_pathPH);  // Calls the AEGP_GetProjectPath function to get the project path.

    if (err) {
        return "";  // Returns an empty string if there's an error.
    }

    // Gets the size of the memory handle.
    suites_->MemorySuite1()->AEGP_GetMemHandleSize(unicode_pathPH, &resultSize);

    // Locks the memory handle to make it safe to dereference.
    if (suites_->MemorySuite1()->AEGP_LockMemHandle(unicode_pathPH, (void**)&resultStr) == A_Err_NONE) {
        // Converts the UTF-16 string to a UTF-8 std::string.
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        std::string utf8Result = convert.to_bytes((char16_t*)resultStr, (char16_t*)(resultStr + resultSize / sizeof(A_UTF16Char)));

        // Unlocks the memory handle.
        suites_->MemorySuite1()->AEGP_UnlockMemHandle(unicode_pathPH);

        // Frees the memory handle.
        suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

        return utf8Result;  // Returns the project path as a UTF-8 string.
    }
    else {
        // Failed to lock the memory handle, so handles the error case.

        // Frees the memory handle.
        suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

        return "";  // Returns an empty string.
    }
}

const Item& Project::getActiveItem() const
{
    return *activeItem;
}

std::unique_ptr<Item> Project::createItem(AEGP_SuiteHandler* suites, AEGP_ItemH& itemH) {
    AEGP_ItemType itemType;
    suites->ItemSuite8()->AEGP_GetItemType(itemH, &itemType);

    switch (itemType) {
    case AEGP_ItemType_FOLDER:
        return std::make_unique<FolderItem>(suites, itemH);
    case AEGP_ItemType_COMP:
        return std::make_unique<CompItem>(suites, itemH);
    case AEGP_ItemType_FOOTAGE:
        return std::make_unique<FootageItem>(suites, itemH);
    default:
        return nullptr;  // Or throw an exception, or handle the error in some other way
    }
}


// Implementation of the App class.
App::App(AEGP_SuiteHandler& suites) : suites_(suites) {  // Initializes member variables with constructor arguments.
    populateAttributes();  // Calls method to populate attribute values.
}

void App::populateAttributes() {
    version = getVersion();  // Gets the app version using the getVersion method and sets it as the version attribute.
    AEGP_ProjectH projH;
    suites_.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH);  // Gets the project handle for the current project (assuming only one project).
    project = std::make_unique<Project>(&suites_, projH);  // Creates a new Project instance and sets it as the project attribute.
 }

std::string App::getVersion() const {  // Method to get the app version.
    return std::string("1.0.0");  // Returns a hardcoded version string.
}

const Project& App::getProject() const {
    return *project;  // Dereferences the unique_ptr to the Project instance and returns it as a reference.
}

// Implementation for starting an undo group.
void App::startUndoGroup(const std::string& undo_name) {
    // Call the AEGP_StartUndoGroup function from the After Effects SDK.
    // Convert the std::string to a C-style string using c_str().
    suites_.UtilitySuite5()->AEGP_StartUndoGroup(undo_name.c_str());
}

// Implementation for ending an undo group.
void App::endUndoGroup() {
    // Call the AEGP_EndUndoGroup function from the After Effects SDK.
    suites_.UtilitySuite5()->AEGP_EndUndoGroup();
}

void App::executeCommand(const int commandId) {
    AEGP_Command cmd = commandId;
    suites_.CommandSuite1()->AEGP_DoCommand(cmd);
}
