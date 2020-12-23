﻿#pragma once
#include "Base/GenerationalId.h"
#include "Containers/constexpr_map.hpp"
#include "System/vfs/path.h"
#include "System/Window.h"

namespace sakura::graphics
{
	struct RenderCommandBuffer;
	class RenderGraph;
	namespace ____
	{
		template<bool True> struct uhalf {};
		template<> struct uhalf<sizeof(size_t) == sizeof(uint32)> { using type = uint16; constexpr static uint16 max = UINT16_MAX; };
		template<> struct uhalf<sizeof(size_t) == sizeof(uint64)> { using type = uint32; constexpr static uint32 max = UINT32_MAX; };
		static_assert(sizeof(uhalf<true>::type) == sizeof(size_t) / 2, "uhalf type error! size should be half as size_t.");
	};
}

namespace sakura::graphics
{
	enum class EBackend : uint16
	{
		Stub,
		D3D12,
		Vulkan,
		WebGPU,
		Metal,
		PS5, //PlayStation 5
		Count
	};

	enum class EIndexFormat : uint8
	{
		UINT16 = 0x00000001,
		UINT32 = 0x00000002,
	};

	enum class ESharingMode : uint8
	{
		Exclusive = 0,
		Concurrent = 1
	};
	
	enum class EVertexFormat : uint16
	{
		UCHAR2,
		UCHAR4,
		CHAR2,
		CHAR4,
		UCHAR2_NORM,
		UCHAR4_NORM,
		CHAR2_NORM,
		CHAR4_NORM,
		USHORT2,
		USHORT4,
		SHORT2,
		SHORT4,
		USHORT2_NORM,
		USHORT4_NORM,
		SHORT2_NORM,
		SHORT4_NORM,
		HALF2,
		HALF4,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		UINT,
		UINT2,
		UINT3,
		UINT4,
		INT,
		INT2,
		INT3,
		INT4,
		COUNT
	};

	enum class ETextureFormat : uint16
	{
		R32G32B32A32_UINT = 0,
		R32G32B32A32_SINT = 1,
		R32G32B32A32_FLOAT = 2,
		R32G32B32_UINT = 3,
		R32G32B32_SINT = 4,
		R32G32B32_FLOAT = 5,
		R16G16B16A16_UNORM = 6,
		R16G16B16A16_SNORM = 7,
		R16G16B16A16_UINT = 8,
		R16G16B16A16_SINT = 9,
		R16G16B16A16_FLOAT = 10,
		R32G32_UINT = 11,
		R32G32_SINT = 12,
		R32G32_FLOAT = 13,
		R10G10B10A2_UNORM = 14,
		R10G10B10A2_UINT = 15,
		R9G9B9E5_UFLOAT = 16,
		R8G8B8A8_UNORM = 17,
		R8G8B8A8_SNORM = 18,
		R8G8B8A8_UINT = 19,
		R8G8B8A8_SINT = 20,
		R8G8B8A8_UNORM_SRGB = 21,
		B8G8R8A8_UNORM = 22,
		B8G8R8A8_UNORM_SRGB = 23,
		R11G11B10_FLOAT = 24,
		R16G16_UNORM = 25,
		R16G16_SNORM = 26,
		R16G16_UINT = 27,
		R16G16_SINT = 28,
		R16G16_FLOAT = 29,
		R32_UINT = 30,
		R32_SINT = 31,
		R32_FLOAT = 32,
		B5G5R5A1_UNORM = 33,
		B5G6R5_UNORM = 34,
		R8G8_UNORM = 35,
		R8G8_SNORM = 36,
		R8G8_UINT = 37,
		R8G8_SINT = 38,
		R16_UNORM = 39,
		R16_SNORM = 40,
		R16_UINT = 41,
		R16_SINT = 42,
		R16_FLOAT = 43,
		R8_UNORM = 44,
		R8_SNORM = 45,
		R8_UINT = 46,
		R8_SINT = 47,
		D24_UNORM_S8_UINT = 49,
		D32_FLOAT = 50,
		D16_UNORM = 51,
		BC1_UNORM = 52,
		BC1_UNORM_SRGB = 53,
		BC2_UNORM = 54,
		BC2_UNORM_SRGB = 55,
		BC3_UNORM = 56,
		BC3_UNORM_SRGB = 57,
		BC4_UNORM = 58,
		BC4_SNORM = 59,
		BC5_UNORM = 60,
		BC5_SNORM = 61,
		BC6H_UF16 = 62,
		BC6H_SF16 = 63,
		BC7_UNORM = 64,
		BC7_UNORM_SRGB = 65,
		Count = BC7_UNORM_SRGB + 1
	};

	enum class ERenderCommandType : uint32_t
	{
		set_scissor_rect,
		draw,
		draw_indirect,

		draw_instanced_with_args,

		dispatch,
		dispatch_indirect,

		update_binding,

		copy_buffer_to_buffer,
		copy_buffer_to_texture,
		copy_texture_to_texture,
		copy_texture_to_buffer,

		fence, // ? necessary ?
		// GPUQueue wait GPUQueue (semaphores) ==> Pass dependencies.
		// GPU wait CPU ==> .....Seems useless, why not ComputePipeline ?
		// CPU wait GPU ==> What about non-block callback pass->on_finish() ?

		barriers, // optional
		transitions, // optional

