Refer here for most of the useful info: http://soulsmodding.wikidot.com/format:flver
If it doesn't exist yet, I've got some info here too!

Appears to be From's internal "mesh" format. Includes texture, uv, bone, etc support. Commonly found in use for actors/characters. Need to confirm wether it's used for map data as well. Lots of tools and scattered info exists so far. I'm trying to consolidate some of it here.

Files with FLVER0 formatted header are for everything prior to Dark Souls PTDE. Dark souls PTDE and all later titles appear to utilize FLVER2 version of the format. Will add differences later.

This utility is only for FLVER2 formats so I have no idea what the difference is between the 0 and 2 designations.

Vertex buffers are stored in a layout specifically set up to be loaded directly into From's shaders. This makes it very fast but also makes general purpose manipulation very difficult. For now this library leans into general purpose and converts everything to more "normal" values.

A problem I've run into is that the vertexBuffer comes before the bufferLayout. The vertex buffer would then have to either be ignored, stored then pushed into an interpreter and stored again, or something like that. Seems complex.

Perhaps need to separate the FLVER into three parts. Interpret, organize, read. Raw FLVER data, even if formatted is difficult to work with.