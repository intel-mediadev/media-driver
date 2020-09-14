/*
* Copyright (c) 2014-2020, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file     mhw_render_g10_X.h
//! \brief    Defines functions for constructing  render engine commands on Gen10-based platforms
//!

#ifndef __MHW_RENDER_G10_X_H__
#define __MHW_RENDER_G10_X_H__

#include "mhw_render_generic.h"
#include "mhw_render_hwcmd_g10_X.h"
#include "mhw_state_heap_g10.h"

class MhwRenderInterfaceG10 : public MhwRenderInterfaceGeneric<mhw_render_g10_X>
{
public:
    MhwRenderInterfaceG10(
        MhwMiInterface          *miInterface,
        PMOS_INTERFACE          osInterface,
        MEDIA_SYSTEM_INFO       *gtSystemInfo,
        uint8_t                 newStateHeapManagerRequested) :
        MhwRenderInterfaceGeneric(miInterface, osInterface, gtSystemInfo, newStateHeapManagerRequested)
    {
        MHW_FUNCTION_ENTER;

        // L3 Cache size per bank = 256 KB.
        // {SLM,    URB,     DC,      RO(I/S, C, T),   L3 Client Pool}
        // {  0,     64,      0,                  0,         192     }
        m_l3CacheCntlRegisterValueDefault = 0xC0000040;

        InitMmioRegisters();
    }

    virtual ~MhwRenderInterfaceG10() { MHW_FUNCTION_ENTER; }

    MOS_STATUS AddMediaVfeCmd(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        PMHW_VFE_PARAMS                 params);

    MOS_STATUS AddPipelineSelectCmd(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        bool                            gpGpuPipe);

    MOS_STATUS AddMediaObject(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        PMHW_BATCH_BUFFER               batchBuffer,
        PMHW_MEDIA_OBJECT_PARAMS        params);

    MOS_STATUS AddMediaObjectWalkerCmd(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        PMHW_WALKER_PARAMS              params);

    MOS_STATUS AddPaletteLoadCmd(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        PMHW_PALETTE_PARAMS             params);

    MOS_STATUS AddGpgpuCsrBaseAddrCmd(
        PMOS_COMMAND_BUFFER             cmdBuffer,
        PMOS_RESOURCE                   csrResource);

    MOS_STATUS EnableL3Caching(
        PMHW_RENDER_ENGINE_L3_CACHE_SETTINGS    cacheSettings);

    MOS_STATUS SetL3Cache(
        PMOS_COMMAND_BUFFER             cmdBuffer );

    MHW_RENDER_ENGINE_L3_CACHE_CONFIG* GetL3CacheConfig() { return &m_l3CacheConfig; }

    virtual PMHW_MI_MMIOREGISTERS GetMmioRegisters()
    {
        return &m_mmioRegisters;
    }

    //!
    //! \brief    Get AVS sampler state Inc unit
    //! \details  Get AVS sampler state Inc unit
    //! \return   [out] uint32_t
    //!           AVS sampler unit.
    uint32_t GetSamplerStateAVSIncUnit() { return MHW_SAMPLER_STATE_AVS_INC_G10; }

    //!
    //! \brief    Get Conv sampler state Inc unit
    //! \details  Get Conv sampler state Inc unit
    //! \return   [out] uint32_t
    //!           Conv sampler unit.
    uint32_t GetSamplerStateConvIncUnit() { return MHW_SAMPLER_STATE_CONV_INC_G10; }

    //!
    //! \brief    Get the sampler height and width align unit
    //! \details  NV12 format needs the width and height to be a multiple of some unit
    //! \param    [in] bool
    //!           true if AVS sampler, false otherwise
    //! \param    [in, out] uint32_t
    //!           weight align unit
    //! \param    [in, out] uint32_t
    //!           height align unit
    virtual void GetSamplerResolutionAlignUnit(bool isAVSSampler, uint32_t &widthAlignUnit, uint32_t &heightAlignUnit)
    {
        // enable 2 plane NV12 when width is not multiple of 2 or height is
        // not multiple of 4. For AVS sampler, no limitation for 4 alignment.
        widthAlignUnit  = isAVSSampler ? MHW_AVS_SAMPLER_WIDTH_ALIGN_UNIT : MHW_SAMPLER_WIDTH_ALIGN_UNIT_G10;
        heightAlignUnit = isAVSSampler ? MHW_AVS_SAMPLER_HEIGHT_ALIGN_UNIT : MHW_SAMPLER_HEIGHT_ALIGN_UNIT_G10;
    }

private:
    //! \brief Mmio registers address
    MHW_MI_MMIOREGISTERS    m_mmioRegisters = {};
    void InitMmioRegisters();
};

#endif
