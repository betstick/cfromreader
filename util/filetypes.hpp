#pragma once
#include "stdafx.hpp"

namespace cfr
{
	//only for formats we need to support
	enum CfrFileType
	{
		UNKOWN,
		FROM_BDF,
		FROM_BHD,
		FROM_BHF,
		FROM_BND,
		FROM_BTL,
		FROM_DCX,
		FROM_DRB,
		FROM_ENTRYFILELIST,
		FROM_ESD,
		FROM_EMEVD,
		FROM_FFX,
		FROM_FLVER,
		FROM_FMG,
		FROM_FSB,
		FROM_FEV,
		FROM_ITL,
		FROM_GFX,
		FROM_GPARAM,
		//FROM_HKS,
		//FROM_HKX,
		FROM_LUA,
		FROM_MSB,
		FROM_MTD,
		FROM_NGP,
		FROM_NVA,
		FROM_NVM,
		FROM_PARAM,
		FROM_SL2,
		FROM_TAE,
		FROM_TPF,
		HAVOK_HKS,
		HAVOK_HKX,
		HAVOK_HKXPWV,
		MS_DDS,
	};

	//take in an 8 char array
	CfrFileType determineFormat(char* magicBytes)
	{
		printf("format: %s\n",magicBytes);

		//TODO: get analysis of most common files, and order this
		//list so it uses less checks

		if(strncmp(magicBytes,"BDF3",4) == 0 || strncmp(magicBytes,"BDF4",4) == 0)
		{
			return FROM_BDF;
		}
		else if(strncmp(magicBytes,"BHD5",4) == 0)
		{
			return FROM_BHD;
		}
		else if(strncmp(magicBytes,"BHF3",4) == 0 || strncmp(magicBytes,"BHF4",4) == 0)
		{
			return FROM_BHF;
		}
		else if(strncmp(magicBytes,"BND3",4) == 0 || strncmp(magicBytes,"BND4",4) == 0)
		{
			return FROM_BND;
		}
		else if(false)
		{
			return FROM_BTL;
		}
		else if(strncmp(magicBytes,"DCX",3) == 0)
		{
			return FROM_DCX;
		}
		else if(false)
		{
			return FROM_DRB;
		}
		else if(false)
		{
			return FROM_ENTRYFILELIST;
		}
		else if(false)
		{
			return FROM_ESD;
		}
		else if(false)
		{
			return FROM_EMEVD;
		}
		else if(false)
		{
			return FROM_FFX;
		}
		else if(strncmp(magicBytes,"FLVER",5) == 0)
		{
			return FROM_FLVER;
		}
		else if(false)
		{
			return FROM_FSB;
		}
		else if(false)
		{
			return FROM_FEV;
		}
		else if(false)
		{
			return FROM_ITL;
		}
		else if(false)
		{
			return FROM_GFX;
		}
		else if(false)
		{
			return FROM_GPARAM;
		}
		else if(false)
		{
			return FROM_LUA;
		}
		else if(strncmp(magicBytes,"MSB ",4) == 0)
		{
			return FROM_MSB;
		}
		else if(false)
		{
			return FROM_MTD;
		}
		else if(false)
		{
			return FROM_NGP;
		}
		else if(false)
		{
			return FROM_NVA;
		}
		else if(false)
		{
			return FROM_NVM;
		}
		else if(false)
		{
			return FROM_PARAM;
		}
		else if(false)
		{
			return FROM_SL2;
		}
		else if(strncmp(magicBytes,"TAE ",4) == 0)
		{
			return FROM_TAE;
		}
		else if(strncmp(magicBytes,"TPF\0",4) == 0)
		{
			return FROM_TPF;
		}
		else if(false)
		{
			return HAVOK_HKS;
		}
		else if(
			(magicBytes[4] == 'T') && 
			(magicBytes[5] == 'A') && 
			(magicBytes[6] == 'G') &&
			(magicBytes[7] == '0')
		)
		{
			return HAVOK_HKX;
		}
		else if(
			(magicBytes[0] == 1) && 
			(magicBytes[1] == 0) && 
			(magicBytes[2] == 0) &&
			(magicBytes[3] == 0) &&
			(magicBytes[4] == 0) &&
			(magicBytes[5] == 0) &&
			(magicBytes[6] != 0) &&
			(magicBytes[7] == 0)
		)
		{
			return HAVOK_HKXPWV;
		}
		else if(false)
		{
			return MS_DDS;
		}
		else
		{
			throw std::runtime_error("Unkown File Type!\n");
		}

		//return UNKOWN;
	};
};