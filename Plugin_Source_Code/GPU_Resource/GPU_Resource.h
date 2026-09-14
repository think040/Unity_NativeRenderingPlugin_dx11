#pragma once
#include "../Header.h"
#include "../Gpu/Gpu.h"

using namespace DirectX;

namespace GPU_Resource
{
	struct Resource
	{
		static void DeleteData(void** data)
		{
			if (*data != nullptr)
			{
				delete[] *data;
				*data = nullptr;
			}
		}

		static void ReleaseResource(ID3D11Resource** buffer)
		{
			if (*buffer != nullptr)
			{
				(*buffer)->Release();
				*buffer = nullptr;
			}
		}

		static void ReleaseView(ID3D11View** view)
		{
			if (*view != nullptr)
			{
				(*view)->Release();
				*view = nullptr;
			}
		}

		//static UINT64 Alignment(UINT64 input, UINT64 unit)
		//{
		//	UINT64 align = unit - 1;
		//	return ((input + align) & ~align);
		//}

		static UINT64 Alignment(UINT64 input, UINT64 unit)
		{
			return ((input + (unit - 1)) / unit) * unit;
		}
	};
	
	struct Buffer
	{
		void* data = nullptr;
		UINT count;
		ID3D11Resource* value = nullptr;
		//ID3D11Buffer* value = nullptr;
		
		UINT stride;
		UINT64 size;

		UINT bindFlags;

		ID3D11Resource* rbBuffer = nullptr;
		ID3D11Resource* udBuffer = nullptr;

		ID3D11View* srv = nullptr;
		ID3D11View* uav = nullptr;

		//ID3D11ShaderResourceView* srv = nullptr;
		//ID3D11UnorderedAccessView* uav = nullptr;

		bool useSRV = false;
		bool useUpload = false;

		bool useUAV = false;
		bool useReadback = false;

		bool useCBV = false;

		void CreateBuffer_Upload()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			
			{
				D3D11_BUFFER_DESC bd;
				memset(&bd, 0, sizeof(bd));				

				bd.Usage = D3D11_USAGE_STAGING;
				bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				

				if (bindFlags & (D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
				//if (bindFlags & (D3D11_BIND_INDEX_BUFFER))
				{
					bd.BindFlags = 0;
					bd.MiscFlags = 0;

					bd.StructureByteStride = 0;
					bd.ByteWidth = stride * count;
				}
				else if (bindFlags & D3D11_BIND_CONSTANT_BUFFER)
				{
					bd.BindFlags = 0;
					bd.MiscFlags = 0;

					bd.StructureByteStride = 0;
					bd.ByteWidth = Resource::Alignment(stride * count, 16);
				}
				else
				{
					bd.BindFlags = 0;
					bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

					bd.StructureByteStride = Resource::Alignment(stride, 4);
					bd.ByteWidth = bd.StructureByteStride * count;
				}
							
				hr = device->CreateBuffer(&bd, nullptr, (ID3D11Buffer**)&udBuffer);

				int a = 0;
			}
		}

		void CreateBuffer_Default()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_BUFFER_DESC bd;
				memset(&bd, 0, sizeof(bd));

				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.CPUAccessFlags = 0;

				if (bindFlags & (D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
				//if (bindFlags & (D3D11_BIND_INDEX_BUFFER))
				{
					bd.BindFlags = bindFlags;
					bd.MiscFlags = 0;
				
					bd.StructureByteStride = 0;
					bd.ByteWidth = stride * count;
				}				
				else if (bindFlags & D3D11_BIND_CONSTANT_BUFFER)
				{
					bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					bd.MiscFlags = 0;

					bd.StructureByteStride = 0;
					bd.ByteWidth = Resource::Alignment(stride * count, 16);
				}
				else
				{
					//bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
					bd.BindFlags = bindFlags;
					bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

					bd.StructureByteStride = Resource::Alignment(stride, 4);;
					bd.ByteWidth = bd.StructureByteStride * count;
				}				
				

				hr = device->CreateBuffer(&bd, nullptr, (ID3D11Buffer**)&value);

				int a = 0;
			}

		}

		void CreateBuffer_Readback()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_BUFFER_DESC bd;
				memset(&bd, 0, sizeof(bd));

				bd.Usage = D3D11_USAGE_STAGING;
				bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				
				if (bindFlags & (D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
				//if (bindFlags & (D3D11_BIND_INDEX_BUFFER))
				{
					bd.BindFlags = 0;
					bd.MiscFlags = 0;

					bd.StructureByteStride = 0;
					bd.ByteWidth = stride * count;
				}
				else if (bindFlags & D3D11_BIND_CONSTANT_BUFFER)
				{
					bd.BindFlags = 0;
					bd.MiscFlags = 0;

					bd.StructureByteStride = 0;
					bd.ByteWidth = Resource::Alignment(stride * count, 16);
				}
				else
				{
					bd.BindFlags = 0;
					bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

					bd.StructureByteStride = Resource::Alignment(stride, 4);;
					bd.ByteWidth = bd.StructureByteStride * count;
				}						

				hr = device->CreateBuffer(&bd, nullptr, (ID3D11Buffer**)&rbBuffer);

				int a = 0;
			}
		}

		void CreateSRV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = 0;				
				desc.Buffer.NumElements = count;

				hr = device->CreateShaderResourceView(value, &desc, (ID3D11ShaderResourceView**)&srv);
			}
		};