		begin_timing,
		end_timing,
		resolve_timings,

		begin_event,
		end_event,

		begin_render_pass,
		end_render_pass,

		begin_compute_pass,
		end_compute_pass,

		ray_trace,
		update_top_level,
		update_bottom_level,
		update_shader_table,
		count
	};

	enum ERenderQueueType
	{
		QueueTypeNone = 0x00,
		Copy = 0x01,
		Compute = 0x02,
		Graphics = 0x04,
		QueueTypeCopy = Copy,
		QueueTypeCompute = Compute,
		QueueTypeGraphics = Graphics,
		QueueTypeAll = Copy | Compute | Graphics
	};

	enum class EBufferCPUAccess : uint8
	{
		None,
		Read,
		Write,
		ReadWrite
	};

	// Shader Frequency
	enum class EShaderFrequency : uint32
	{
		None = 0x00000000,
		// Default pipeline 3
		VertexShader = 0x00000001,
		PixelShader = 0x00000002,
		ComputeShader = 0x00000004,
		// Mesh pipeline 2
		TaskShader = 0x00000010,
		MeshShader = 0x00000020,
		// RayTracing 6
		AnyHitShader = 0x00000100,
		CallableShader = 0x00000200,
		ClosestHitShader = 0x00000400,
		IntersectionShader = 0x00000800,
		MissShader = 0x00001000,
		RayGenerationShader = 0x00002000,
		// 1 + 3 + 2 + 6
		Count = 12
	};

	RenderGraphAPI bool isMeshPipelineStage(const EShaderFrequency freq);
	RenderGraphAPI bool isRayTracingStage(const EShaderFrequency freq);
	RenderGraphAPI bool isRenderPipelineStage(const EShaderFrequency freq);

	enum class EShaderCodeFormat
	{
		Spirv,
		DXBC,
		DXIL,
		GLSL,
		PSSL2, //PlayStation 5
		Count
	};

