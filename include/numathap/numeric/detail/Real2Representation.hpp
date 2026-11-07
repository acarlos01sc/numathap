#pragma once

#if defined(NUMATHAP_REAL_TYPE_DOUBLE)

namespace numathap::numeric::detail {

using Real2Representation = double;

}  // namespace numathap::numeric::detail

#else

#error "Unsupported numeric representation"

#endif