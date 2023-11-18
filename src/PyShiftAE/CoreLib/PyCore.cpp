#include "PyCore.h"

/*
* PyCore.cpp
* This file contains the definitions for the functions that bind the C++ classes to Python.
* The functions are called from PyLink.cpp.
* e.x. bindItem(m) is called from PyLink.cpp to bind the Item class to Python.
*
*
*/
void bindItem(py::module_& m)
{
    py::class_<Item, std::shared_ptr<Item>>(m, "Item")
        .def(py::init<const Result<AEGP_ItemH>&>())
        .def_property("name",
            &Item::getName,
            &Item::setName);

}

void bindCompItem(py::module_& m)
{
    py::class_<CompItem, Item, std::shared_ptr<CompItem>>(m, "CompItem")
        .def(py::init<Result<AEGP_ItemH>>());
}

void bindFootageItem(py::module_& m)
{
    py::class_<FootageItem, Item, std::shared_ptr<FootageItem>>(m, "FootageItem")
        .def(py::init<Result<AEGP_ItemH>>());
}

void bindFolderItem(py::module_& m)
{
    py::class_<FolderItem, Item, std::shared_ptr<FolderItem>>(m, "FolderItem")
        .def(py::init<Result<AEGP_ItemH>>());

}

void bindProject(py::module_& m)
{
    py::class_<Project, std::shared_ptr<Project>>(m, "Project")
        .def(py::init<>())
        .def_property_readonly("activeItem", &Project::ActiveItem, py::return_value_policy::reference)
        .def_readwrite("name", &Project::name)
        .def_readwrite("path", &Project::path);
}

void bindApp(py::module_& m)
{
    py::class_<App, std::shared_ptr<App>>(m, "App")
        .def(py::init<>())
        .def_readwrite("project", &App::project)
        .def("beginUndoGroup", &App::beginUndoGroup, py::arg("undo_name") = "Default Undo Group Name")
        .def("endUndoGroup", &App::endUndoGroup)
        .def("reportInfo", &App::reportInfo, py::arg("info") = "Hello World");

    // Create an instance of App and set it as an attribute of the module
    auto appInstance = std::make_shared<App>();
    m.attr("app") = appInstance;
}