	enum class EPrimitiveTopology : uint32
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		Count
	};

	enum class EPolygonMode
	{
		FILL = 0,
		LINE = 1,
		POINT = 2
	};

	enum class EBlendOp : uint8
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};

	enum class EBlendFactor : uint8
	{
		Zero = 0x00,
		One = 0x01,
		SrcColor = 0x02,
		OneMinusSrcColor = 0x03,
		SrcAlpha = 0x04,
		OneMinusSrcAlpha = 0x05,
		DstColor = 0x06,
		OneMinusDstColor = 0x07,
		DstAlpha = 0x08,
		OneMinusDstAlpha = 0x09,
		SrcAlphaSaturated = 0x0A,
		BlendColor = 0x0B,
		OneMinusBlendColor = 0x0C
	};

	enum EColorMaskBits : uint8
	{
		None = 0x00,
		R = 0x01,
		G = 0x02,
		B = 0x04,
		A = 0x08,
		All = 0x0F,
	};
	using ColorMask = uint8;

	enum class ELoadOp : uint8
	{
		Clear = 0,
		Load = 1,
		DontCare = 2,
		Count
	};

	enum class EStoreOp : uint8
	{
		Store = 0,
		Clear = 1,
		DontCare = 2,
		Count
	};

	enum class ECullMode : uint8
	{
		None = 0,
		Back = 1,
		Front = 2,
		FrontAndBack = 3,
		Count
	};

	using RenderGraphId = sakura::GenerationalId;
	struct RenderGraphAPI RenderObjectHandle
	{
		RenderObjectHandle(const RenderGraphId _id);
		inline operator size_t() const { return _id; }
		inline operator bool() const { return _id; }
		inline RenderGraphId id() const { return _id; }
	protected:
		RenderObjectHandle();
		RenderGraphId _id;
	};
	inline bool operator==(const RenderObjectHandle& lhs, const RenderObjectHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}

	struct RenderGraphAPI RenderPassHandle
	{
		RenderPassHandle(const RenderGraphId _id);
		inline operator size_t() const { return _id; }
		inline operator bool() const { return _id; }
		inline RenderGraphId id() const { return _id; }
	protected:
		RenderPassHandle();
		RenderGraphId _id;
	};
	inline bool operator==(const RenderPassHandle& lhs, const RenderPassHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}

	struct RenderGraphAPI RenderResourceHandle
	{
		RenderResourceHandle(const RenderGraphId _id);
		RenderResourceHandle();
		inline operator size_t() const { return _id; }
		inline operator bool() const { return _id; }
		inline RenderGraphId id() const { return _id; }
	protected:
		RenderGraphId _id;
	};
	inline bool operator==(const RenderResourceHandle& lhs, const RenderResourceHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}

	using extent2d = sakura::extent2d;
	using extent3d = sakura::extent3d;

	struct RenderGraphAPI IGpuObject
	{
		virtual ~IGpuObject() = default;
		virtual RenderObjectHandle handle() const = 0;
		virtual sakura::string format() const { return ""; }
	};

	struct RenderGraphAPI IGpuMemoryResource
	{
		virtual ~IGpuMemoryResource() = default;
		virtual RenderResourceHandle handle() const = 0;
		virtual size_t size() const = 0;
		virtual sakura::string format() const { return ""; }
	};

	template<typename _ResourceType>
	struct TypedRenderResourceHandle : public RenderResourceHandle
	{
		using ResourceType = _ResourceType;
		inline TypedRenderResourceHandle(const RenderGraphId _id)
			:RenderResourceHandle(_id)
		{

		}
		TypedRenderResourceHandle() = default;
		static_assert(std::is_base_of_v<IGpuMemoryResource, ResourceType>, "Resource Handle must be used with GPU Resources!");
	};
	template<typename _ObjectType>
	struct TypedRenderObjectHandle : public RenderObjectHandle
	{
		using ObjectType = _ObjectType;
		TypedRenderObjectHandle(const RenderGraphId _id)
			:RenderObjectHandle(_id)
		{

		}
		TypedRenderObjectHandle() = default;
		static_assert(std::is_base_of_v<IGpuObject, ObjectType>, "GPUObject Handle must be used with GPU Objects!");
	};






	using BufferUsages = uint32;
	struct RenderGraphAPI BufferDesc
	{
		enum Usage
		{
			Unknown,

			CopySrc,
			CopyDst,

			IndexBuffer,
			VertexBuffer,
			UniformBuffer,
			IndirectBuffer,
			StorageBuffer,
			Query,
			RayTracingAccelerateStructure
		};
		const BufferUsages usages = Usage::UniformBuffer;
		const EBufferCPUAccess access = EBufferCPUAccess::None;
		const size_t length = 0;
		const void* data = nullptr;
		BufferDesc(BufferUsages usage = Usage::UniformBuffer,
			size_t length = 0, const void* data = nullptr,
			const EBufferCPUAccess access = EBufferCPUAccess::None);
	};
	using EBufferUsage = BufferDesc::Usage;

	using TextureUsages = uint32;
	using TextureFlags = uint32;
	struct RenderGraphAPI TextureDesc
	{
		enum Usage
		{
			None = 0,
			CopySrc = 0x00000001,
			CopyDst = 0x00000002,
			Sampled = 0x00000004,
			Storage = 0x00000008,
			Attachment = 0x00000010,
			Present = 0x00000020
		};
		enum Dimension
		{
			Texture1D = 0,
			Texture2D = 1,
			Texture3D = 2
		};
		using Flags = uint32;
		using Usages = uint32;

		ETextureFormat format;
		Dimension dimension = Dimension::Texture2D;
		Usages usages = 0;
		uint32 sample_count = 1;
		uint32 mip_levels = 1;
		extent3d size;

		// Advanced Features
		enum Flag
		{
			SparseBinding = 0x00000001,
			SparseResidency = 0x00000002,
			SparseAliased = 0x00000004,
			MutableFormat = 0x00000008,
			Cube = 0x00000010,
			T2DArray = 0x00000020,
			Alias = 0x00000400,
			BlockButBitCompatible = 0x00000080,
		};
		Flags flags = 0;
		bool force_linear = false;
		uint32 array_layers = 1;
		// multi-queue support.
		ESharingMode sharing_mode = ESharingMode::Exclusive;
	};

	struct RenderGraphAPI SamplerDesc
	{
		
	};
	
	using ETextureDimension = TextureDesc::Dimension;
	using ETextureFlag = TextureDesc::Flag;
	using ETextureUsage = TextureDesc::Usage;
	
	struct RenderGraphAPI IGpuShader : public IGpuMemoryResource
	{
		virtual EShaderFrequency frequency() const = 0;
		
		virtual sakura::string_view entry_name() const = 0;
		
		virtual EShaderCodeFormat code_format() const = 0;
	};
	struct RenderGraphAPI IGpuBuffer : public IGpuMemoryResource
	{
		virtual BufferUsages usages() const = 0;
	};
	struct RenderGraphAPI IGpuTexture : public IGpuMemoryResource
	{
		virtual uint32 width() const = 0;
		
		virtual uint32 height() const = 0;
		
		virtual uint32 depth() const = 0;
		
		virtual ETextureFormat render_format() const = 0;

		virtual RenderResourceHandle handle() const = 0;
		
		virtual TextureUsages usages() const = 0;
	};
	struct RenderGraphAPI IGpuSampler : public IGpuMemoryResource
	{
		
	};
	using GpuShaderHandle = TypedRenderResourceHandle<IGpuShader>;
	using GpuBufferHandle = TypedRenderResourceHandle<IGpuBuffer>;
	using GpuTextureHandle = TypedRenderResourceHandle<IGpuTexture>;
	using GpuSamplerHandle = TypedRenderResourceHandle<IGpuSampler>;

	struct RenderGraphAPI ISwapChain : public IGpuObject
	{
		virtual uint8 buffer_count() const = 0;
		virtual extent2d extent() const = 0;
		virtual sakura::Window window() const = 0;
		virtual ETextureFormat render_format() const = 0;
		virtual bool present() = 0;
		//virtual RenderAttachmentHandle back_buffer() const = 0;
	};
	using SwapChainHandle = TypedRenderObjectHandle<ISwapChain>;
	
	struct RenderGraphAPI IRenderPipeline : public IGpuObject
	{

	};
	using RenderPipelineHandle = TypedRenderObjectHandle<IRenderPipeline>;
	
	struct RenderGraphAPI IComputePipeline : public IGpuObject
	{

	};
	using ComputePipelineHandle = TypedRenderObjectHandle<IComputePipeline>;
	
	struct RenderGraphAPI IFence : public IGpuObject
	{
		//virtual void wait() = 0;
		
		//virtual bool completed() const = 0;
	};
	using FenceHandle = TypedRenderObjectHandle<IFence>;

	using QueueIndex = size_t;


	struct RenderGraphAPI TextureDataLayout
	{
		uint64_t offset = 0;
		uint32_t bytes_per_raw = 0;
		uint32_t rows_per_image = 0;
	};
	struct RenderGraphAPI TextureSlice
	{
		enum EAspect
		{
			All,
			StencilOnly,
			DepthOnly
		};
		EAspect aspect = EAspect::All;
		uint32_t mip_level = 1;
		pos3d origin = { 0, 0, 0 };
	};
	using ETextureAspect = TextureSlice::EAspect;
	
	struct RenderGraphAPI ShaderLayout
	{
		const GpuShaderHandle* shaders = nullptr;
		uint32 count = 0;
		ShaderLayout() = default;
		template<size_t N>
		constexpr explicit ShaderLayout(const GpuShaderHandle(&shaders)[N]);
	};
	template <size_t N>
	constexpr ShaderLayout::ShaderLayout(const GpuShaderHandle(&_shaders)[N])
		:shaders(_shaders), count(N)
	{
		
	}

	struct RenderGraphAPI VertexLayout
	{
		enum class Frequency
		{
			PerVertexData,
			PerInstanceData
		};
		struct RenderGraphAPI Element
		{
			Element() = default;
			Element(string semantic_name, EVertexFormat format, uint32 offset) noexcept;
			sakura::string semantic_name = "";
			EVertexFormat format = EVertexFormat::FLOAT;
			uint32 offset = 0;
		};
		sakura::vector<Element> elements;
		Frequency freq = Frequency::PerVertexData;
		uint32 stride = 0;
		VertexLayout() = default;
		template<size_t N>
		VertexLayout(
			const VertexLayout::Element(&vertex_elements)[N], const VertexLayout::Frequency frequecy, uint32 stride);
	};
	template <size_t N>
	VertexLayout::VertexLayout(
		const VertexLayout::Element(&vertex_elements)[N], const VertexLayout::Frequency frequecy, uint32 _stride)
		:elements(vertex_elements, vertex_elements + N), freq(frequecy), stride(_stride)
	{
		
	}

	struct RenderGraphAPI BindingLayout
	{
		enum EType
		{
			UniformBuffer = 0x00000000,
			StorageBuffer = 0x00000001,
			ReadonlyStorageBuffer = 0x00000002,
			Sampler = 0x00000003,
			ComparisonSampler = 0x00000004,
			SampledTexture = 0x00000005,
			MultisampledTexture = 0x00000006,
			ReadonlyStorageTexture = 0x00000007,
			WriteonlyStorageTexture = 0x00000008,
			Count = WriteonlyStorageTexture + 2,
			Invalid = 0x7FFFFFFF
		};
		struct RenderGraphAPI Slot
		{
			Slot(uint32 binding, EType binding_type, EShaderFrequency visibility, uint32 count = 1) noexcept;
			Slot() = default;
			uint32 binding = 0;
			EType binding_type = EType::UniformBuffer;
			EShaderFrequency visibility = EShaderFrequency::None;
			uint32 count = 1;
		};
		struct RenderGraphAPI Set
		{
			Set(const Slot* slots, uint32 slots_count) noexcept;
			Set() = default;
			template<size_t N>
			explicit constexpr Set(const Slot(&)[N]);
			const sakura::vector<Slot> slots;
		};
		BindingLayout() = default;
		template<size_t N>
		explicit constexpr BindingLayout(const Set(&)[N]);
		const sakura::vector<BindingLayout::Set> tables;
	};
	template <size_t N>
	constexpr BindingLayout::Set::Set(const Slot(& _slots)[N])
		:slots(_slots, _slots + N)
	{
		
	}
	template <size_t N>
	constexpr BindingLayout::BindingLayout(const Set(& _tables)[N])
		:tables(_tables, _tables + N)
	{
		
	}
	using BindingSlot = BindingLayout::Slot;
	using BindingTable = BindingLayout::Set;
	
	struct RenderGraphAPI AttachmentLayout
	{
		struct RenderGraphAPI Slot
		{
			ETextureFormat format; // size = 16
			bool blend_enable = false;
			ELoadOp load_operation = ELoadOp::Count;
			ELoadOp stencil_load_operation = ELoadOp::DontCare;
			EStoreOp store_operation = EStoreOp::Count;
			EStoreOp stencil_store_operation = EStoreOp::DontCare;
			struct BlendDesc
			{
				ELoadOp load_operation = ELoadOp::Count;
				EStoreOp store_opeartion = EStoreOp::Count;
				EBlendOp operation = EBlendOp::Add;
				EBlendFactor src_factor = EBlendFactor::SrcAlpha;
				EBlendFactor dst_factor = EBlendFactor::OneMinusSrcAlpha;
			} alpha_blend, color_blend; // size = 32 * 2
			ColorMask write_mask = EColorMaskBits::All; // size = 8
			Slot(ETextureFormat format, ELoadOp loadOp, EStoreOp storeOp,
				ELoadOp stencilLoadOp = ELoadOp::DontCare, EStoreOp stencilStoreOp = EStoreOp::DontCare) noexcept;
			Slot() = default;
		};
		AttachmentLayout() = default;
		template<size_t N>
		explicit constexpr AttachmentLayout(const Slot(&)[N]);
		const sakura::vector<Slot> slots;
	};
	template <size_t N>
	constexpr AttachmentLayout::AttachmentLayout(const Slot(& attachments)[N])
		:slots(attachments, attachments + N)
	{
		
	}
	using AttachmentSlot = AttachmentLayout::Slot;
	using BlendDesc = AttachmentSlot::BlendDesc;

	enum class EPresentMode : uint8
	{
		Immediate = 0,
		FIFO = 1,
		Mailbox = 2,
		Count = 3
	};

	struct RenderGraphAPI FenceDesc
	{
		const uint64 initial_value = 0;
		
		FenceDesc() = default;
		FenceDesc(const uint64 initialValue) noexcept;
	};
	
	struct RenderGraphAPI SwapChainDesc
	{
		const EPresentMode present_mode = EPresentMode::Mailbox;
		const Window window = sakura::Window();
		const uint8 buffer_count = 2;
		
		SwapChainDesc() = default;
		SwapChainDesc(const EPresentMode presentMode, const Window window, const uint8 bufferCount) noexcept;
	};
	
	struct RenderGraphAPI RenderPipelineDesc
	{
		EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList;
		uint8 sample_count = 1;
		uint32 sample_mask = 0xFFFFFFFF;

		ECullMode cull_mode = ECullMode::Back;
		EPolygonMode polygon_mode = EPolygonMode::FILL;
		float lineWidth = 1.f;

		ShaderLayout shader_layout;
		const sakura::vector<VertexLayout> vertex_layout;
		BindingLayout binding_layout;
		AttachmentLayout attachment_layout;

		RenderPipelineDesc() = default;
		explicit RenderPipelineDesc(
			const ShaderLayout& shader_layout, const VertexLayout& vertex_layout,
			const BindingLayout& binding_layout, const AttachmentLayout& attachment_layout,
			const ECullMode cull_mode = ECullMode::Back,
			const EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList,
			const EPolygonMode polygon_mode = EPolygonMode::FILL,
			uint8 sample_count = 1, uint32 sample_mask = 0xFFFFFFFF);
		
		template<size_t N>
		explicit RenderPipelineDesc(
			const ShaderLayout& shader_layout, const VertexLayout(&vertex_layouts)[N],
			const BindingLayout binding_layout, const AttachmentLayout& attachment_layout,
			const ECullMode cull_mode = ECullMode::Back,
			const EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList,
			const EPolygonMode polygon_mode = EPolygonMode::FILL,
			uint8 sample_count = 1, uint32 sample_mask = 0xFFFFFFFF);
	};
	template <size_t N>
	RenderPipelineDesc::RenderPipelineDesc(
		const ShaderLayout& _shader_layout, const VertexLayout(&_vertex_layouts)[N],
		const BindingLayout _binding_layout, const AttachmentLayout& _attachment_layout,
		const ECullMode _cull_mode,
		const EPrimitiveTopology _primitive_topology, const EPolygonMode _polygon_mode,
		uint8 _sample_count, uint32 _sample_mask)
		: primitive_topology(_primitive_topology), sample_count(_sample_count), sample_mask(_sample_mask),
		shader_layout(_shader_layout), vertex_layout(_vertex_layouts, _vertex_layouts + N), binding_layout(_binding_layout),
		attachment_layout(_attachment_layout), polygon_mode(_polygon_mode), cull_mode(_cull_mode)
	{
	}


	
	struct RenderGraphAPI Attachment
	{
		struct RenderGraphAPI Slot
		{
			Slot() = default;
			Slot(GpuTextureHandle attachmentHandle,
				double4 clearColor = double4(), ELoadOp loadOp = ELoadOp::Clear, EStoreOp storeOp = EStoreOp::Store)
				:attachment(attachmentHandle), load_op(loadOp), store_op(storeOp), clear_color(clearColor)
			{
				  
			}
			Slot(SwapChainHandle attachmentHandle,
				double4 clearColor = double4(), ELoadOp loadOp = ELoadOp::Clear, EStoreOp storeOp = EStoreOp::Store)
				:attachment(attachmentHandle), load_op(loadOp), store_op(storeOp), clear_color(clearColor)
			{
				  
			}
			sakura::variant<GpuTextureHandle, SwapChainHandle> attachment;
			ELoadOp load_op = ELoadOp::Clear;
			EStoreOp store_op = EStoreOp::Store;
			double4 clear_color = { 0, 0, 0, 0 };
		};
		sakura::vector<Slot> slots;
		
		template<size_t N>
		Attachment(const Slot(&slots)[N]) noexcept;
		
		Attachment() = default;
 	};

	template <size_t N>
	Attachment::Attachment(const Slot(&_slots)[N]) noexcept
		:slots(_slots, _slots + N)
	{
		
	}

	struct RenderGraphAPI Binding
	{
		struct RenderGraphAPI Slot
		{
			uint32 slot_index = 0;
			uint32 size = 0;
			uint32 offset = 0;
			GpuBufferHandle buffer = RenderGraphId::UNINITIALIZED;
			Slot(GpuBufferHandle buffer = RenderGraphId::UNINITIALIZED,
				uint32 index_on_set = 0, uint32 size = 0, uint32 offset = 0);
		};
		struct RenderGraphAPI Set
		{
			Set() = default;
			template<size_t N>
			Set(const Slot(&slots)[N]) noexcept;
			template<typename I, size_t N>
			Set(const Slot(&slots)[N], const I(&dynamic_offsets)[N]) noexcept;
			const sakura::vector<Slot> slots;
			const sakura::vector<uint32_t> dynamic_offsets;
		};
		const sakura::vector<Set> sets;
		Binding() = default;
		template<size_t N>
		Binding(const Set(&sets)[N]) noexcept;
	};

	template <size_t N>
	Binding::Set::Set(const Slot(& _slots)[N]) noexcept
		: slots(_slots, _slots + N), dynamic_offsets(N, 0)
	{
		
	}
	
	template <typename I, size_t N>
	Binding::Set::Set(const Slot(&_slots)[N], const I(&offsets)[N]) noexcept
		:slots(_slots, _slots + N), dynamic_offsets(offsets, offsets + N)
	{

	}
	
	template <size_t N>
	Binding::Binding(const Set(& _sets)[N]) noexcept
		:sets(_sets, _sets + N)
	{

	}



	// Shader Description struct
	struct RenderGraphAPI ShaderDesc
	{
		sakura::string name = "";
		sakura::string entry = "";
		EShaderFrequency frequency = EShaderFrequency::None;

		const sakura::span<const std::byte> code;

		ShaderDesc() = default;
		explicit ShaderDesc(const sakura::string& name,
			const sakura::string& entry, const EShaderFrequency frequency,
			const sakura::span<const std::byte> code = {}) noexcept;
	};

}













