# pyds_analytics_meta

Python bindings for NvDsAnalyticsObjInfo and NvDsAnalyticsFrameMeta in DeepStream Python API

This repository was developed followed this original repo [pyds_tracker_meta](https://github.com/mrtj/pyds_tracker_meta) and NVidia developers forum topic [developer_zone](https://forums.developer.nvidia.com/t/deepstream-5-0-python-bindings-for-gst-nvdsanalytics-access-meta-data/147670/11)

[pybind11](https://github.com/pybind/pybind11) wrapper to access Nvidia [DeepStream](https://developer.nvidia.com/deepstream-sdk) NvDsAnalytics\* meta info (`NvDsAnalyticsFrameMeta` `NvDsAnalyticsObjInfo` classes) from Python.

## Introduction
This library provides utility functions to access to the `NvDsAnalyticsFrameMeta` and `NvDsAnalyticsObjInfo` types user metadata of DeepStream. See [DeepStream Python API](https://docs.nvidia.com/metropolis/deepstream/5.0/python-api/NvDsAnalytics/pyanalytics.html) NvDsAnalytics reference for detailed explanation of functions.

## Installation

### Prerequisites

1. Install [pybind11](https://github.com/pybind/pybind11). The recommended way is to [build it from source](https://pybind11.readthedocs.io/en/stable/basics.html?highlight=install#compiling-the-test-cases). Alternatively you might try simply `pip3 install pybind11`.
2. You should have `gstreamer-1.0` and `gstreamer-video-1.0` packages installed in your system. If you are using DeepStream, you most likely installed these packages.
3. You will need also the standard `c++` compiler you usually find in Linux distributions. `c++11` standard is used.

### Compile the source

1. The source should be compiled on your target platform (Jetson or x86).
2. Set your DeepStream version and path in `build.sh`.
3. Launch `build.sh`
4. Install the compiled module with `python setup.py install` (use `sudo` or `python3` if needed).

## Usage

`pyds_analytics_meta` is meant to be used together with the standard [Python bindings for DeepStream](https://github.com/NVIDIA-AI-IOT/deepstream_python_apps). Make sure you have `pyds` available.

Most likely you will use this library from the buffer probe callbacks of a gstreamer plugin pad, when the object tracking results are available. The [deepstream-test2](https://github.com/NVIDIA-AI-IOT/deepstream_python_apps/tree/master/apps/deepstream-test2) python app shows you how to set up such a callback. 

The example snippet provided bellow shows how to cast a user meta to a past frame object batch, and how to access all fields of the metadata. Add the following lines to the `osd_sink_pad_buffer_probe` method found int `deepstream-test2.py`, just after the [`batch_meta` was acquired](https://github.com/NVIDIA-AI-IOT/deepstream_python_apps/blob/2931f6b295b58aed15cb29074d13763c0f8d47be/apps/deepstream-test2/deepstream_test_2.py#L61):

```python
import pyds_analytics_meta

def osd_sink_pad_buffer_probe(pad, info, u_data):
    
    batch_meta = pyds.gst_buffer_get_nvds_batch_meta(hash(gst_buffer))
   
    print("----NvDsAnalytics Frame Meta----") 
    
    l_frame = batch_meta.frame_meta_list 
    # Iterate over list of FrameMeta
    while l_frame is not None:
        try:
            # Casting l_frame.data to ipyds.NvDsFrameMeta
            frame_meta = pyds.NvDsFrameMeta.cast(l_frame.data)
            l_user = frame_meta.frame_user_meta_list
            while l_user is not None:
                try:
                    # Cast to NvDsUserMeta and check it either NvDsAnalyticsFrameMeta or not
                    user_meta = pyds.NvDsUserMeta.cast(l_user.data)
                    if user_meta.base_meta.meta_type != pyds.nvds_get_user_meta_type(
                            "NVIDIA.DSANALYTICSFRAME.USER_META"):
                        continue

                    user_meta_analytics = pyds_analytics_meta.NvDsAnalyticsFrameMeta.cast(user_meta.user_meta_data)
                    print('objCnt:', user_meta_analytics.objCnt)
                    print('objInROIcnt:', user_meta_analytics.objInROIcnt)
                    print('objLCCumCnt:', user_meta_analytics.objLCCumCnt)
                    print('objLCCurrCnt:', user_meta_analytics.objLCCurrCnt)
                    print('ocStatus:', user_meta_analytics.ocStatus)
                    print('unique_id:', user_meta_analytics.unique_id)
                    print(user_meta_analytics.objLCCumCnt)
                except Exception as ex:
                    print('Exception', ex)
                try:
                    l_user = l_user.next
                except StopIteration:
                    break
        except StopIteration:
            break
        # Get next FrameMeta in list 
        try:
            l_frame=l_frame.next
        except StopIteration:
            break

    print("----NvDsAnalytics Object Info----")
    
    l_obj = frame_meta.obj_meta_list
    while l_obj is not None:
        try:
            # Casting l_obj.data to pyds.NvDsObjectMeta
            obj_meta = pyds.NvDsObjectMeta.cast(l_obj.data)
            user_meta_list = obj_meta.obj_user_meta_list
            while user_meta_list is not None:
                try:
                    user_meta = pyds.NvDsUserMeta.cast(user_meta_list.data)
                    user_meta_data = user_meta.user_meta_data
                    if user_meta.base_meta.meta_type != pyds.nvds_get_user_meta_type(
                            "NVIDIA.DSANALYTICSOBJ.USER_META"):
                        continue
                    user_meta_analytics = pyds_analytics_meta.NvDsAnalyticsObjInfo.cast(user_meta.user_meta_data)
                    print('unique_id:', user_meta_analytics.unique_id)
                    print('lcStatus:', user_meta_analytics.lcStatus)
                    print('dirStatus:', user_meta_analytics.dirStatus)
                    print('ocStatus:', user_meta_analytics.ocStatus)
                    print('roiStatus:', user_meta_analytics.roiStatus)
                except StopIteration:
                    break
                try:
                    user_meta_list = user_meta_list.next
                except StopIteration:
                    break
        except StopIteration:
            break
        try:
            l_obj = l_obj.next
        except StopIteration:
            break

```

