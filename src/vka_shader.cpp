#define _CRT_SECURE_NO_WARNINGS
#include "../include/vulkan_absraction.h"
#include <fstream>
#include <string.h>

vka::Shader::Shader(void) noexcept : Shader(VK_NULL_HANDLE) {}

vka::Shader::Shader(VkDevice device, const std::string& entry_point) noexcept
{
	this->_device = device;
	this->_entry_point = nullptr;
	this->set_entry_point(entry_point);
	this->_stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	this->_module = VK_NULL_HANDLE;
	this->_size = 0;

	this->_loaded = false;
}

vka::Shader::~Shader(void) noexcept
{
	this->clear();
	delete[] this->_entry_point;
}

vka::Shader::Shader(Shader&& shader)
{
	*this = std::move(shader);
}

vka::Shader& vka::Shader::operator= (Shader&& shader)
{
	this->_device = shader._device;
	shader._device = VK_NULL_HANDLE;

	this->_entry_point = shader._entry_point;
	shader._entry_point = nullptr;
	shader.set_entry_point("main");

	this->_stage = shader._stage;
	shader._stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;

	this->_module = shader._module;
	shader._module = VK_NULL_HANDLE;

	this->_size = shader._size;
	shader._size = 0;

	this->_loaded = shader._loaded;
	shader._loaded = false;

	return *this;
}

VkResult vka::Shader::load(const std::string& path, VkShaderStageFlagBits stage)
{
	if (this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of Shader is VK_NULL_HANDLE! Requiered from shader create");

	if (this->_loaded)
		throw std::runtime_error("Shader already loaded!");

	std::fstream shader_file(path, std::ios::in | std::ios::binary | std::ios::ate);	// read file backwards
	if (!shader_file)
		throw std::invalid_argument("Path to shader-file not found! (\"" + path + "\")");

	this->_stage = stage;

	this->_size = shader_file.tellg();		// get cursor position (last position = number of bytes in that file)
	shader_file.seekg(0);					// set cursor at first character (begin of the file)

	char* byte_buff = new char[this->_size];
	shader_file.read(byte_buff, this->_size);
	shader_file.close();

	VkShaderModuleCreateInfo shader_module_create_info = {};
	shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_create_info.pNext = nullptr;
	shader_module_create_info.flags = 0;
	shader_module_create_info.codeSize = this->_size;
	shader_module_create_info.pCode = reinterpret_cast<uint32_t*>(byte_buff);

	VkResult result = vkCreateShaderModule(this->_device, &shader_module_create_info, nullptr, &this->_module);
	if (result == VK_SUCCESS)
		this->_loaded = true;
	return result;
}

void vka::Shader::clear(void)
{
	if (this->_loaded)
	{
		if (this->_device == VK_NULL_HANDLE)
			throw std::invalid_argument("Device of Shader is VK_NULL_HANDLE! Requiered from shader clear.");

		vkDestroyShaderModule(this->_device, this->_module, nullptr);
		this->_module = VK_NULL_HANDLE;
	}

	this->set_entry_point("main");
}

void vka::Shader::set_device(VkDevice device) noexcept
{
	this->_device = device;
}

void vka::Shader::set_entry_point(const std::string& entry_point) noexcept
{
	if (this->_entry_point != nullptr)
		delete[] this->_entry_point;
	this->_entry_point = new char[entry_point.size() + 1];	// +1 for ASCII 0
	strcpy(this->_entry_point, entry_point.c_str());
}

VkShaderModule vka::Shader::get_module(void) const noexcept
{
	return this->_module;
}

VkShaderStageFlagBits vka::Shader::get_stage(void) const noexcept
{
	return this->_stage;
}

const char* vka::Shader::get_entry_point(void) const noexcept
{
	return this->_entry_point;
}

size_t vka::Shader::size(void) const noexcept
{
	return this->_size;
}


vka::ShaderProgram::ShaderProgram(void) noexcept {}

vka::ShaderProgram::ShaderProgram(std::vector<Shader>& shaders)
{
	for (Shader& shader : shaders)
		this->attach(shader);
}

vka::ShaderProgram::~ShaderProgram(void) noexcept {}

void vka::ShaderProgram::attach(Shader& shader)
{
	if (shader.get_module() == VK_NULL_HANDLE)
		throw std::invalid_argument("Module of Shader is a VK_NULL_HANDLE! Requiered from shader program attach.");


	VkPipelineShaderStageCreateInfo shader_stage_create_info = {};
	shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_create_info.pNext = nullptr;
	shader_stage_create_info.flags = 0;
	shader_stage_create_info.stage = shader.get_stage();
	shader_stage_create_info.module = shader.get_module();
	shader_stage_create_info.pName = shader.get_entry_point();
	shader_stage_create_info.pSpecializationInfo = nullptr;

	this->_shaders.push_back(std::move(shader));
	this->_shader_stages.push_back(shader_stage_create_info);
}

void vka::ShaderProgram::clear(void) noexcept
{
	this->_shaders.clear();
	this->_shader_stages.clear();
}

uint32_t vka::ShaderProgram::count(void) const noexcept
{
	return this->_shader_stages.size();
}

const VkPipelineShaderStageCreateInfo* vka::ShaderProgram::get_stages(void) const noexcept
{
	if (this->_shader_stages.size() == 0)
		return nullptr;
	return this->_shader_stages.data();
}