		void CreateUAV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = 0;
				desc.Buffer.NumElements = count;
				desc.Buffer.Flags = 0;

				hr = device->CreateUnorderedAccessView(value, &desc, (ID3D11UnorderedAccessView**)&uav);
			}
		};

		Buffer(
			UINT stride, int count, UINT bindFlags = 0x0L,
			bool useReadback = false, bool useUAV = false, bool useUpload = true, bool useSRV = true, bool useCBV = false)
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			
			this->count = count;

			this->stride = stride;			

			this->size = this->stride * count;

			this->useSRV = useSRV;
			this->useUpload = useUpload;

			this->useUAV = useUAV;
			this->useReadback = useReadback;

			this->useCBV = useCBV;

			this->bindFlags = bindFlags;

			this->bindFlags |= useSRV ? D3D11_BIND_SHADER_RESOURCE : bindFlags;
			this->bindFlags |= useUAV ? D3D11_BIND_UNORDERED_ACCESS : bindFlags;
			this->bindFlags |= useCBV ? D3D11_BIND_CONSTANT_BUFFER : bindFlags;

			{
				this->data = malloc(size);
				memset(data, 0, size);
			}

			if (useUpload)
			{
				CreateBuffer_Upload();
			}

			{
				CreateBuffer_Default();
			}

			if (useReadback)
			{
				CreateBuffer_Readback();
			}


			if (useSRV)
			{
				CreateSRV();
			}

			if (useUAV)
			{
				CreateUAV();
			}			

		}

		~Buffer()
		{
			Resource::DeleteData(&data);

			Resource::ReleaseResource(&value);
			Resource::ReleaseResource(&udBuffer);
			Resource::ReleaseResource(&rbBuffer);

			Resource::ReleaseView(&srv);
			Resource::ReleaseView(&uav);
		}

		void DeleteData()
		{
			if (data != nullptr)
			{
				delete[] data;
				data = nullptr;
			}
		}

		void* ptrData(int x)
		{
			return (std::byte*)data + x * stride;
		}

		void SetData(int x, void* value)
		{
			memcpy((std::byte*)data + x * stride, (std::byte*)value, stride);
		}

		void GetData(int x, void* value)
		{
			memcpy((std::byte*)value, (std::byte*)data + x * stride, stride);
		}

		void WriteToBuffer(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::iContext;
			ID3D11Resource* value = udBuffer;

			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(value, 0, D3D11_MAP_WRITE, 0, &map);				
				if (hr == S_OK)
				{
					memcpy(map.pData, data, size);
					context->Unmap(value, 0);
				}			
			}
		}

		void CopyToBuffer(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(value, udBuffer);
				//context->CopySubresourceRegion()
			}
		}

		void CopyFromBuffer(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(rbBuffer, value);
				//context->CopySubresourceRegion()
			}
		}

		void ReadFromBuffer(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::iContext;
			ID3D11Resource* value = rbBuffer;

			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					memcpy(data, map.pData, size);
					context->Unmap(value, 0);
				}
			}
		}


		//Test
		void ReadFromBufferWait(ID3D11DeviceContext* context, ID3D11Query* fence)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::iContext;
			ID3D11Resource* value = rbBuffer;

			{
				D3D11_MAPPED_SUBRESOURCE map;
				//hr = context->Map(value, 0, D3D11_MAP_READ, 0, &map);
				context->End(fence);

				while (context->GetData(fence, nullptr, 0, 0) == S_FALSE)
				{
					// CPU 점유율 폭발을 막기 위해 잠시 대기
					//Sleep(0);				
				}

				while (context->Map(value, 0, D3D11_MAP_READ, 0, &map) == S_FALSE)
				{
					// CPU 점유율 폭발을 막기 위해 잠시 대기
					//Sleep(0);				
				}

				//if (hr == S_OK)
				{
					memcpy(data, map.pData, size);
					context->Unmap(value, 0);
				}
			}
		}

	};

	template<typename T>
	struct ROBuffer : public Buffer
	{
		ROBuffer(int count, UINT bindFlags = 0x0L)
			: Buffer(sizeof(T), count, bindFlags, false, false, true, true, false)
		{

		};
	};

	template<typename T>
	struct RWBuffer : public Buffer
	{		
		RWBuffer(int count, UINT bindFlags = 0x0L, bool useUpload = true, bool useSRV = true)
			: Buffer(sizeof(T), count, bindFlags, true, true, useUpload, useSRV, false)
		{

		};		
	};

	template<typename T>
	struct COBuffer : public Buffer
	{
		COBuffer()
			: Buffer(sizeof(T), 1, D3D11_BIND_CONSTANT_BUFFER, false, false, true, false, true)
		{

		};
	};


	struct Texture
	{
		void* data = nullptr;

		DXGI_FORMAT format;

		ID3D11Resource* value = nullptr;
		
		UINT64 count;
		UINT stride;	

		UINT bindFlags;

		ID3D11Resource* rbTex = nullptr;
		ID3D11Resource* udTex = nullptr;

		bool useSRV = false;
		bool useUpload = false;

		bool useUAV = false;
		bool useReadback = false;

		ID3D11View* srv = nullptr;
		ID3D11View* uav = nullptr;

		Texture()
		{

		}

		virtual ~Texture()
		{
			Resource::DeleteData(&data);

			Resource::ReleaseResource(&value);
			Resource::ReleaseResource(&udTex);
			Resource::ReleaseResource(&rbTex);

			Resource::ReleaseView(&srv);
			Resource::ReleaseView(&uav);
		}

		UINT64 Get_RowPitch_Readback(UINT64 input)
		{
			const int unit = 8;

			UINT64 output = ((input + (unit - 1)) / unit) * unit;

			return output;
		}
		
	};

	struct Texture3D : public Texture
	{
		int width = 0;
		int height = 0;
		int depth = 0;
		
		UINT64 rowPitch;
		UINT64 slicePitch;
		UINT64 size;

		//UINT64 rowPitch_readback;
		//UINT64 slicePitch_readback;
		//UINT64 size_readback;

		void CreateBuffer_Upload()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE3D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.Depth = depth;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				td.MiscFlags = 0;

			

				hr = device->CreateTexture3D(&td, nullptr, (ID3D11Texture3D**)&udTex);

				int a = 0;
			}
		}

		void CreateTex3D_Default()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_TEXTURE3D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.Depth = depth;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_DEFAULT;
				//td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | bindFlags;
				td.BindFlags = bindFlags;
				td.CPUAccessFlags = 0;
				td.MiscFlags = 0;

				hr = device->CreateTexture3D(&td, nullptr, (ID3D11Texture3D**)&value);

				int a = 0;
			}
		}

		void CreateBuffer_Readback()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE3D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.Depth = depth;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				td.MiscFlags = 0;

				hr = device->CreateTexture3D(&td, nullptr, (ID3D11Texture3D**)&rbTex);

				int a = 0;
			}
		}

		void CreateSRV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = 1;
				desc.Texture3D.MostDetailedMip = 0;

				hr = device->CreateShaderResourceView(__super::value, &desc, (ID3D11ShaderResourceView**)&srv);

				int a = 0;
			}
		};

		void CreateUAV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.FirstWSlice = 0;
				desc.Texture3D.MipSlice = 0;
				desc.Texture3D.WSize = depth;

				hr = device->CreateUnorderedAccessView(__super::value, &desc, (ID3D11UnorderedAccessView**)&uav);

				int a = 0;
			}
		};

		Texture3D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT depth, UINT bindFlags = 0x0L,
			bool useReadback = false, bool useUAV = false, bool useUpload = true, bool useSRV = true)
		{			
			this->format = format;

			this->stride = stride;
			this->width = width;
			this->height = height;
			this->depth = depth;

			this->rowPitch = stride * width;
			this->slicePitch = rowPitch * height;
			this->size = slicePitch * depth;

			this->bindFlags = bindFlags;
						
			this->useReadback = useReadback;
			this->useUAV = useUAV;
			this->useUpload = useUpload;
			this->useSRV = useSRV;

			this->bindFlags |= useSRV ? D3D11_BIND_SHADER_RESOURCE : bindFlags;
			this->bindFlags |= useUAV ? D3D11_BIND_UNORDERED_ACCESS : bindFlags;

			this->count = width * height * depth;
			{
				this->data = malloc(size);
				memset(data, 0, size);
			}

			if (useUpload)
			{
				CreateBuffer_Upload();
			}

			{
				CreateTex3D_Default();
			}

			if (useReadback)
			{
				CreateBuffer_Readback();
			}


			if (useSRV)
			{
				CreateSRV();
			}

			if (useUAV)
			{
				CreateUAV();
			}			
		}

		virtual ~Texture3D()
		{
			
		}


		void* ptrData(int z, int y, int x)
		{
			return (std::byte*)data + z * slicePitch + y * rowPitch + x * stride;
		}

		void SetData(int z, int y, int x, void* value)
		{
			memcpy((std::byte*)data + z * slicePitch + y * rowPitch + x * stride, (std::byte*)value, stride);
		}

		void GetData(int z, int y, int x, void* value)
		{
			memcpy((std::byte*)value, (std::byte*)data + z * slicePitch + y * rowPitch + x * stride, stride);
		}


		void WriteToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);
				if (hr == S_OK)
				{					
					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;
					
					UINT64 slicePitch_dst = map.DepthPitch;
					UINT64 slicePitch_src = slicePitch;

					BYTE* dst = (BYTE*)map.pData;
					BYTE* src = (BYTE*)data;
					size_t rowSize = rowPitch;					

					for (int i = 0; i < depth; i++)
					{
						UINT offset_dst = i * slicePitch_dst;
						UINT offset_src = i * slicePitch_src;
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

		void CopyToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(value, udTex);				
			}
		}

		void CopyFromTex(ID3D11DeviceContext* context)
		{

			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(rbTex, value);				
			}
		}

		void ReadFromTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;
			
			if (data != nullptr)
			{				
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;					

					UINT64 slicePitch_dst = slicePitch;
					UINT64 slicePitch_src = map.DepthPitch;
					
					BYTE* dst = (BYTE*)data;
					BYTE* src = (BYTE*)map.pData;
					size_t rowSize = rowPitch;

					for (int i = 0; i < depth; i++)
					{
						UINT offset_dst = i * slicePitch_dst;
						UINT offset_src = i * slicePitch_src;						
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;							
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;							
						}
					}
			
					context->Unmap(_value, 0);
				}
			}			
		}


		void WriteToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);
				
				if (hr == S_OK)
				{
										
					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;
										
					UINT64 slicePitch_dst = map.DepthPitch;
					UINT64 slicePitch_src = slicePitch;

					BYTE* dst = (BYTE*)map.pData + (pos0[2] * slicePitch_dst + pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)data		 + (pos1[2] * slicePitch_src + pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;

					for (int i = 0; i < count[2]; i++)
					{
						UINT offset_dst = i * slicePitch_dst;
						UINT offset_src = i * slicePitch_src;
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}

		}

		void CopyToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{				
				auto dst = value;
				auto src = udTex;
				
				D3D11_BOX box = {
					pos1[0],			pos1[1],			pos1[2],
					pos1[0] + count[0],	pos1[1] + count[1],	pos1[2] + count[2] };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void CopyFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				auto dst = rbTex;
				auto src = value;

				D3D11_BOX box = {
					pos1[0],			pos1[1],			pos1[2],
					pos1[0] + count[0],	pos1[1] + count[1],	pos1[2] + count[2] };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void ReadFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);				
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;
					
					UINT64 slicePitch_dst = slicePitch;
					UINT64 slicePitch_src = map.DepthPitch;
					
					BYTE* dst = (BYTE*)data      + (pos0[2] * slicePitch_dst + pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)map.pData + (pos1[2] * slicePitch_src + pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;

					for (int i = 0; i < count[2]; i++)
					{
						UINT offset_dst = i * slicePitch_dst;
						UINT offset_src = i * slicePitch_src;						
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;							
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;							
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

	};

	struct ROTexture3D : public Texture3D
	{
		ROTexture3D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT depth, UINT bindFlags)
			: Texture3D(format, stride, width, height, depth, bindFlags, false, false, true, true)
		{

		};

	};

	struct RWTexture3D : public Texture3D
	{		
		RWTexture3D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT depth, UINT bindFlags, bool useUpload = true, bool useSRV = true)
			: Texture3D(format, stride, width, height, depth, bindFlags, true, true, useUpload, useSRV)
		{

		};
	};


	struct Texture2DArray : public Texture
	{
		int width = 0;
		int height = 0;
		int arrayCount = 0;

		UINT64 rowPitch;
		UINT64 arrayPitch;
		UINT64 size;

		bool useMsaa;

		bool useRTV;
		bool useDSV;

		ID3D11View* rtv = nullptr;
		ID3D11View* dsv = nullptr;

		bool useCube;
		UINT cubeCount;

		//UINT64 rowPitch_readback;
		//UINT64 slicePitch_readback;
		//UINT64 size_readback;

		void CreateBuffer_Upload()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = arrayCount;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				td.MiscFlags = 0;

				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&udTex);

				int a = 0;
			}
		}

		void CreateTex2DArray_Default()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = arrayCount;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_DEFAULT;				
				td.BindFlags = bindFlags;
				td.CPUAccessFlags = 0;
				td.MiscFlags = 0;

				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				if (useRTV || useDSV)
				{
					td.MipLevels = 1;
				}

				if (useMsaa)
				{
					td.SampleDesc.Count = Gpu::msaaCount;
					td.SampleDesc.Quality = Gpu::m4xMsaaQuality - 1;
				}

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&value);

				int a = 0;
			}
		}

		void CreateBuffer_Readback()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = arrayCount;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				td.MiscFlags = 0;

				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&rbTex);

				int a = 0;
			}
		}

		void CreateSRV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;				
				
				if (useCube)
				{
					if(cubeCount > 1)
					{
						desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
						desc.TextureCubeArray.NumCubes = cubeCount;
						desc.TextureCubeArray.First2DArrayFace = 0;
						desc.TextureCubeArray.MipLevels = -1;
						desc.TextureCubeArray.MostDetailedMip = 0;
					}
					else
					{
						desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
						desc.TextureCube.MipLevels = -1;
						desc.TextureCube.MostDetailedMip = 0;
					}
				}
				else
				{
					if (useMsaa)
					{
						desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
						desc.Texture2DMSArray.ArraySize = arrayCount;
						desc.Texture2DMSArray.FirstArraySlice = 0;
					}
					else
					{
						desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
						desc.Texture2DArray.ArraySize = arrayCount;
						desc.Texture2DArray.FirstArraySlice = 0;
						desc.Texture2DArray.MipLevels = 1;
						desc.Texture2DArray.MostDetailedMip = 0;
					}
				}								

				hr = device->CreateShaderResourceView(__super::value, &desc, (ID3D11ShaderResourceView**)&srv);

				int a = 0;
			}
		};

		void CreateUAV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.ArraySize = arrayCount;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice = 0;				

				hr = device->CreateUnorderedAccessView(__super::value, &desc, (ID3D11UnorderedAccessView**)&uav);

				int a = 0;
			}
		};

		void CreateRTV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			
			{
				D3D11_RENDER_TARGET_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));				
				desc.Format = format;
				desc.ViewDimension = useMsaa ? D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.ArraySize = arrayCount;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice = 0;

				hr = device->CreateRenderTargetView(__super::value, &desc, (ID3D11RenderTargetView**) &rtv);
			}
		};

		void CreateDSV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = useMsaa ? D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.ArraySize = arrayCount;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice = 0;

				hr = device->CreateDepthStencilView(__super::value, &desc, (ID3D11DepthStencilView**)&dsv);
			}
		};

		Texture2DArray(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT arrayCount, UINT bindFlags = 0x0L, 			
			bool useReadback = false, bool useUAV = false, bool useUpload = true, bool useSRV = true, 
			bool useMsaa = false, bool useRTV = false, bool useDSV = false,
			bool useCube = false)
		{
			this->format = format;

			this->stride = stride;
			this->width = width;
			this->height = height;
			this->arrayCount = arrayCount;

			this->rowPitch = stride * width;
			this->arrayPitch = rowPitch * height;
			this->size = arrayPitch * arrayCount;

			this->useMsaa = useMsaa;
			this->useRTV = useRTV;
			this->useDSV = useDSV;

			this->bindFlags = bindFlags;

			this->useReadback = useReadback;
			this->useUAV = useUAV;
			this->useUpload = useUpload;
			this->useSRV = useSRV;

			this->bindFlags |= useSRV ? D3D11_BIND_SHADER_RESOURCE : bindFlags;
			this->bindFlags |= useUAV ? D3D11_BIND_UNORDERED_ACCESS : bindFlags;

			this->bindFlags |= useRTV ? D3D11_BIND_RENDER_TARGET : bindFlags;
			this->bindFlags |= useDSV ? D3D11_BIND_DEPTH_STENCIL : bindFlags;

			this->count = width * height * arrayCount;

			this->useCube = useCube;
			this->cubeCount = arrayCount / 6;
			{
				this->data = malloc(size);
				memset(data, 0, size);
			}

			if (useUpload)
			{
				CreateBuffer_Upload();
			}

			{
				CreateTex2DArray_Default();
			}

			if (useReadback)
			{
				CreateBuffer_Readback();
			}


			if (useSRV)
			{
				CreateSRV();
			}

			if (useUAV)
			{
				CreateUAV();
			}

			if (useRTV)
			{
				CreateRTV();
			}

			if (useDSV)
			{
				CreateDSV();
			}
		}

		virtual ~Texture2DArray()
		{
			Resource::ReleaseView(&rtv);
			Resource::ReleaseView(&dsv);
		}
		


		void* ptrData(int z, int y, int x)
		{
			return (std::byte*)data + z * arrayPitch + y * rowPitch + x * stride;
		}

		void SetData(int z, int y, int x, void* value)
		{
			memcpy((std::byte*)data + z * arrayPitch + y * rowPitch + x * stride, (std::byte*)value, stride);
		}

		void GetData(int z, int y, int x, void* value)
		{
			memcpy((std::byte*)value, (std::byte*)data + z * arrayPitch + y * rowPitch + x * stride, stride);
		}


		void WriteToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;

					UINT64 arrayPitch_dst = map.DepthPitch;
					UINT64 arrayPitch_src = arrayPitch;

					BYTE* dst = (BYTE*)map.pData;
					BYTE* src = (BYTE*)data;
					size_t rowSize = rowPitch;

					for (int i = 0; i < arrayCount; i++)
					{
						UINT offset_dst = i * arrayPitch_dst;
						UINT offset_src = i * arrayPitch_src;
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

		void CopyToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(value, udTex);
			}
		}

		void CopyFromTex(ID3D11DeviceContext* context)
		{

			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(rbTex, value);
			}
		}

		void ReadFromTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;

					UINT64 arrayPitch_dst = arrayPitch;
					UINT64 arrayPitch_src = map.DepthPitch;

					BYTE* dst = (BYTE*)data;
					BYTE* src = (BYTE*)map.pData;
					size_t rowSize = rowPitch;

					for (int i = 0; i < arrayCount; i++)
					{
						UINT offset_dst = i * arrayPitch_dst;
						UINT offset_src = i * arrayPitch_src;
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}


		void WriteToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);

				if (hr == S_OK)
				{

					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;

					UINT64 arrayPitch_dst = map.DepthPitch;
					UINT64 arrayPitch_src = arrayPitch;

					BYTE* dst = (BYTE*)map.pData	+ (pos0[2] * arrayPitch_dst + pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)data			+ (pos1[2] * arrayPitch_src + pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;

					for (int i = 0; i < count[2]; i++)
					{
						UINT offset_dst = i * arrayPitch_dst;
						UINT offset_src = i * arrayPitch_src;
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}

		}

		void CopyToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				auto dst = value;
				auto src = udTex;

				D3D11_BOX box = {
					pos1[0],			pos1[1],			pos1[2],
					pos1[0] + count[0],	pos1[1] + count[1],	pos1[2] + count[2] };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void CopyFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				auto dst = rbTex;
				auto src = value;

				D3D11_BOX box = {
					pos1[0],			pos1[1],			pos1[2],
					pos1[0] + count[0],	pos1[1] + count[1],	pos1[2] + count[2] };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void ReadFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;

					UINT64 arrayPitch_dst = arrayPitch;
					UINT64 arrayPitch_src = map.DepthPitch;

					BYTE* dst = (BYTE*)data			+ (pos0[2] * arrayPitch_dst + pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)map.pData	+ (pos1[2] * arrayPitch_src + pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;

					for (int i = 0; i < count[2]; i++)
					{
						UINT offset_dst = i * arrayPitch_dst;
						UINT offset_src = i * arrayPitch_src;
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

	};

	struct ROTexture2DArray : public Texture2DArray
	{
		ROTexture2DArray(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT arrayCount, UINT bindFlags,
			bool useUpload = true, bool useSRV = true, 
			bool useMsaa = false, bool useRTV = false, bool useDSV = false,
			bool useCube = false
			)
			: Texture2DArray(format, stride, width, height, arrayCount, bindFlags,  
				false, false, 
				useUpload, useSRV,
				useMsaa, useRTV, useDSV,
				useCube)
		{

		};

	};

	struct RWTexture2DArray : public Texture2DArray
	{
		RWTexture2DArray(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT arrayCount, UINT bindFlags,
			bool useReadback = true, bool useUAV = true,
			bool useUpload = true, bool useSRV = true)
			: Texture2DArray(format, stride, width, height, arrayCount, bindFlags,
				useReadback, useUAV,
				useUpload, useSRV,
				false, false, false, 
				false)
		{

		};

	};


	struct Texture2D : public Texture
	{
		int width = 0;
		int height = 0;
		//int arrayCount = 0;

		UINT64 rowPitch;
		//UINT64 arrayPitch;
		UINT64 size;

		bool useMsaa;

		bool useRTV;
		bool useDSV;

		ID3D11View* rtv = nullptr;
		ID3D11View* dsv = nullptr;

		//UINT64 rowPitch_readback;
		//UINT64 slicePitch_readback;
		//UINT64 size_readback;

		void CreateBuffer_Upload()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = 1;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				td.MiscFlags = 0;

				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&udTex);

				int a = 0;
			}
		}

		void CreateTex2D_Default()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = 1;
				//td.MipLevels = 0;		
				td.Format = format;
				td.Usage = D3D11_USAGE_DEFAULT;
				td.BindFlags = bindFlags;
				td.CPUAccessFlags = 0;
				td.MiscFlags = 0;

				td.MipLevels = 1;
				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				if (useRTV || useDSV)
				{
					td.MipLevels = 1;
				}

				if (useMsaa)
				{
					td.SampleDesc.Count = Gpu::msaaCount;
					td.SampleDesc.Quality = Gpu::m4xMsaaQuality - 1;
				}

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&value);

				int a = 0;
			}
		}

		void CreateBuffer_Readback()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_TEXTURE2D_DESC td;
				memset(&td, 0, sizeof(td));
				td.Width = width;
				td.Height = height;
				td.ArraySize = 1;
				//td.MipLevels = 0;
				td.MipLevels = 1;
				td.Format = format;
				td.Usage = D3D11_USAGE_STAGING;
				td.BindFlags = 0;
				td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				td.MiscFlags = 0;

				td.SampleDesc.Count = 1;
				td.SampleDesc.Quality = 0;

				hr = device->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&rbTex);

				int a = 0;
			}
		}

		void CreateSRV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = useMsaa ? D3D11_SRV_DIMENSION_TEXTURE2DMS: D3D11_SRV_DIMENSION_TEXTURE2D;

				if (useMsaa)
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;					
				}
				else
				{					
					desc.Texture2D.MipLevels = 1;
					desc.Texture2D.MostDetailedMip = 0;
				}

				hr = device->CreateShaderResourceView(__super::value, &desc, (ID3D11ShaderResourceView**)&srv);

				int a = 0;
			}
		};

		void CreateUAV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;				
				desc.Texture2D.MipSlice = 0;

				hr = device->CreateUnorderedAccessView(__super::value, &desc, (ID3D11UnorderedAccessView**)&uav);

				int a = 0;
			}
		};

		void CreateRTV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_RENDER_TARGET_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = useMsaa ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

				if (useMsaa)
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;				
				}
				else
				{
					desc.Texture2D.MipSlice = 0;
				}
				
				hr = device->CreateRenderTargetView(__super::value, &desc, (ID3D11RenderTargetView**)&rtv);
			}
		};

		void CreateDSV()
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;

			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				memset(&desc, 0, sizeof(desc));
				desc.Format = format;
				desc.ViewDimension = useMsaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;

				if (useMsaa)
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;
				}
				else
				{
					desc.Texture2D.MipSlice = 0;
				}

				hr = device->CreateDepthStencilView(__super::value, &desc, (ID3D11DepthStencilView**)&dsv);
			}
		};

		Texture2D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT bindFlags = 0x0L,
			bool useReadback = false, bool useUAV = false, 
			bool useUpload = true, bool useSRV = true,
			bool useMsaa = false, bool useRTV = false, bool useDSV = false)
		{
			this->format = format;

			this->stride = stride;
			this->width = width;
			this->height = height;			

			this->rowPitch = stride * width;			
			this->size = rowPitch * height;

			this->useMsaa = useMsaa;
			this->useRTV = useRTV;
			this->useDSV = useDSV;

			this->bindFlags = bindFlags;

			this->useReadback = useReadback;
			this->useUAV = useUAV;
			this->useUpload = useUpload;
			this->useSRV = useSRV;

			this->bindFlags |= useSRV ? D3D11_BIND_SHADER_RESOURCE : bindFlags;
			this->bindFlags |= useUAV ? D3D11_BIND_UNORDERED_ACCESS : bindFlags;

			this->bindFlags |= useRTV ? D3D11_BIND_RENDER_TARGET : bindFlags;
			this->bindFlags |= useDSV ? D3D11_BIND_DEPTH_STENCIL : bindFlags;

			this->count = width * height;
			{
				this->data = malloc(size);
				memset(data, 0, size);
			}

			if (useUpload)
			{
				CreateBuffer_Upload();
			}

			{
				CreateTex2D_Default();
			}

			if (useReadback)
			{
				CreateBuffer_Readback();
			}


			if (useSRV)
			{
				CreateSRV();
			}

			if (useUAV)
			{
				CreateUAV();
			}

			if (useRTV)
			{
				CreateRTV();
			}

			if (useDSV)
			{
				CreateDSV();
			}
		}

		Texture2D(wstring path)
		{
			HRESULT hr = S_OK;
			auto device = Gpu::device;
			
			//hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			
			ID3D11ShaderResourceView* srv;
						
			TexMetadata info;
			ScratchImage* image = new ScratchImage();

			hr = LoadFromWICFile(path.c_str(),
				WIC_FLAGS_ALL_FRAMES, &info, *image);
		
			hr = CreateShaderResourceView(device,
				image->GetImages(), image->GetImageCount(),
				image->GetMetadata(), &srv);

			this->srv = srv;
			
			int a = 0;

			//debug
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				srv->GetDesc(&desc);

				a = 0;
			}
			

			
		}

		virtual ~Texture2D()
		{
			Resource::ReleaseView(&rtv);
			Resource::ReleaseView(&dsv);
		}

		void* ptrData(int y, int x)
		{
			return (std::byte*)data + y * rowPitch + x * stride;
		}

		void SetData(int y, int x, void* value)
		{
			memcpy((std::byte*)data + y * rowPitch + x * stride, (std::byte*)value, stride);
		}

		void GetData(int y, int x, void* value)
		{
			memcpy((std::byte*)value, (std::byte*)data + y * rowPitch + x * stride, stride);
		}


		void WriteToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;				

					BYTE* dst = (BYTE*)map.pData;
					BYTE* src = (BYTE*)data;
					size_t rowSize = rowPitch;
				
					{
						UINT offset_dst = 0;
						UINT offset_src = 0;
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

		void CopyToTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(value, udTex);			
			}
		}

		void CopyFromTex(ID3D11DeviceContext* context)
		{

			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				context->CopyResource(rbTex, value);
			}
		}

		void ReadFromTex(ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;
					
					BYTE* dst = (BYTE*)data;
					BYTE* src = (BYTE*)map.pData;
					size_t rowSize = rowPitch;
					
					{
						UINT offset_dst = 0;
						UINT offset_src = 0;
						for (int j = 0; j < height; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}


		void WriteToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = udTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_WRITE, 0, &map);

				if (hr == S_OK)
				{

					UINT64 rowPitch_dst = map.RowPitch;
					UINT64 rowPitch_src = rowPitch;
				
					BYTE* dst = (BYTE*)map.pData + (pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)data      + (pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;
					
					{
						UINT offset_dst = 0;
						UINT offset_src = 0;
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}

		}

		void CopyToTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				auto dst = value;
				auto src = udTex;

				D3D11_BOX box = {
					pos1[0],			pos1[1],			0,
					pos1[0] + count[0],	pos1[1] + count[1],	1 };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void CopyFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			//auto context = Gpu::dContext;

			{
				auto dst = rbTex;
				auto src = value;

				D3D11_BOX box = {
					pos1[0],			pos1[1],			0,
					pos1[0] + count[0],	pos1[1] + count[1],	1 };

				context->CopySubresourceRegion(dst, 0, pos0[0], pos0[1], pos0[2], src, 0, &box);
			}
		}

		void ReadFromTex_Region(UINT* pos0, UINT* pos1, UINT* count, ID3D11DeviceContext* context)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* _value = rbTex;
			//auto context = Gpu::iContext;

			if (data != nullptr)
			{
				D3D11_MAPPED_SUBRESOURCE map;
				hr = context->Map(_value, 0, D3D11_MAP_READ, 0, &map);
				if (hr == S_OK)
				{
					UINT64 rowPitch_dst = rowPitch;
					UINT64 rowPitch_src = map.RowPitch;
				
					BYTE* dst = (BYTE*)data		 + (pos0[1] * rowPitch_dst + pos0[0] * stride);
					BYTE* src = (BYTE*)map.pData + (pos1[1] * rowPitch_src + pos1[0] * stride);
					size_t rowSize = count[0] * stride;
					
					{
						UINT offset_dst = 0;
						UINT offset_src = 0;
						for (int j = 0; j < count[1]; j++)
						{
							BYTE* _dst = dst + offset_dst;
							BYTE* _src = src + offset_src;
							memcpy(_dst, _src, rowSize);

							offset_dst += rowPitch_dst;
							offset_src += rowPitch_src;
						}
					}

					context->Unmap(_value, 0);
				}
			}
		}

	};

	struct ROTexture2D : public Texture2D
	{
		ROTexture2D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT bindFlags,
			bool useUpload = true, bool useSRV = true,
			bool useMsaa = false, bool useRTV = false, bool useDSV = false)		
			: Texture2D(format, stride, width, height, bindFlags,
				false, false,
				useUpload, useSRV,
				useMsaa, useRTV, useDSV)
		{

		};

		ROTexture2D(wstring path) : Texture2D(path)
		{

		};
	};

	struct RWTexture2D: public Texture2D
	{
		RWTexture2D(
			DXGI_FORMAT format, UINT stride, UINT width, UINT height, UINT bindFlags,
			bool useReadback = true, bool useUAV = true,
			bool useUpload = true, bool useSRV = true)
			: Texture2D(format, stride, width, height, bindFlags,
				useReadback, useUAV,
				useUpload, useSRV,
				false, false, false)
		{

		};

	};
}
