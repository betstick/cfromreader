#pragma once
#include "stdafx.hpp"

//this messy file is for misc stuff needed by many files. types, functions, etc.

namespace cfr 
{
	//Outputs a string representation of a byte as a number. Debugging only.
	int32_t byteToBinary(int8_t byte)
	{
		int32_t output = 0;
		output += (bool)((byte >> 0) & 0x01) ?        1 : 0;
		output += (bool)((byte >> 1) & 0x01) ?       10 : 0;
		output += (bool)((byte >> 2) & 0x01) ?      100 : 0;
		output += (bool)((byte >> 3) & 0x01) ?     1000 : 0;
		output += (bool)((byte >> 4) & 0x01) ?    10000 : 0;
		output += (bool)((byte >> 5) & 0x01) ?   100000 : 0;
		output += (bool)((byte >> 6) & 0x01) ?  1000000 : 0;
		output += (bool)((byte >> 7) & 0x01) ? 10000000 : 0;

		return output;
	};

	struct Vector2
	{
		_Float32 x;
		_Float32 y;
	};

	struct Vector3
	{
		_Float32 x;
		_Float32 y;
		_Float32 z;
	};

	//this is ugly. make it better :/
	struct OffsetString
	{
		uint32_t offset; //might need to be 64 bit later...
		char string[64]; //confirm if this is too many chars
	};

	//Returns length of null terminated string or errors if there is no end.
	uint64_t getStringSize(BSReader* file)
	{
		file->markPos();
		uint64_t length = 0;
		char temp = 'a';

		//would use for loop, but need to retrun length
		while(temp != '\0')
		{
			file->read(&temp,1);
			length++;
		}

		file->returnToMark(); //return to where started
		return length;
	};

	/*template<typename T> void BSReader <Vector3>::read(Vector3* vecPtr)
	{
		file->read(vecPtr->x,4);
		file->read(vecPtr->y,4);
		file->read(vecPtr->z,4);
	};*/

	OffsetString readOffsetString(BSReader* file)
	{
		OffsetString temp;
		file->read(&temp.offset,4);
		file->stepIn(temp.offset);

		uint64_t length = getStringSize(file);
		file->read(&temp.string,length);
		
		file->stepOut();
		return temp;
	};

	//template<> void BSReader::read<Vector3>(Vector3* vecPtr){};

	Vector3 readVec3(BSReader* reader)
	{
		Vector3 tempVec;
		reader->read(&tempVec.x,4);
		reader->read(&tempVec.y,4);
		reader->read(&tempVec.z,4);
		return tempVec;
	};
};