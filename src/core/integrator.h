
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_INTEGRATOR_H
#define PBRT_CORE_INTEGRATOR_H

// core/integrator.h*
#include "pbrt.h"
#include "primitive.h"
#include "spectrum.h"
#include "light.h"
#include "reflection.h"
#include "sampler.h"
#include "material.h"

namespace pbrt
{

// Integrator Declarations
// Integrator 声明
class Integrator
{
public:
    // Integrator Interface
    // Integrator 接口
    virtual ~Integrator();                       // 虚析构函数
    virtual void Render(const Scene &scene) = 0; // 纯虚函数
};

Spectrum UniformSampleAllLights(const Interaction &it, const Scene &scene,
                                MemoryArena &arena, Sampler &sampler,
                                const std::vector<int> &nLightSamples,
                                bool handleMedia = false);
Spectrum UniformSampleOneLight(const Interaction &it, const Scene &scene,
                               MemoryArena &arena, Sampler &sampler,
                               bool handleMedia = false,
                               const Distribution1D *lightDistrib = nullptr);
Spectrum EstimateDirect(const Interaction &it, const Point2f &uShading,
                        const Light &light, const Point2f &uLight,
                        const Scene &scene, Sampler &sampler,
                        MemoryArena &arena, bool handleMedia = false,
                        bool specular = false);
std::unique_ptr<Distribution1D> ComputeLightPowerDistribution(
    const Scene &scene);

// SamplerIntegrator Declarations
// SamplerIntegrator 声明
class SamplerIntegrator : public Integrator
{
public:
    // SamplerIntegrator Public Methods
    // SamplerIntegrator 公有方法
    SamplerIntegrator(std::shared_ptr<const Camera> camera,
                      std::shared_ptr<Sampler> sampler,
                      const Bounds2i &pixelBounds)
        : camera(camera), sampler(sampler), pixelBounds(pixelBounds) {}
    virtual void Preprocess(const Scene &scene, Sampler &sampler) {} // 预处理，可选择性实现
    void Render(const Scene &scene);
    virtual Spectrum Li(const RayDifferential &ray, const Scene &scene,
                        Sampler &sampler, MemoryArena &arena,
                        int depth = 0) const = 0;
    Spectrum SpecularReflect(const RayDifferential &ray,
                             const SurfaceInteraction &isect,
                             const Scene &scene, Sampler &sampler,
                             MemoryArena &arena, int depth) const;
    Spectrum SpecularTransmit(const RayDifferential &ray,
                              const SurfaceInteraction &isect,
                              const Scene &scene, Sampler &sampler,
                              MemoryArena &arena, int depth) const;

protected:
    // SamplerIntegrator Protected Data
    // SamplerIntegrator 受保护数据
    std::shared_ptr<const Camera> camera; // 相机

private:
    // SamplerIntegrator Private Data
    // SamplerIntegrator 私有数据
    std::shared_ptr<Sampler> sampler; // 采样器
    const Bounds2i pixelBounds;       // 包围盒，平面，int型
};

} // namespace pbrt

#endif // PBRT_CORE_INTEGRATOR_H
