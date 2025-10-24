/**
 * @brief Implementation for the shader class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>
#include <fstream>

void vka::Shader::destroy_handles() const noexcept
{
    if (this->m_module != VK_NULL_HANDLE)
        vkDestroyShaderModule(this->m_device, this->m_module, nullptr);
}

void vka::Shader::internal_create(const std::string& path)
{
    // read a shader file
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) [[unlikely]] // it is expected that the file exists
        detail::error::throw_runtime_error(FILE_OPEN_FAILED);

    // get the size of the file and set the cursor to the beginning of the file
    const size_t file_size = file.tellg();
    file.seekg(0, std::ifstream::beg);

    // allocate a buffer and read the file
    // use unique ptr for memory guard
    const std::unique_ptr<char> code(new char[file_size]);
    file.read(code.get(), (std::streamsize)file_size);
    file.close();

    const VkShaderModuleCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = file_size,
        .pCode = reinterpret_cast<uint32_t*>(code.get()) // shader code size is always a multiple of 4, it's ok to cast from char* to uint32_t*
    };
    check_result(vkCreateShaderModule(this->m_device, &ci, nullptr, &this->m_module), SHADER_CREATE_FAILED);
}
