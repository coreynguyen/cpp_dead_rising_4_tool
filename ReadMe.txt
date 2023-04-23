Unpacking Utility for Dead Rising 4 on PC
By Corey Nguyen

Dependencies:
devIL for image processing
zlib for decompressing and compressing the archives
zlib complete to make working in zlib easier
dirent for filesystem functions that work on both posix and windows (probably dont need this)

Copyright:
copyright is held by the respective authors of each libraries listed above and I claim no ownship over them
audio used was copied from my installation of Capcom vs Marvel 3, copyright of CAPCOM.

Overview:
During COVID-19 I had some time to look at some projects, one of these being DR4.
Before being able to tackle the models I first had to create an archive unpacker and packer.
When this task completed I discovered that any changes I made to the game caused it to crash.

After rigorously double checking the file structure and eventually decided on doing just a simple byte change.
I opened a random archive and changed a random byte by one bit… and oddly this was enough to crash the game.
This is very abnormal so we must assume the game checks integrity of EVERY file either actively or on at  loading. This meant that modding on the game was NOT EVEN POSSIBLE and the weeks spent making the archive utility was wasted.
Shared all tests, findings and the archive tool “as-is” before abandoning the project
https://deadrising2mods.proboards.com/thread/3098/dead-rising-modding-protection-modded

Made an explanation video going over the circumstances about how anti-consumer DRM and also to call out for any help to defeat the security protection.
https://www.youtube.com/watch?v=jdY0_eVVxTY

In fact, the security was bright down few years later but not because modders but for money - ha why else. Around April of 2022 Intel released their new CPU architecture to compete against a foreign chip manufacturer’s. Intel’s new design capitalizes on chipets thereby cutting up last years CPU’s and gluing them into one Frankenstein’s chip. Because of this approach the way the CPU do tasks changed, causing games to break. This is because many game use a memory and file protection called Denuvo which relies on these things to operate in a certain manner. 
Surprisingly enough CAPCOM sent out an update that removed the Denuvo protection on many of their past games instead of waiting for Denuvo to update the protection.
I was made ware of this but I am hesitant to work on any of these games because CAPCOM can throw the switch ON at any moment without notice. Also COVID is over and I don’t have time to look at this game any more.



Notes: <August 8, 2020>
Reviewing the .big files housing geometry related data, there seems a pattern of the type of files. Will have to documented them one by one and understand each structure and its purpose in the archive

** in earlier versions they .big and in later versions they used .mdl, so both extensions should be checked. Just need to check file list for a name “SceneHeader” could also just check for its hash 0xb91be525

_ATTRIBUTE_
Always 4 bytes long, always contains the number 3 as a LONG ?

_COLLPRIMS_
Empty file 16 bytes long

MatArray
Contains a list of data, however it does change in size, need to find the count
There appears to be a count in the SceneHeader, which I can use for calculating a pseudo version number for now… 
Stride 36 =	Dead Rising 2 
Stride 36 =	Dead Rising 2 OTR
Stride 80 =	Dead Rising 3
Stride 28 =	Dead Rising 4

MatTextureInfoArray
Contains a list of texture names, however it does change in size, need to find the count
Dead Rising 2 		= 100 bytes
Dead Rising 2 OTR 	= 100 bytes
Dead Rising 3 		= 16 bytes
Dead Rising 4 		= 16 bytes

In dr2/otr there are names and texture info together, in dr3 they separated the names into another file, called TextureNames

TextureNames
Seems to contain a string buffer, shows upin dr3 and dr4
File is a bit more complex, unlike the other files which reserved a string space. This is more dynamic and instead of having pointers it just has sizes which is stupid
SHORT count
SHORT[count] strlength // this includes the 0 termination at the end of the string
Buffer[n] bytes

TotalIBSize
4byte long int for the index buffer count I presume

TotalVBSize
4byte long int for the vertex buffer count I presume

