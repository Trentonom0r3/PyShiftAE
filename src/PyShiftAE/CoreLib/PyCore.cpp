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
		.def_property_readonly("index", &Layer::index);
}

void bindItem(py::module_& m)
{
    py::class_<Item, std::shared_ptr<Item>>(m, "Item")
        .def(py::init<const Result<AEGP_ItemH>&>())
        .def_property("name",
            &Item::getName,
            &Item::setName)
        // Binding for frameAtTime with memory management
        .def("frameAtTime", [](Item& self, float time) -> py::array_t<uint8_t> {
            // Get the ImageData object from your API
            ImageData image_data = self.frameAtTime(time);

            // Get the raw pointer to the vector's data
            uint8_t* raw_data = image_data.data->data();
            int height = image_data.height;
            int width = image_data.width;
            int channels = image_data.channels;

            // Define a std::function with the deleter
            auto deleter_func = [data = image_data.data](void* /* owner */) {
                // The shared_ptr data will be released when the NumPy array is deleted
            };

            // Convert std::function to a function pointer
            auto capsule_deleter = new std::function<void(void*)>(deleter_func);

            // Use a lambda that calls the function pointer as the deleter
            auto capsule_deleter_wrapper = [](void* data) {
                auto func_ptr = reinterpret_cast<std::function<void(void*)>*>(data);
                (*func_ptr)(nullptr);
                delete func_ptr;
            };

            // Create the NumPy array
            return py::array_t<uint8_t>(
                { height, width, channels }, // shape
                { channels * width * sizeof(uint8_t), channels * sizeof(uint8_t), sizeof(uint8_t) }, // strides
                raw_data, // the data pointer
                py::capsule(capsule_deleter, "array_data_capsule", capsule_deleter_wrapper) // the capsule with the deleter
            );
            });
}

void bindCompItem(py::module_& m)
{
    py::class_<CompItem, Item, std::shared_ptr<CompItem>>(m, "CompItem")
        .def(py::init<Result<AEGP_ItemH>>())
        .def_property_readonly("layer", &CompItem::getLayers, py::return_value_policy::reference)
        .def_property_readonly("layers", &CompItem::getLayers, py::return_value_policy::reference)
        .def_property_readonly("numLayers", &CompItem::NumLayers);

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
