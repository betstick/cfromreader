#pragma once
#include "stdafx.hpp"

//common structus for MSB format

namespace cfr
{
	enum ShapeType
	{
		ShapeTypeNone = -1,
		ShapeTypePoint = 0,
		ShapeTypeCircle = 1,
		ShapeTypeSphere = 2,
		ShapeTypeCylinder = 3,
		ShapeTypeRect = 4,
		ShapeTypeBox = 5,
		ShapeTypeComposite = 6
	};

	struct MSB_Header
	{
		char magic[4];
		int32_t unk04;
		int32_t headerSize;
		char isBigEndian;
		char isBitBigEndian;
		char textEncoding;
		char is64BitOffset;

		void(*MSB_ShapeRect)(MSB_Header* temp, FILE* src);
	};

	void initMsbStruct(MSB_Header* temp, FILE* src)
	{
		fread(&temp->magic,16,1,src);
	};

	struct MSB_Param
	{
		int32_t version;

		//if varint_long false, reverse order!
		int32_t count;
		int64_t nameOffset;

		//quad if varint_long, otherwise int
		int64_t* offsets; //[count -1]
		int64_t nextParamOffset;
	};

	//TODO struct for this

	struct MSB_ShapeCircle
	{
		_Float32 radius;

		void(*MSB_ShapeRect)(MSB_ShapeCircle* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeCircle* temp, FILE* src)
	{
		fread(&temp->radius,4,1,src);
	};

	struct MSB_ShapeShpere
	{
		_Float32 radius;

		void(*MSB_ShapeRect)(MSB_ShapeShpere* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeShpere* temp, FILE* src)
	{
		fread(&temp->radius,4,1,src);
	};

	struct MSB_ShapeCylinder
	{
		_Float32 radius;
		_Float32 height;

		void(*MSB_ShapeRect)(MSB_ShapeCylinder* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeCylinder* temp, FILE* src)
	{
		fread(&temp->radius,4,2,src);
	};

	struct MSB_ShapeRect
	{
		_Float32 width;
		_Float32 height;

		void(*MSB_ShapeRect)(MSB_ShapeRect* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeRect* temp, FILE* src)
	{
		fread(&temp->width,4,2,src);
	};

	struct MSB_ShapeBox
	{
		_Float32 width;
		_Float32 depth;
		_Float32 height;

		void(*MSB_ShapeBox)(MSB_ShapeBox* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeBox* temp, FILE* src)
	{
		fread(&temp->width,4,3,src);
	};

	struct MSB_ShapeComposite
	{
		//each "pair" must be read sequentially
		int32_t pointIndex[8];
		int32_t unk04[8];

		void(*MSB_ShapeComposite)(MSB_ShapeComposite* temp, FILE* src);
	};

	void initMsbStruct(MSB_ShapeComposite* temp, FILE* src)
	{
		for(int i = 0; i < 8; i++)
		{
			fread(&temp->pointIndex[i],4,1,src);
			fread(&temp->unk04[i],4,1,src);
		}
	}
};