#pragma once

#include <mitsuba/core/profiler.h>
#include <mitsuba/render/interaction.h>
#include <drjit/call.h>
#include <mitsuba/render/bsdf.h>

NAMESPACE_BEGIN(mitsuba)



template <typename Float, typename Spectrum>
class MI_EXPORT_LIB TemporalProfile : public Object {
public:
    MI_IMPORT_TYPES(Texture)
    
    ~TemporalProfile();

    virtual Float sample_delay(const SurfaceInteraction3f &si,
                            const Point2f &sample1,
                            Mask active) const = 0;

    virtual Float eval_delay(const SurfaceInteraction3f &si,
                            const Float delay,
                            Mask active) const = 0;

    std::string to_string() const override = 0;

    MI_DECLARE_CLASS()
protected:

    TemporalProfile(const Properties &props);

};

MI_EXTERN_CLASS(TemporalProfile)

NAMESPACE_END(mitsuba)


MI_CALL_TEMPLATE_BEGIN(TemporalProfile)
    DRJIT_CALL_METHOD(sample_delay)
    DRJIT_CALL_METHOD(eval_delay)
MI_CALL_TEMPLATE_END(BSDF)
