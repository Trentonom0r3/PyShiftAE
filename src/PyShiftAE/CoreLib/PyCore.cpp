#include "PyCore.h"

/*
* PyCore.cpp
* This file contains the definitions for the functions that bind the C++ classes to Python.
* The functions are called from PyLink.cpp.
* e.x. bindItem(m) is called from PyLink.cpp to bind the Item class to Python.
*
*
*/

void bindLayer(py::module_& m)
{
	py::class_<Layer, std::shared_ptr<Layer>>(m, "Layer")
		.def(py::init<const Result<AEGP_LayerH>&>())
		.def_property("name",
            			&Layer::GetLayerName,
            			&Layer::SetLayerName)
		.def_property_readonly("sourceName", &Layer::GetSourceName)
        .def_property_readonly("time", &Layer::layerTime) //ADD TO DOCS
        .def_property_readonly("compTime", &Layer::layerCompTime) //ADD TO DOCS
        .def_property_readonly("inPoint", &Layer::inPoint) //ADD TO DOCS
        .def_property_readonly("compInPoint", &Layer::compInPoint) //ADD TO DOCS
        .def_property_readonly("duration", &Layer::duration) //ADD TO DOCS 
        .def_property_readonly("compDuration", &Layer::compDuration) //ADD TO DOCS
        .def_property("quality", &Layer::getQuality, &Layer::setQuality) //ADD TO DOCS
        .def_property("offset", &Layer::getOffset, &Layer::setOffset) //ADD TO DOCS
        .def("delete", &Layer::deleteLayer) //ADD TO DOCS
        .def("duplicate", &Layer::duplicate) //ADD TO DOCS
        .def_property("index",
            			&Layer::index,
            			&Layer::changeIndex);

}

void bindItem(py::module_& m)
{
    py::class_<Item, std::shared_ptr<Item>>(m, "Item")
        .def(py::init<const Result<AEGP_ItemH>&>())
        .def_property_readonly("width", &Item::getWidth) //ADD TO DOCS
        .def_property_readonly("height", &Item::getHeight)//ADD TO DOCS
        .def_property("name",
            &Item::getName,
            &Item::setName);
}

void bindCompItem(py::module_& m)
{
    py::class_<CompItem, Item, std::shared_ptr<CompItem>>(m, "CompItem")
        .def(py::init<Result<AEGP_ItemH>>())
        .def_property_readonly("layer", &CompItem::getLayers, py::return_value_policy::reference)
        .def_property_readonly("layers", &CompItem::getLayers, py::return_value_policy::reference)
        .def_property_readonly("numLayers", &CompItem::NumLayers)
        .def_property("width", &Item::getWidth, &CompItem::setWidth)//ADD TO DOCS
        .def_property("height", &Item::getHeight, &CompItem::setHeight)//ADD TO DOCS
        .def_property("duration", &CompItem::getDuration, &CompItem::setDuration)//ADD TO DOCS
        .def("addSolid", &CompItem::newSolid, py::arg("name") = "New Solid", py::arg("width") = 0,//ADD TO DOCS
            py::arg("height") = 0, py::arg("red") = 0, py::arg("green") = 0, py::arg("blue") = 0, py::arg("alpha") = 0, py::arg("duration") = 0)
        .def_property("frameRate",//ADD TO DOCS
            &CompItem::getFrameRate,
            &CompItem::setFrameRate)
        .def("addLayer", &CompItem::addLayer, py::arg("name") = "New Layer",
            py::arg("path") = NULL, py::arg("index") = -1);

}

void bindFootageItem(py::module_& m)
{
    py::class_<FootageItem, Item, std::shared_ptr<FootageItem>>(m, "FootageItem")
        .def(py::init<Result<AEGP_ItemH>>());
}

void bindFolderItem(py::module_& m)
{
    py::class_<FolderItem, Item, std::shared_ptr<FolderItem>>(m, "FolderItem")
        .def(py::init<Result<AEGP_ItemH>>())
        .def("addFolder", &FolderItem::addFolder, py::arg("name") = "New Folder");

}

void bindProject(py::module_& m)
{
    py::class_<Project, std::shared_ptr<Project>>(m, "Project")
        .def(py::init<>())
        .def_property_readonly("activeItem", &Project::ActiveItem, py::return_value_policy::reference)
        .def_property_readonly("name", &Project::getName)
        .def_property_readonly("path", &Project::getPath)
        .def("saveAs", &Project::saveAs, py::arg("path")) //ADD TO DOCS
        .def("addFolder", &Project::addFolder, py::arg("name") = "New Folder")
        .def("addComp", &Project::addComp, py::arg("name") = "New Comp", py::arg("width") = 1920,
            py::arg("height") = 1080, py::arg("frameRate") = 24.0, py::arg("duration") = 10,
            py::arg("aspectRatio") = 1.0)
        .def("addFootage", &Project::addFootage, py::arg("path") = "C:\\");
}

void bindApp(py::module_& m)
{
    py::class_<App, std::shared_ptr<App>>(m, "App")
        .def(py::init<>())
        .def_readwrite("project", &App::project)
        .def("beginUndoGroup", &App::beginUndoGroup, py::arg("undo_name") = "Default Undo Group Name")
        .def("endUndoGroup", &App::endUndoGroup)
        .def("reportInfo", [](App& self, py::object info) {
        // Convert the Python object to a string
            std::ostringstream oss;
            oss << info;
            self.reportInfo(oss.str());
            }, py::arg("info") = "Hello World");

    // Create an instance of App and set it as an attribute of the module
    auto appInstance = std::make_shared<App>();
    m.attr("app") = appInstance;
}