namespace sakura::graphics
{
	namespace detail
	{
		constexpr auto ShaderFrequencyNameLut = sakura::map_c<sakura::graphics::EShaderFrequency, sakura::string_view>(
			{
				{ sakura::graphics::EShaderFrequency::None, "None" },
				{ sakura::graphics::EShaderFrequency::VertexShader, "VertexShader" },
				{ sakura::graphics::EShaderFrequency::PixelShader, "PixelShader" },
				{ sakura::graphics::EShaderFrequency::ComputeShader, "ComputeShader" },

				{ sakura::graphics::EShaderFrequency::TaskShader, "TaskShader" },
				{ sakura::graphics::EShaderFrequency::MeshShader, "MeshShader" },

				{ sakura::graphics::EShaderFrequency::AnyHitShader, "AnyHitShader" },
				{ sakura::graphics::EShaderFrequency::CallableShader, "CallableShader" },
				{ sakura::graphics::EShaderFrequency::ClosestHitShader, "ClosestHitShader" },
				{ sakura::graphics::EShaderFrequency::IntersectionShader, "IntersectionShader" },
				{ sakura::graphics::EShaderFrequency::MissShader, "MissShader" },
				{ sakura::graphics::EShaderFrequency::RayGenerationShader, "RayGenerationShader" },
			}
		);
		static_assert(ShaderFrequencyNameLut.size() == static_cast<int>(EShaderFrequency::Count),
			"Found ShaderFrequency Not Registed Into LUT!");