SceneDescription
List data, 36 bytes per line. Seems to be a string each 36 bytes. Seems like node names, mesh and bone names maybe? File is only present on DR2 – DR3, and in DR4 it is merged with the Scene Header

SceneHeader
In DR4 the SceneDescription is merged
Otherwise file is 20 bytes and contains 5 counts as Long ints 
LONG always 1
LONG version1
LONG version2
LONG SceneDescription  count
LONG MatArray Count

The scene Header in dr4 is completely different, need to detect version in a backwards compatible way. At offset 4, read a short value and that should work.
Therefore these Ints can determine Version
Short 2 =	Dead Rising 2 
Short 2 =	Dead Rising 2 OTR
Short 3 =	Dead Rising 3
Short 136 =	Dead Rising 4

Shared*
?material or shader related, code gets very complex

_BONENAMES_
31bytes per entry
List of bone names
Shares names references that would otherwise be in the SceneDescription, I guess this tells us specifically which are bones. However this file only shows up ina  few files so huh

LocatorArray
Some type of data

NumLocatorArray
4bytes, holds a single long value probably count for LocatorArray

UsedShaders.txt
Plain text file contains a list in this format
0x5CEF4D20 : ProtoSkinnedClothRGB

_BOUNDINGBOX_
24 bytes long, seems to contain 3 floats for min and 3 floats for max

_ANIMCHANNELINFO_ 
??
_ANIMLIB_SKELETON_
Not really sure if this is an animation file, or just a bone map to fit a master skeleton?
There entry list at the top 36bytes long, followed by a string buffer

MatParameterArray
This file is really dense, shows up on dr3 and dr4. Maybe holds shader related things

MatSamplerInfoArray
Shows up on dr3 and dr4 and seems to be the remaing data that was removed from 
MatTextureInfoArray
The file contains entries 40bytes per, with a series of ints followed by a series of floats

valuesets.big
contains files with the same type and namoclature 
7c5f7500.scv

Structure inside the file is rather dynamic looking, however there is always a name defined at the top;
boss_chef_mtl_arm.pdv

seems possible it related to materials in a large way. The file then seems to contain a list, maybe for animations?

persistent.big
only present in DR2, OTR and is missing in DR3 and Dr4
contains more file types, more notably there is data files that mirror names in the root of the .big which means this provides some additional contextual data to the main files… dang
whats confusing is some __ files which I’d assume are info files for other data files which being such I would expect that all to be in in the root. And in some cases some are.. and some are in the stupid persistent. So a bit confusing why they bothered nesting files in some and not in others..

LodHeader
?
_BONEHIERARCHY_
I’m assuming it’s the parent info, looks like LONG index, LONG parent

__STRING_STORAGE__
? random text

_COLLISIONLIST_
40bytes long
Contains a list of entries, consisting of a name, hash, and Id


mBoneIDHierarchyMapIndex
? empty padded file

mRenderTransforms
? empty padded file

File Suffixes 
** These are suffixed to the end of each geometry related file which act as the header info to read the buffer data… the file base or prefix name needs to be read from the SceneDescription or in the later version the SceneHeader. Then I think based on the suffix of each name, we choose basically how we combine them into a complete mesh object… honest its looking messy as heck

I will need to break this layout down in terms of game version is suppose to reduce messiness
DR2
* Indices 0
* Vertices 0
\persistent\* IBHeader
\persistent\* RenderStrip
\persistent\* VBHeader
\persistent\* VDHeader
\persistent\*
Or
Combined Indices 2
Combined Vertices 56
In this case the last value is the entry stride…


Conclusion
I’m going to have to divide operation into operation of what is known and what is not known
I’m juggling with the idea of using external files, or just directly embedding them into the SMD scene file.
Something like
Scene
o	::RandomFile
o	AMAAAAAAAAAHQAAAA==
And just store the data in the node name as base64, maybe have a output node with a symbol in front of the name to denote a data connected?
