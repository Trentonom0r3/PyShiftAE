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

        // PROPERTIES
        .def_property("name", &Layer::GetLayerName, &Layer::SetLayerName)
        .def_property("quality", &Layer::getQuality, &Layer::setQuality)
        .def_property("startTime", &Layer::getOffset, &Layer::setOffset) //CHANGE IN DOCS
        .def_property("index", &Layer::index, &Layer::changeIndex)

        //ADD TO DOCS
        .def_property("video_active", [](Layer& self) {return self.getFlag(LayerFlag::VIDEO_ACTIVE); },
            			[](Layer& self, bool value) {self.setFlag(LayerFlag::VIDEO_ACTIVE, value); })
        .def_property("audio_active", [](Layer& self) {return self.getFlag(LayerFlag::AUDIO_ACTIVE); },
            						[](Layer& self, bool value) {self.setFlag(LayerFlag::AUDIO_ACTIVE, value); })
        .def_property("effects_active", [](Layer& self) {return self.getFlag(LayerFlag::EFFECTS_ACTIVE); },
            									[](Layer& self, bool value) {self.setFlag(LayerFlag::EFFECTS_ACTIVE, value); })
        .def_property("motion_blur", [](Layer& self) {return self.getFlag(LayerFlag::MOTION_BLUR); },
            [](Layer& self, bool value) {self.setFlag(LayerFlag::MOTION_BLUR, value); })
        .def_property("frame_blending", [](Layer& self) {return self.getFlag(LayerFlag::FRAME_BLENDING); },
            			[](Layer& self, bool value) {self.setFlag(LayerFlag::FRAME_BLENDING, value); })
        .def_property("locked", [](Layer& self) {return self.getFlag(LayerFlag::LOCKED); },
            								[](Layer& self, bool value) {self.setFlag(LayerFlag::LOCKED, value); })
        .def_property("shy", [](Layer& self) {return self.getFlag(LayerFlag::SHY); },
            [](Layer& self, bool value) {self.setFlag(LayerFlag::SHY, value); })
        .def_property("collapse", [](Layer& self) {return self.getFlag(LayerFlag::COLLAPSE); },
            										[](Layer& self, bool value) {self.setFlag(LayerFlag::COLLAPSE, value); })
        .def_property("auto_orient_rotation", [](Layer& self) {return self.getFlag(LayerFlag::AUTO_ORIENT_ROTATION); },
            															[](Layer& self, bool value) {self.setFlag(LayerFlag::AUTO_ORIENT_ROTATION, value); })
        .def_property("adjustment_layer", [](Layer& self) {return self.getFlag(LayerFlag::ADJUSTMENT_LAYER); },
            			[](Layer& self, bool value) {self.setFlag(LayerFlag::ADJUSTMENT_LAYER, value); })
        .def_property("time_remapping", [](Layer& self) {return self.getFlag(LayerFlag::TIME_REMAPPING); },
            												[](Layer& self, bool value) {self.setFlag(LayerFlag::TIME_REMAPPING, value); })
        .def_property("layer_is_3d", [](Layer& self) {return self.getFlag(LayerFlag::LAYER_IS_3D); },
            [](Layer& self, bool value) {self.setFlag(LayerFlag::LAYER_IS_3D, value); })
        .def_property("look_at_camera", [](Layer& self) {return self.getFlag(LayerFlag::LOOK_AT_CAMERA); },
            															[](Layer& self, bool value) {self.setFlag(LayerFlag::LOOK_AT_CAMERA, value); })
        .def_property("look_at_poi", [](Layer& self) {return self.getFlag(LayerFlag::LOOK_AT_POI); },
            			[](Layer& self, bool value) {self.setFlag(LayerFlag::LOOK_AT_POI, value); })
        .def_property("solo", [](Layer& self) {return self.getFlag(LayerFlag::SOLO); },
            											[](Layer& self, bool value) {self.setFlag(LayerFlag::SOLO, value); })
        .def_property("markers_locked", [](Layer& self) {return self.getFlag(LayerFlag::MARKERS_LOCKED); },
            																[](Layer& self, bool value) {self.setFlag(LayerFlag::MARKERS_LOCKED, value); })
        .def_property("null_layer", [](Layer& self) {return self.getFlag(LayerFlag::NULL_LAYER); },
            				[](Layer& self, bool value) {self.setFlag(LayerFlag::NULL_LAYER, value); })
        .def_property("hide_locked_masks", [](Layer& self) {return self.getFlag(LayerFlag::HIDE_LOCKED_MASKS); },
            																			[](Layer& self, bool value) {self.setFlag(LayerFlag::HIDE_LOCKED_MASKS, value); })
        .def_property("guide_layer", [](Layer& self) {return self.getFlag(LayerFlag::GUIDE_LAYER); },
            				[](Layer& self, bool value) {self.setFlag(LayerFlag::GUIDE_LAYER, value); })
        .def_property("advanced_frame_blending", [](Layer& self) {return self.getFlag(LayerFlag::ADVANCED_FRAME_BLENDING); },
            																							[](Layer& self, bool value) {self.setFlag(LayerFlag::ADVANCED_FRAME_BLENDING, value); })
        .def_property("sublayers_render_separately", [](Layer& self) {return self.getFlag(LayerFlag::SUBLAYERS_RENDER_SEPARATELY); },
            																												[](Layer& self, bool value) {self.setFlag(LayerFlag::SUBLAYERS_RENDER_SEPARATELY, value); })
        .def_property("environment_layer", [](Layer& self) {return self.getFlag(LayerFlag::ENVIRONMENT_LAYER); },
            				[](Layer& self, bool value) {self.setFlag(LayerFlag::ENVIRONMENT_LAYER, value); })
        
        //ADD TO DOCS

        .def_property_readonly("sourceName", &Layer::GetSourceName)
        .def_property_readonly("time", &Layer::layerTime) 
        .def_property_readonly("compTime", &Layer::layerCompTime) 
        .def_property_readonly("inPoint", &Layer::inPoint) 
        .def_property_readonly("compInPoint", &Layer::compInPoint) 
        .def_property_readonly("duration", &Layer::duration)
        .def_property_readonly("compDuration", &Layer::compDuration) 


        // METHODS
        .def("delete", &Layer::deleteLayer) 
        .def("duplicate", &Layer::duplicate);

}