		constexpr auto LoadOpNameLut = sakura::map_c<sakura::graphics::ELoadOp, sakura::string_view>(
			{
				{ sakura::graphics::ELoadOp::Clear, "Clear" },
				{ sakura::graphics::ELoadOp::Load, "Load" },
				{ sakura::graphics::ELoadOp::DontCare, "DontCare" },
			}
		);
		static_assert(LoadOpNameLut.size() == static_cast<int>(ELoadOp::Count),
			"Found LoadOp Not Registed Into LUT!");
		
		constexpr auto StoreOpNameLut = sakura::map_c<sakura::graphics::EStoreOp, sakura::string_view>(
			{
				{ sakura::graphics::EStoreOp::Clear, "Clear" },
				{ sakura::graphics::EStoreOp::Store, "Store" },
				{ sakura::graphics::EStoreOp::DontCare, "DontCare" },
			}
		);
		static_assert(StoreOpNameLut.size() == static_cast<int>(EStoreOp::Count),
			"Found StoreOp Not Registed Into LUT!");
		
		
		constexpr auto BindingTypeNameLut = sakura::map_c<sakura::graphics::BindingLayout::EType, sakura::string_view>(
			{
				{ sakura::graphics::BindingLayout::EType::Invalid, "Invalid" },
				{ sakura::graphics::BindingLayout::EType::UniformBuffer, "UniformBuffer" },
				{ sakura::graphics::BindingLayout::EType::StorageBuffer, "StorageBuffer" },
				{ sakura::graphics::BindingLayout::EType::ReadonlyStorageBuffer, "ReadonlyStorageBuffer" },

				{ sakura::graphics::BindingLayout::EType::Sampler, "Sampler" },
				{ sakura::graphics::BindingLayout::EType::ComparisonSampler, "ComparisonSampler" },

				{ sakura::graphics::BindingLayout::EType::SampledTexture, "SampledTexture" },
				{ sakura::graphics::BindingLayout::EType::MultisampledTexture, "MultisampledTexture" },
				{ sakura::graphics::BindingLayout::EType::ReadonlyStorageTexture, "ReadonlyStorageTexture" },
				{ sakura::graphics::BindingLayout::EType::WriteonlyStorageTexture, "WriteonlyStorageTexture" },
			}
		);
		static_assert(BindingTypeNameLut.size() == static_cast<int>(BindingLayout::EType::Count),
			"Found BindingType Not Registed Into LUT!");

