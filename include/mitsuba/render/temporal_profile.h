#pragma once

#include <mitsuba/core/profiler.h>
#include <mitsuba/render/interaction.h>
#include <drjit/call.h>
#include <mitsuba/render/bsdf.h>

NAMESPACE_BEGIN(mitsuba)


/**
 * \brief Temporal profile interface
 *
 * This class provides an abstract interface to all %TemporalProfile plugins in Mitsuba.
 * It exposes functions for evaluating and sampling the model.
 *
 * A temporal profile describes how light is scattered in time when it interacts.
 * It needs to be sampled and evaluated at each surface interaction in the
 * path of a light transport simulation.
 */
template <typename Float, typename Spectrum>
class MI_EXPORT_LIB TemporalProfile : public Object {
public:
    MI_IMPORT_TYPES(Texture)
    
    /// Destructor
    ~TemporalProfile();

    /**
     * \brief Sample the temporal profile to obtain a time delay.
     * 
     * The function returns a positive time delay (in optical path length units)
     * sampled from the temporal profile at the given surface interaction. 
     * It must be a positive value.
     * 
     * \param si
     *     A surface interaction data structure describing the underlying
     *    surface position.
     * 
     * \param sample1
     *      A uniformly distributed sample on \f$[0,1]\f$. It is used
     *      to sample the temporal profile.
     * 
     * \return A positive time delay (in optical path length units).
     */
    virtual Float sample_delay(const SurfaceInteraction3f &si,
                            const Point2f &sample1,
                            Mask active) const = 0;
    
    /**
     * \brief Evaluate the probability density function (pdf) of sampling a given time delay.
     * 
     * The function returns the value of the probability density function (pdf)
     * at the specified time delay.
     * 
     * \param si
     *    A surface interaction data structure describing the underlying
     *    surface position.
     * 
     * \param delay
     *   The time delay (in optical path length units) to evaluate the pdf.
     *   It must be a positive value.
     * 
     * 
     */
    virtual Float eval_delay(const SurfaceInteraction3f &si,
                            const Float delay,
                            Mask active) const = 0;
    
    /// Return a human-readable string representation of the object's contents.
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