void bindItem(py::module_& m)
{
    py::class_<Item, std::shared_ptr<Item>>(m, "Item")
        .def(py::init<const Result<AEGP_ItemH>&>())
        .def_property_readonly("width", &Item::getWidth) 
        .def_property_readonly("height", &Item::getHeight)
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
        .def_property("width", &Item::getWidth, &CompItem::setWidth)
        .def_property("height", &Item::getHeight, &CompItem::setHeight)
        .def_property("duration", &CompItem::getDuration, &CompItem::setDuration)
        .def("addSolid", &CompItem::newSolid, py::arg("name") = "New Solid", py::arg("width") = 0,
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
        .def("saveAs", &Project::saveAs, py::arg("path")) 
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

void bindLayerEnum(py::module_& m)
{
    py::enum_<LayerFlag>(m, "LayerFlag")
		.value("VIDEO_ACTIVE", LayerFlag::VIDEO_ACTIVE)
		.value("AUDIO_ACTIVE", LayerFlag::AUDIO_ACTIVE)
		.value("EFFECTS_ACTIVE", LayerFlag::EFFECTS_ACTIVE)
		.value("MOTION_BLUR", LayerFlag::MOTION_BLUR)
		.value("FRAME_BLENDING", LayerFlag::FRAME_BLENDING)
		.value("LOCKED", LayerFlag::LOCKED)
		.value("SHY", LayerFlag::SHY)
		.value("COLLAPSE", LayerFlag::COLLAPSE)
		.value("AUTO_ORIENT_ROTATION", LayerFlag::AUTO_ORIENT_ROTATION)
		.value("ADJUSTMENT_LAYER", LayerFlag::ADJUSTMENT_LAYER)
		.value("TIME_REMAPPING", LayerFlag::TIME_REMAPPING)
		.value("LAYER_IS_3D", LayerFlag::LAYER_IS_3D)
		.value("LOOK_AT_CAMERA", LayerFlag::LOOK_AT_CAMERA)
		.value("LOOK_AT_POI", LayerFlag::LOOK_AT_POI)
		.value("SOLO", LayerFlag::SOLO)
		.value("MARKERS_LOCKED", LayerFlag::MARKERS_LOCKED)
		.value("NULL_LAYER", LayerFlag::NULL_LAYER)
		.value("HIDE_LOCKED_MASKS", LayerFlag::HIDE_LOCKED_MASKS)
		.value("GUIDE_LAYER", LayerFlag::GUIDE_LAYER)
		.value("ADVANCED_FRAME_BLENDING", LayerFlag::ADVANCED_FRAME_BLENDING)
		.value("SUBLAYERS_RENDER_SEPARATELY", LayerFlag::SUBLAYERS_RENDER_SEPARATELY)
		.value("ENVIRONMENT_LAYER", LayerFlag::ENVIRONMENT_LAYER)
		.export_values();

}
