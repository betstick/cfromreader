#pragma once
#include "stdafx.hpp"

namespace cfr {
	// SoulsFormats/Formats/FLVER/Vertex.cs
	class FLVER_VertexData
	{
		public:
		Vector3 position;

		FLVER2_VertexBoneWeights boneWeights;
		FLVER2_VertexBoneIndices boneIndices;

		Vector3 normal;
		int32_t normalW;

		Vector3* UVs;

		Vector4* tangents;

		//vec pointing perpendicular to normal and tangent
		Vector4 bitangent; 

		FLVER2_VertexColor* vertexColor;

		//uvQueue;
		//tangentQueue;
		//colorQueue;

		//n is the number of layoutmembers. this REALLY needs to be its own file :/
		//uvfactor is needed, finish this maybe later if this function is even needed
		FLVER_VertexData(BSReader* file, FLVER2_LayoutMember* layout, uint32_t n) //, _Float32 uvFactor)
		{
			for(uint32_t i = 0; i < n; i++)
			{
				if(layout[i].semantic == 0)
				{
					if(layout[i].type == 0x02)
					{
						position = readVec3(file);
					}
					else if(layout[i].type == 0x04)
					{
						position = readVec3(file);
						file->seek(file->readPos+4); //should push it 4 ahead?
					}
					else if(layout[i].type == 0xF0)
					{
						throw std::runtime_error("Edge compressed format not supported!\n");
					}
					else
						throw std::runtime_error("Uknown vertex layout type...\n");
				}
				else if(layout[i].semantic == 1)
				{
					if(layout[i].type == 0x10)
						boneWeights = FLVER2_VertexBoneWeights(file,0,  127.0f);
					else if(layout[i].type == 0x13)
						boneWeights = FLVER2_VertexBoneWeights(file,1,  255.0f);
					else if(layout[i].type == 0x16)
						boneWeights = FLVER2_VertexBoneWeights(file,2,32767.0f);
					else if(layout[i].type == 0x1A)
						boneWeights = FLVER2_VertexBoneWeights(file,2,32767.0f);
					else
						throw std::runtime_error(":fatdog:\n");
				}
				else if(layout[i].semantic == 2)
				{
					if(layout[i].type == 0x11)
						boneIndices = FLVER2_VertexBoneIndices(file,1);
					else if(layout[i].type == 0x18)
						boneIndices = FLVER2_VertexBoneIndices(file,2);
					else if(layout[i].type == 0x2F)
						boneIndices = FLVER2_VertexBoneIndices(file,1);
					else
						throw std::runtime_error(":fatdog:\n");
				}
				else if(layout[i].semantic == 3)
				{
					//NORMALS
					/*if(layout[i].type == 0x02)
					{
						normal = readVec3(file);
					}
					else if(layout[i].type == 0x03)
					{
						normal = readVec3(file);
						_Float32 w;
						file->read(&w,4); 
						normalW = (int32_t)w; //must convert, its stored as float
					}
					else if(layout[i].type == 0x10)
					{
						normal = readByteNormVec3(file);
						file->read(&normalW,1); //confirm this works somehow
					}
					else if(layout[i].type == 0x11)
					{
						normal = readByteNormVec3(file);
						file->read(&normalW,1); //confirm this works somehow
					}
					else if(layout[i].type == 0x12)
					{
						file->read(&normalW,1);
						normal = readByteNormVec3(file);
					}
					else if(layout[i].type == 0x13)
					{
						normal = readByteNormVec3(file);
						file->read(&normalW,1);
					}
					else if(layout[i].type == 0x1A)
					{
						normal = readByteNormVec3(file);
						file->read(&normalW,2); //verify
					}
					else if(layout[i].type == 0x2E)
					{
						normal = readByteNormVec3(file);
						file->read(&normalW,2); //verify
					}*/
				}
			}
		};
	};
};