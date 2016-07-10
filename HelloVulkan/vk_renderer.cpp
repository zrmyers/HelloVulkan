#include "vk_renderer.h"

VK_Renderer::VK_Renderer(VK_RendererInfo* rinfo)
{	
	//extension needed for instance
	uint32_t ext_count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&ext_count);

	//information to create instance object
	VK_InstanceInfo instInfo;	
	instInfo.pApplicationName = rinfo->applicationName.c_str();
	instInfo.applicationVersion = VK_MAKE_VERSION(rinfo->app_VMAJOR, rinfo->app_VMINOR, rinfo->app_VPATCH);
	instInfo.pEngineName = rinfo->engineName.c_str();
	instInfo.enabledExtensionCount = ext_count;	
	instInfo.ppEnabledExtensionNames = &extensions[0];
	instInfo.enabledLayerCount = 0;	 
	instInfo.ppEnabledLayerNames = nullptr;
	instInfo.allocs = nullptr;
	instInfo.apiVersion = 0; //current version of vulkan

	//create instance object
	std::cout << "Creating Vulkan Instance!\n";
	if ((instance = new VK_Instance(&instInfo)) == nullptr)
	{
		std::cout << "Error: Failed to create vulkan instance\n";
		return;
	}

	//information to create physical device object
	VK_PhysicalDeviceInfo physInfo;
	physInfo.instance = instance->getInstance();
	physInfo.minAPIVersionMajor = 1;
	physInfo.minAPIVersionMinor = 0;
	physInfo.minAPIVersionPatch = 0;
	physInfo.maxDimm2D = 4096;

	//create physical device object
	std::cout << "Creating Vulkan Physical Device Instance\n";
	if ((physicalDevice = new VK_PhysicalDevice(&physInfo)) == nullptr)
	{
		std::cout << "Error: Failed to obtain physical device with vulkan support!\n";
		return;
	}


	if (!glfwGetPhysicalDevicePresentationSupport(*(instance->getInstance()), *(physicalDevice->getPhysicalDevice()), physicalDevice->getGraphicsQueueFamilyIndex()))
	{
		std::cout << "Error: Instance does not support vulkan presentation!\n";
		return;
	}

	//create a window surface
	VK_SurfaceInfo sinfo;
	sinfo.instance = instance->getInstance();
	sinfo.allocs = nullptr;
	sinfo.window = rinfo->window;

	//information to create logical device object
	VK_DeviceInfo devInfo;
	devInfo.physicalDevice = physicalDevice->getPhysicalDevice();
	devInfo.graphicsQueueFamilyIndex = physicalDevice->getGraphicsQueueFamilyIndex();
	devInfo.enabledExtensionCount = 0;
	devInfo.ppEnabledExtensionNames = nullptr;
	devInfo.enabledLayerCount = 0;
	devInfo.ppEnabledLayerNames = nullptr;
	devInfo.pEnabledFeatures = nullptr;
	devInfo.allocs = nullptr;

	//create logical device object
	std::cout << "Creating Vulkan Device Instance\n";
	if ((device = new VK_Device(&devInfo)) == nullptr)
	{
		std::cout << "Error: Failed to create logical device!\n";
		return;
	}

	//information to create graphics queue
	VK_QueueInfo qinfo;
	qinfo.device = device->getDevice();
	qinfo.queueFamilyIndex = physicalDevice->getGraphicsQueueFamilyIndex();
	qinfo.queueIndex = 0;

	//create queue instance
	std::cout << "Creating Vulkan Graphics Queue Instance\n";
	if ((queue = new VK_Queue(&qinfo)) == nullptr)
	{
		std::cout << "Error: Failed to create queue!\n";
		return;
	}
}

VK_Renderer::~VK_Renderer()
{
	std::cout << "Destroying Vulkan Graphics Queue Instance\n";
	if (queue != nullptr) delete queue;
	std::cout << "Destroying Vulkan Device Instance\n";
	if (device != nullptr) delete device;
	std::cout << "Destroying Vulkan Physical Device Instance\n";
	if (physicalDevice != nullptr) delete physicalDevice;
	std::cout << "Destroying Vulkan Instance\n";
	if (instance != nullptr) delete instance;
	
}