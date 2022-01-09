# Notes
this file is for general scrawlings and information i've gathered from the games, people, and code relating to souls structures.

| Format | Ext  | DS | DSR | DS2 | DS3 |
|--------|------|----|-----|-----|-----|
| BDF3   | bdt  |    |     |     |     |
| BDF4   | bdt  |    |     |     |     |
|xxxxxxxx|xxxxxx|xxxx|xxxxx|xxxxx|xxxxx|
| BHD5_1 | bhd5 |    |     |     |     |
| BHD5_2 | bhd  |    |     |     |     |
| BHF3   | bhd  |    |     |     |     |
| BHF4   | bhd  |    |     |     |     |
|xxxxxxxx|xxxxxx|xxxx|xxxxx|xxxxx|xxxxx|
| BND3   | bnd  |    |     |     |     |
| BND4   | bnd  |    |     |     |     |


bhd are headers, bdt are data in the format the header specifies.
headers can essentially be considred "maps" or trees of the files.
load these into ram since they're small and easy to work with.

bnd files are the containers for the files. groups of mesh/animation/textures for a complete "thing" like an NPC or an enemy. example:
the c2320 object has an anibnd, chrbnd, and a chrtpfbdt.
chr is mesh, ani is animation, and tpf is texture pack format?

bdt's are sometimes refered to as DVDBNDs. 

bhf3 and bhf4 are pre DS2 split archive headers

chrtpfbdt files have their header located in the corresponding chrbnd files. if no chrtpfbdt is specified, the tpf files will be in the chrbnd file.

sometimes the textures for certain "things" are located in OTHER things.

enemies do not have weapons. unless its a human character. or dark souls 2.
the "weapon" you see is a part of their model

game has a texture pool and anything can pull from anything
wyvern boss has no texture in its chrbnd. it pulls one that was loaded by the map.
this is "wrong" but can totally be done. extremely cursed.

the structure *generally* goes (bdt+bhd)/bnd/(textures/meshes/animations)
as mentioned before, this isn't always the case. sometimes From gets freaky.

need to add in a failure for if trying to compile on big endian systems. i will not support them.


the structures needed for DSR characters are as follows:
dcx->bnd->tae,flver,hkx,hkpwv
bdt->tpf->dds