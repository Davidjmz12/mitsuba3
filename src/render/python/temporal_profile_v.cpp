#include <mitsuba/render/temporal_profile.h>
#include <mitsuba/render/shape.h>
#include <mitsuba/core/properties.h>
#include <mitsuba/python/python.h>
#include <nanobind/trampoline.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <drjit/python.h>


MI_VARIANT class PyTemporalProfile : public TemporalProfile<Float, Spectrum> {
public:
    MI_IMPORT_TYPES(TemporalProfile)
    NB_TRAMPOLINE(TemporalProfile, 2);

    PyTemporalProfile(const Properties &props) : TemporalProfile(props) { }

    Float sample_delay(const SurfaceInteraction3f &si,
                        const Point2f &sample1,
                        Mask active) const override {
        NB_OVERRIDE_PURE(sample_delay, si, sample1, active);
    }

    Float eval_delay(const SurfaceInteraction3f &si,
                      const Float delay,
                      Mask active) const override {
        NB_OVERRIDE_PURE(eval_delay, si, delay, active);
    }
    std::string to_string() const override {
        NB_OVERRIDE_PURE(to_string);
    }

};

template <typename Ptr, typename Cls> void bind_temporal_delay_generic(Cls &cls) {
    MI_PY_IMPORT_TYPES()

    cls.def("sample_delay",
            [](Ptr temporal_profile, const SurfaceInteraction3f &si,
               const Point2f &sample1, Mask active) {
                return temporal_profile->sample_delay(si, sample1, active);
            }, "si"_a, "sample1"_a, "active"_a = true)
        .def("eval_delay",
            [](Ptr temporal_profile, const SurfaceInteraction3f &si,
               const Float delay, Mask active) {
                return temporal_profile->eval_delay(si, delay, active);
            }, "si"_a, "delay"_a, "active"_a = true);
}

MI_PY_EXPORT(TemporalProfile){
    MI_PY_IMPORT_TYPES(TemporalProfile, TemporalProfilePtr)
    using PyTemporalProfile = PyTemporalProfile<Float, Spectrum>;
    using Properties = PropertiesV<Float>;

    auto temporal_profile = MI_PY_TRAMPOLINE_CLASS(PyTemporalProfile, TemporalProfile, Object)
        .def(nb::init<const Properties&>(), "props"_a)
        .def("__repr__", &TemporalProfile::to_string, D(TemporalProfile, to_string));
    
    bind_temporal_delay_generic<TemporalProfile *>(temporal_profile);

    if constexpr (dr::is_array_v<TemporalProfilePtr>) {
        dr::ArrayBinding b;
        auto temporal_profile_ptr = dr::bind_array_t<TemporalProfilePtr>(b, m, "TemporalProfilePtr");
        bind_temporal_delay_generic<TemporalProfilePtr>(temporal_profile_ptr);
    }

    MI_PY_REGISTER_OBJECT("register_temporal_profile", TemporalProfile);
}