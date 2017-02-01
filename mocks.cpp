#include <cxxtest/TestSuite.h>
#include "mocks.h"

namespace Mocks {

void reject(const char *message)
{
  TSM_ASSERT(message, false);
}

} // namespace Mocks
