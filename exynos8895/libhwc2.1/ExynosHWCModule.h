/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_EXYNOS_HWC_MODULE_H_
#define ANDROID_EXYNOS_HWC_MODULE_H_

#include "ExynosHWC.h"
#include "DeconHeader.h"
#include "ExynosHWCHelper.h"

#define HWC_VERSION     HWC_DEVICE_API_VERSION_1_5

#define VSYNC_DEV_PREFIX "/sys/devices/platform/"
#define PSR_DEV_NAME  "12860000.decon_f/psr_info"
#define DP_LINK_NAME	"11090000.displayport"
#define DP_UEVENT_NAME	"change@/devices/platform/%s/extcon/extcon0"
#define DP_CABLE_STATE_NAME "/sys/devices/platform/%s/extcon/extcon0/cable.%d/state"
#define BRIGHTNESS_NODE_BASE "/sys/class/backlight/panel/brightness"
#define MAX_BRIGHTNESS_NODE_BASE "/sys/class/backlight/panel/max_brightness"

#define IDMA(x) static_cast<decon_idma_type>(x)

#define DEDICATED_CHANNEL_TYPE MPP_DPP_G
#define DEDICATED_CHANNEL_INDEX 0
#define USE_DEDICATED_TOP_WINDOW

enum {
    HWC_DISPLAY_NONE_BIT = 0
};

/*
 * pre_assign_info: all display_descriptors that want to reserve
 */
struct exynos_mpp_t {
    int physicalType;
    int logicalType;
    char name[16];
    uint32_t physical_index;
    uint32_t logical_index;
    uint32_t pre_assign_info;
};

const dpp_channel_map_t IDMA_CHANNEL_MAP[] = {
    {MPP_DPP_G,      0, IDMA_G0,    IDMA(0)},
    {MPP_DPP_G,      1, IDMA_G1,    IDMA(1)},
    {MPP_DPP_VG,     0, IDMA_VG0,   IDMA(2)},
    {MPP_DPP_VG,     1, IDMA_VG1,   IDMA(3)},
    {MPP_DPP_VGF,    0, IDMA_VGF0,  IDMA(4)},
    {MPP_DPP_VGF,    1, IDMA_VGF1,  IDMA(5)},
    {MPP_P_TYPE_MAX, 0, ODMA_WB,    IDMA(6)}, // not idma but..
    {static_cast<mpp_phycal_type_t>(MAX_DECON_DMA_TYPE), 0, MAX_DECON_DMA_TYPE, IDMA(7)}
};

#define MAX_NAME_SIZE   32
struct exynos_display_t {
    uint32_t type;
    uint32_t index;
    char display_name[MAX_NAME_SIZE];
    char decon_node_name[MAX_NAME_SIZE];
    char vsync_node_name[MAX_NAME_SIZE];
};

#define PRIMARY_MAIN_EXTERNAL_WINCNT   2
#define EXTERNAL_MAIN_EXTERNAL_WINCNT  4

#define DISPLAY_MODE_MASK_LEN    8
#define DISPLAY_MODE_MASK_BIT    0xff

enum {
    DISPLAY_MODE_PRIMARY_MAIN = 0,  /* This is default mode */
    DISPLAY_MODE_EXTERNAL_MAIN,
    DISPLAY_MODE_NUM
};

/*
 * This is base window index of primary display for each display mode.
 * External display base window is always 0
 */
const uint32_t EXTERNAL_WINDOW_COUNT[] = {PRIMARY_MAIN_EXTERNAL_WINCNT, EXTERNAL_MAIN_EXTERNAL_WINCNT};

#define EXTERNAL_MAIN_DISPLAY_START_BIT (DISPLAY_MODE_MASK_LEN * DISPLAY_MODE_EXTERNAL_MAIN)
enum {
    EXTERNAL_MAIN_DISPLAY_PRIMARY_BIT = 1 << (EXTERNAL_MAIN_DISPLAY_START_BIT + HWC_DISPLAY_PRIMARY),
    EXTERNAL_MAIN_DISPLAY_EXTERNAL_BIT = 1 << (EXTERNAL_MAIN_DISPLAY_START_BIT + HWC_DISPLAY_EXTERNAL),
    EXTERNAL_MAIN_DISPLAY_VIRTUAL_BIT = 1 << (EXTERNAL_MAIN_DISPLAY_START_BIT + HWC_DISPLAY_VIRTUAL),
};