		constexpr auto ShaderFormatNameLut = sakura::map_c<sakura::graphics::EShaderCodeFormat, sakura::string_view>(
			{
				{ sakura::graphics::EShaderCodeFormat::PSSL2, "PSSL2" },
				{ sakura::graphics::EShaderCodeFormat::DXBC, "DXBC" },
				{ sakura::graphics::EShaderCodeFormat::DXIL, "DXIL" },
				{ sakura::graphics::EShaderCodeFormat::GLSL, "GLSL" },
				{ sakura::graphics::EShaderCodeFormat::Spirv, "Spirv" },
			}
		);
		static_assert(ShaderFormatNameLut.size() == static_cast<int>(EShaderCodeFormat::Count),
			"Found ShaderFrequency Not Registed Into LUT!");

		constexpr auto BlendOpNameLut = sakura::map_c<sakura::graphics::EBlendOp, sakura::string_view>(
			{
				{ sakura::graphics::EBlendOp::Add, "Add" },
				{ sakura::graphics::EBlendOp::Subtract, "Subtract" },
				{ sakura::graphics::EBlendOp::ReverseSubtract, "ReverseSubtract" },
				{ sakura::graphics::EBlendOp::Min, "Min" },
				{ sakura::graphics::EBlendOp::Max, "Max" },
			}
		);
		constexpr auto BlendFactorNameLut = sakura::map_c<sakura::graphics::EBlendFactor, sakura::string_view>(
			{
				{ sakura::graphics::EBlendFactor::Zero, "Zero" },
				{ sakura::graphics::EBlendFactor::One, "One" },
				{ sakura::graphics::EBlendFactor::SrcColor, "SrcColor" },
				{ sakura::graphics::EBlendFactor::OneMinusSrcColor, "OneMinusSrcColor" },
				{ sakura::graphics::EBlendFactor::SrcAlpha, "SrcAlpha" },
				{ sakura::graphics::EBlendFactor::OneMinusSrcAlpha, "OneMinusSrcAlpha" },
				{ sakura::graphics::EBlendFactor::DstColor, "DstColor" },
				{ sakura::graphics::EBlendFactor::OneMinusDstColor, "OneMinusDstColor" },
				{ sakura::graphics::EBlendFactor::DstAlpha, "DstAlpha" },
				{ sakura::graphics::EBlendFactor::OneMinusDstAlpha, "OneMinusDstAlpha" },
				{ sakura::graphics::EBlendFactor::SrcAlphaSaturated, "SrcAlphaSaturated" },
				{ sakura::graphics::EBlendFactor::BlendColor, "BlendColor" },
				{ sakura::graphics::EBlendFactor::OneMinusBlendColor, "OneMinusBlendColor" },
			}
		);
		constexpr auto ColorMaskNameLut = sakura::map_c<sakura::graphics::EColorMaskBits, sakura::string_view>(
			{
				{ sakura::graphics::EColorMaskBits::None, "None" },
				{ sakura::graphics::EColorMaskBits::R, "R" },
				{ sakura::graphics::EColorMaskBits::G, "G" },
				{ sakura::graphics::EColorMaskBits::B, "B" },
				{ sakura::graphics::EColorMaskBits::A, "A" },
				{ sakura::graphics::EColorMaskBits::All, "All" },
			}
		);

	}
}

