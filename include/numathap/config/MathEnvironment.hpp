/**
 * @file MathEnvironment.hpp
 * @brief Defines the MathEnvironment class for managing evaluation settings.
 */
#pragma once

#include <memory>
#include <set>

#include "numathap/config/Capability.hpp"
#include "numathap/config/MathAdapter.hpp"
#include "numathap/config/MathLibrary.hpp"
#include "numathap/config/NumericType.hpp"

namespace numathap::config {

/**
 * @brief Central container for mathematical environment configuration.
 *
 * The MathEnvironment class holds the active configuration, including the
 * selected @ref MathLibrary, the @ref NumericType, and the underlying
 * @ref MathAdapter implementation.
 *
 * It serves as the primary state object used by the evaluation engine to
 * determine which mathematical backends to utilize during expression
 * processing.
 */
class MathEnvironment {
   public:
    /** @brief Constructs a default environment with standard settings. */
    MathEnvironment();

    /** @name Copy/Move Semantics */
    ///@{
    MathEnvironment(const MathEnvironment&);
    MathEnvironment& operator=(const MathEnvironment&);

    MathEnvironment(MathEnvironment&&) noexcept = default;
    MathEnvironment& operator=(MathEnvironment&&) noexcept = default;
    ///@}

    /** @brief Destructor. */
    ~MathEnvironment() = default;

    /**
     * @brief Gets the identifier of the active mathematical library.
     * @return The currently active @ref MathLibrary.
     */
    [[nodiscard]]
    MathLibrary mathLibrary() const noexcept;

    /**
     * @brief Provides access to the active mathematical adapter.
     * @return A reference to the current @ref MathAdapter instance.
     */
    [[nodiscard]]
    const MathAdapter& mathAdapter() const noexcept;

    /**
     * @brief Gets the identifier of the active numeric type.
     * @return The currently active @ref NumericType.
     */
    [[nodiscard]]
    NumericType numericType() const noexcept;

    /**
     * @brief Enables a processing capability.
     *
     * Enabled capabilities are applied by the processing pipeline.
     *
     * @param capability Capability to enable.
     */
    void enableCapability(Capability capability);

    /**
     * @brief Disables a processing capability.
     *
     * @param capability Capability to disable.
     */
    void disableCapability(Capability capability);

    /**
     * @brief Checks whether a capability is enabled.
     *
     * @param capability Capability to query.
     * @return true if the capability is enabled.
     */
    [[nodiscard]]
    bool hasCapability(Capability capability) const noexcept;

   private:
    MathLibrary math_library_{
        MathLibrary::CMath};  ///< Active math library backend.
    NumericType numeric_type_{
        NumericType::Double};  ///< Active numeric representation.
    std::unique_ptr<MathAdapter>
        adapter_;  ///< Unique pointer to the current adapter.
    std::set<Capability> capabilities_;  ///< Enabled processing capabilities.
};

}  // namespace numathap::config