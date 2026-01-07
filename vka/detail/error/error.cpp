/**
 * @brief Implements throw functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

[[noreturn]]
void vka::detail::error::throw_invalid_argument(const char* msg)
{
    throw std::invalid_argument(msg);
}

[[noreturn]]
void vka::detail::error::throw_runtime_error(const char* msg)
{
    throw std::runtime_error(msg);
}

[[noreturn]]
void vka::detail::error::throw_out_of_range(const char* msg)
{
    throw std::out_of_range(msg);
}

[[noreturn]]
void vka::detail::error::throw_bad_alloc()
{
    throw std::bad_alloc();
}