const exynos_mpp_t AVAILABLE_OTF_MPP_UNITS[] = {
    {MPP_DPP_G, MPP_LOGICAL_DPP_G, "DPP_G0", 0, 0, HWC_DISPLAY_PRIMARY_BIT},
    {MPP_DPP_G, MPP_LOGICAL_DPP_G, "DPP_G1", 1, 0, HWC_DISPLAY_EXTERNAL_BIT},
    {MPP_DPP_VG, MPP_LOGICAL_DPP_VG, "DPP_VG0", 0, 0, HWC_DISPLAY_PRIMARY_BIT},
    {MPP_DPP_VG, MPP_LOGICAL_DPP_VG, "DPP_VG1", 1, 0, HWC_DISPLAY_EXTERNAL_BIT},
    {MPP_DPP_VGF, MPP_LOGICAL_DPP_VGF, "DPP_VGF0", 0, 0, HWC_DISPLAY_PRIMARY_BIT},
    {MPP_DPP_VGF, MPP_LOGICAL_DPP_VGF, "DPP_VGF1", 1, 0, HWC_DISPLAY_EXTERNAL_BIT}
};

const exynos_mpp_t AVAILABLE_M2M_MPP_UNITS[] = {
    {MPP_MSC, MPP_LOGICAL_MSC, "MSC0_PRI", 0, 0, HWC_DISPLAY_PRIMARY_BIT},
    {MPP_MSC, MPP_LOGICAL_MSC_YUV, "MSC0_EXT", 0, 1, HWC_DISPLAY_EXTERNAL_BIT},
    {MPP_G2D, MPP_LOGICAL_G2D_YUV, "G2D0-YUV_EXT", 0, 0, HWC_DISPLAY_EXTERNAL_BIT},
    {MPP_G2D, MPP_LOGICAL_G2D_RGB, "G2D0-RGB_EXT", 0, 1, HWC_DISPLAY_EXTERNAL_BIT}
};

const exynos_display_t AVAILABLE_DISPLAY_UNITS[] = {
    {HWC_DISPLAY_PRIMARY, 0, "PrimaryDisplay", "/dev/graphics/fb0", "12860000.decon_f/vsync"},
    {HWC_DISPLAY_EXTERNAL, 0, "ExternalDisplay", "/dev/graphics/fb1", "12a40000.decon_t/vsync"},
    {HWC_DISPLAY_VIRTUAL, 0, "VirtualDisplay", {}, {}},
};

#define DISPLAY_COUNT sizeof(AVAILABLE_DISPLAY_UNITS)/sizeof(exynos_display_t)

/* It is code to avoid build error. It is not actually used in this project. */
#define MAX_DPPCNT_PER_SET 2

struct resource_set_t {
    uint32_t windows[MAX_DPPCNT_PER_SET];
    char dpps[MAX_DPPCNT_PER_SET][16];
    bool attr_support[MAX_ATTRIBUTE_NUM];
};

const uint32_t ATTRIBUTE_PRIORITY_LIST[] = {
};

const resource_set_t REFERENCE_RESOURCE_SET_TABLE[] = {
};

#define RESOURCE_SET_COUNT sizeof(REFERENCE_RESOURCE_SET_TABLE)/sizeof(resource_set_t)

struct display_resource_info_t {
    uint32_t displayId;
    /* If this variable has a non-zero value, the display uses resource sets by defined value */
    uint32_t fix_setcnt;
    /* Small number have higher priority than bigger one.
     * Range of priority is from 0 to (NUM_OF_DISPLAY_NEEDED_OTFMPP -1) */
    uint32_t dpp_priority;
    uint32_t attr_priority[MAX_ATTRIBUTE_NUM];
};

const display_resource_info_t RESOURCE_INFO_TABLE[] = {
};

#endif
