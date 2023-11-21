/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <RealEngine/program/Synchronizer.hpp>

namespace md {

/**
 * @brief Number of simulation steps per second
 */
constexpr int k_stepsPerSecond = 100;

/**
 * @brief Fraction of second that passes between each step
 */
constexpr float k_perStep = 1.0f / static_cast<float>(k_stepsPerSecond);

#ifdef _DEBUG
constexpr unsigned int k_frameRateLimit = 300u;
#else
constexpr unsigned int k_frameRateLimit = re::Synchronizer::k_doNotLimitFramesPerSecond;
#endif // _DEBUG

} // namespace md
