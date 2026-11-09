#pragma once

#if (defined(NUMATHAP_REAL_TYPE_DOUBLE) + \
     defined(NUMATHAP_REAL_TYPE_LONG_DOUBLE) + \
     defined(NUMATHAP_REAL_TYPE_MULTIPRECISION)) != 1

#error "Exactly one NUMATHAP_REAL_TYPE must be defined."

#endif

#if defined(NUMATHAP_REAL_TYPE_DOUBLE)

namespace numathap::numeric::detail {

using Real2Representation = double;

}

#elif defined(NUMATHAP_REAL_TYPE_LONG_DOUBLE)

namespace numathap::numeric::detail {

using Real2Representation = long double;

}

#elif defined(NUMATHAP_REAL_TYPE_MULTIPRECISION)

// Boost Multiprecision support will be implemented later.

#error "Multiprecision representation is not implemented yet."

#endif