#ifndef CFR_BHD5_1__
#define CFR_BHD5_1__

#include "bhd5_1.hpp"

namespace cfr
{
	BHD5_1::BHD5_1(FILE* headerSrc) //, FILE* dataSrc)
	{
		if(headerSrc == NULL)
			throw std::runtime_error("Header file is NULL!\n");
		
		//just to be safe, reset to beginning
		fseek(headerSrc,0,SEEK_SET);

		Header header;
		fread(&header,sizeof(Header),1,headerSrc);

		printf("bucket count: %i\n", header.bucketCount);

		if(memcmp(header.magic,"BHD5",4) != 0)
			throw std::runtime_error("File is not a BHD5!\n");

		Bucket* buckets = (Bucket*)malloc(header.bucketCount*sizeof(Bucket));

		//read *all* the buckets in one supermassive dump!
		fread(&buckets[0],sizeof(Bucket),header.bucketCount,headerSrc);

		printf("bucket[0].count: %i\n",buckets[0].fileHeaderCount);

		//declared out of loop to hopefully improve speed
		int32_t hash;
		FileHeader fh;

		//go to each bucket and add its fileheaders to the hashMap
		for(int i = 0; i < header.bucketCount; i++)
		{
			fseek(headerSrc,buckets[i].fileHeadersOffset,SEEK_SET);

			for(int i = 0; i < buckets[i].fileHeaderCount; i++)
			{
				fread(&hash,sizeof(int32_t),1,headerSrc);

				//not sure why, but these needed to be separated?
				fread(&fh.fileSize,sizeof(int32_t),1,headerSrc);
				fread(&fh.offset,sizeof(int64_t),1,headerSrc);

				this->hashMap.insert({hash,fh});
			}
		}

		//free this cause its probably on the heap!
		free(buckets);
	};
};

#endif