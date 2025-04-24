#include <cstring>

#include <mitsuba/render/bsdf.h>
#include <mitsuba/render/temporal_profile.h>
#include <mitsuba/render/texture.h>
#include <mitsuba/core/properties.h>

NAMESPACE_BEGIN(mitsuba)

MI_VARIANT TemporalProfile<Float, Spectrum>::TemporalProfile(const Properties &/*props*/){
    MI_REGISTRY_PUT("TemporalProfile", this);
}

MI_VARIANT TemporalProfile<Float, Spectrum>::~TemporalProfile() {
    if constexpr (dr::is_jit_v<Float>)
        jit_registry_remove(this);
}

MI_IMPLEMENT_CLASS_VARIANT(TemporalProfile, Object, "TemporalProfile")
MI_INSTANTIATE_CLASS(TemporalProfile)
NAMESPACE_END(mitsuba)