namespace std
{
	template<> struct hash<sakura::graphics::RenderResourceHandle> 
	{
		sakura::size_t operator()(sakura::graphics::RenderResourceHandle handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};
	
	template<> struct hash<sakura::graphics::RenderObjectHandle>
	{
		sakura::size_t operator()(sakura::graphics::RenderObjectHandle handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};
	
	template<typename ResourceType> struct hash<sakura::graphics::TypedRenderResourceHandle<ResourceType>>
	{
		sakura::size_t operator()(sakura::graphics::TypedRenderResourceHandle<ResourceType> handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};

	template<typename ObjectType> struct hash<sakura::graphics::TypedRenderObjectHandle<ObjectType>>
	{
		sakura::size_t operator()(sakura::graphics::TypedRenderObjectHandle<ObjectType> handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};
}

namespace fmt
{
	template<> struct formatter<sakura::graphics::BindingLayout::EType> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::BindingLayout::EType type, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view bindingTypeName = "Binding::EType:Undefined";
			if (BindingTypeNameLut.find(type) != BindingTypeNameLut.end())
				bindingTypeName = BindingTypeNameLut.find(type)->second;
			return fmt::formatter<sakura::string_view>::format(bindingTypeName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EShaderCodeFormat> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EShaderCodeFormat fmt, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view fmtName = "ShaderCodeFormat:Undefined";
			if (ShaderFormatNameLut.find(fmt) != ShaderFormatNameLut.end())
				fmtName = ShaderFormatNameLut.find(fmt)->second;
			return fmt::formatter<sakura::string_view>::format(fmtName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EShaderFrequency> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EShaderFrequency freq, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view freqName = "ShaderFrequency:Undefined";
			if (ShaderFrequencyNameLut.find(freq) != ShaderFrequencyNameLut.end())
				freqName = ShaderFrequencyNameLut.find(freq)->second;
			return fmt::formatter<sakura::string_view>::format(freqName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EBlendOp> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EBlendOp op, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view opName = "BlendOp:Undefined";
			if (BlendOpNameLut.find(op) != BlendOpNameLut.end())
				opName = BlendOpNameLut.find(op)->second;
			return fmt::formatter<sakura::string_view>::format(opName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EBlendFactor> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EBlendFactor factor, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view factorName = "EBlendFactor:Undefined";
			if (BlendFactorNameLut.find(factor) != BlendFactorNameLut.end())
				factorName = BlendFactorNameLut.find(factor)->second;
			return fmt::formatter<sakura::string_view>::format(factorName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EColorMaskBits> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EColorMaskBits mask, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view maskName = "EColorMaskBits:Undefined";
			if (ColorMaskNameLut.find(mask) != ColorMaskNameLut.end())
				maskName = ColorMaskNameLut.find(mask)->second;
			return fmt::formatter<sakura::string_view>::format(maskName, ctx);
		}
	};
	
	template<> struct formatter<sakura::graphics::RenderResourceHandle> : fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::RenderResourceHandle handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};
	template<> struct formatter<sakura::graphics::RenderObjectHandle> : fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::RenderObjectHandle handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};

	template<typename ResourceType> struct formatter<sakura::graphics::TypedRenderResourceHandle<ResourceType>>
		: fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::TypedRenderResourceHandle<ResourceType> handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};
	template<typename ObjectType> struct formatter<sakura::graphics::TypedRenderObjectHandle<ObjectType>>
		: fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::TypedRenderObjectHandle<ObjectType> handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};

	
	template<> struct formatter<sakura::graphics::ShaderDesc> : fmt::formatter<sakura::vfs::path>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::ShaderDesc desc, FormatContext& ctx)
		{
			using namespace sakura::graphics::detail;
			constexpr sakura::string_view pathTitle = "Shader Name";
			constexpr sakura::string_view freqTitle = "Shader Frequency";
			constexpr sakura::string_view entryTitle = "Entry Point";
			constexpr size_t widthLeft
				= sakura::max(pathTitle.size(), freqTitle.size(), entryTitle.size());
			size_t widthRight= sakura::max(
					desc.name.size(), desc.entry.size(),
					ShaderFrequencyNameLut.find(desc.frequency)->second.size()
			);
			return fmt::format_to(
				ctx.out(),
				"***{4:*^{2}}***\n"
				"* {5: <{0}}   {6: >{1}} *\n"
				"* {7: <{0}}   {8: >{1}} *\n"
				"* {9: <{0}}   {10: >{1}} *\n"
				"***{3:*^{2}}***\n",
				widthLeft, widthRight, widthRight + widthLeft + 1,
				"", //3 : none
				" ShaderDesc ",//4 : TotalTitle
				pathTitle, desc.name, //5, 6
				freqTitle, desc.frequency,//7, 8
				entryTitle, desc.entry//9, 10
			);
		}
	};

	template<> struct formatter<sakura::graphics::TextureDesc> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::TextureDesc desc, FormatContext& ctx)
		{
			constexpr sakura::string_view widthTitle = "Texture Width";
			constexpr sakura::string_view heightTitle = "Texture Height";
			constexpr sakura::string_view depthTitle = "Texture Depth";
			constexpr int widthLeft = sakura::max(widthTitle.size(), heightTitle.size(), depthTitle.size());
			int widthRight = 30;
			return fmt::format_to(
				ctx.out(),
				"***{4:*^{2}}***\n"
				"* {5: <{0}}   {6: >{1}} *\n"
				"* {7: <{0}}   {8: >{1}} *\n"
				"* {9: <{0}}   {10: >{1}} *\n"
				"***{3:*^{2}}***\n",
				widthLeft, widthRight, widthRight + widthLeft + 1,
				"", //3 : none
				" Texture Desc ",//4 : TotalTitle
				widthTitle, desc.size.width,//5, 6
				heightTitle, desc.size.height, //7,8
				depthTitle, desc.size.depth //9,10
			);
		}
	};
	//RenderAttachmentDesc
	template<> struct formatter<sakura::graphics::Attachment::Slot> : fmt::formatter<sakura::graphics::TextureDesc>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::Attachment::Slot desc, FormatContext& ctx)
		{
			constexpr sakura::string_view widthTitle = " Attachment Desc ";
			constexpr int widthLeft = sakura::max(widthTitle.size(), size_t(30));
			int widthRight = 30;
			return fmt::format_to(
				ctx.out(),
				"***{4:*^{2}}***\n"
				"{5: ^{2}}\n",
				widthLeft, widthRight, widthRight + widthLeft + 1,
				"", //3 : none
				widthTitle, 0//4: RTTitle, 5: TexDesc
			);
		}
	};
	template<> struct formatter<sakura::graphics::Attachment> : fmt::formatter<sakura::graphics::TextureDesc>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::Attachment desc, FormatContext& ctx)
		{
			constexpr sakura::string_view totalTitle = " Attachment Desc ";
			constexpr size_t widthLeft = sakura::max(size_t(0), size_t(10));
			size_t widthRight = 10;
			return fmt::format_to(
				ctx.out(),
				"***{4:*^{2}}***\n"
				,
				widthLeft, widthRight, widthRight + widthLeft + 1,
				"", //3 : none
				totalTitle//4: RTTitle
			);
		}
	};

}