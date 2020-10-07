#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "nvds_analytics_meta.h"

namespace py = pybind11;
PYBIND11_MODULE(pyds_analytics_meta, m) {
	py::class_<NvDsAnalyticsObjInfo>(m,"NvDsAnalyticsObjInfo",py::module_local())
            .def(py::init<>())
            .def_readwrite("roiStatus",&NvDsAnalyticsObjInfo::roiStatus)
            .def_readwrite("ocStatus",&NvDsAnalyticsObjInfo::ocStatus)
            .def_readwrite("lcStatus",&NvDsAnalyticsObjInfo::lcStatus)
            .def_readwrite("dirStatus",&NvDsAnalyticsObjInfo::dirStatus)
            .def_readwrite("unique_id",&NvDsAnalyticsObjInfo::unique_id)
	    .def_static("cast",[](void *data){
	    	return (NvDsAnalyticsObjInfo*)data;
            },py::return_value_policy::reference);
    py::class_<NvDsAnalyticsFrameMeta>(m,"NvDsAnalyticsFrameMeta",py::module_local())
            .def(py::init<>())
            .def_readwrite("ocStatus",&NvDsAnalyticsFrameMeta::ocStatus)
            .def_readwrite("objInROIcnt",&NvDsAnalyticsFrameMeta::objInROIcnt)
            .def_readwrite("objLCCurrCnt",&NvDsAnalyticsFrameMeta::objLCCurrCnt)
            .def_readwrite("objLCCumCnt",&NvDsAnalyticsFrameMeta::objLCCumCnt)
            .def_readwrite("unique_id",&NvDsAnalyticsFrameMeta::unique_id)
            .def_readwrite("objCnt",&NvDsAnalyticsFrameMeta::objCnt)
            .def_static("cast",[](void *data){
	    	return (NvDsAnalyticsFrameMeta *)data;
             },py::return_value_policy::reference);
}
